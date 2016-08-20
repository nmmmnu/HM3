#include "disktable.h"

#include "pairblob.h"

#include "binarysearch.h"

#include "diskfile/diskfilefilenames.h"
#include "btreeindex/btreeindexfilenames.h"
#include "btreeindex/btreeindexnode.h"

#include "levelorderlookup.h"

#include <endian.h>	// htobe16

#include <iostream>

namespace hm3{

#include "logger.h"

//#define log__(...) /* nada */


const btreeindex::LevelOrderLookup<btreeindex::NODE_LEVELS> g_ll;


bool DiskTable::open(const std::string &filename){
	header_.open(diskfile::filenameMeta(filename));

	if (header_ == false)
		return false;

	mmapIndx_.open(diskfile::filenameIndx(filename));
	mmapData_.open(diskfile::filenameData(filename));

	mmapTree_.open(btreeindex::filenameIndx(filename));
	mmapKeys_.open(btreeindex::filenameData(filename));

	return true;
}

void DiskTable::close(){
	mmapIndx_.close();
	mmapData_.close();

	mmapTree_.close();
	mmapKeys_.close();
}

inline bool DiskTable::binarySearch_(const StringRef &key, size_type &result) const{
	return binarySearch(*this, getCount(), key, BinarySearchCompList{}, result, BIN_SEARCH_MINIMUM_DISTANCE);
}

inline bool DiskTable::search_(const StringRef &key, size_type &result) const{
	if (mmapTree_ && mmapKeys_){
		log__("btree");
		return btreeSearch_(key, result);
	}else{
		log__("binary");
		return binarySearch_(key, result);
	}
}


bool DiskTable::btreeSearch_(const StringRef &key, size_type &result) const{
	using Node		= btreeindex::Node;
	using NodeData		= btreeindex::NodeData;

	using branch_type	= btreeindex::branch_type;

	constexpr auto VALUES   = btreeindex::VALUES;
	constexpr auto BRANCHES = btreeindex::BRANCHES;

	size_type bs_left  = 0;
	size_type bs_right = getCount();

	size_type pos = 0;

	while(true){
		// ---
		const Node *node = (const Node *) mmapTree_.safeAccess(pos * sizeof(Node));

		if (!node){
			// go try with binary search
			log__("Problem, switch to binary search");
			return binarySearch_(key, result);
		}
		// ---

		// check if node is half full

		uint16_t const size_ = be16toh(node->size);

		bool const leaf		= size_ <= VALUES;


		branch_type node_index;
		bool needRight = true;

		// MODIFIED LEVEL ORDERED MINI-BINARY SEARCH
		{
			const auto &ll = g_ll;

			branch_type node_pos = 0;

			node_index = ll.fw(node_pos);

			while (node_pos < VALUES){

				// ACCESS ELEMENT
				// ---
				const uint64_t offset = be64toh(node->values[ node_pos ]);

				if (offset == Node::NIL){
					// special case, go left,
					// no need to update anything
					node_pos = 2 * node_pos + 1;

					log__("\t L: NIL");

					continue;
				}

				const NodeData *nd = (const NodeData *) mmapKeys_.safeAccess((size_t) offset);

				if (!nd){
					// go try with binary search
					log__("Problem, switch to binary search");
					return binarySearch_(key, result);
				}

				const uint16_t keysize = be16toh(nd->keysize);
				const uint64_t dataid  = be64toh(nd->dataid);

				const char *keyptr = (const char *) nd + sizeof(NodeData);

				const StringRef keyx{ keyptr, keysize };
				// ---
				// EO ACCESS ELEMENT

				log__("node [", node_index, "]: POS:", node_pos, keyx);

				int const cmp = keyx.compare(key);

				// not optimal way, but more clear
				if (cmp == 0){
					// found

					log__("\t F: POS:", node_pos);

					result = dataid;
					return true;
				}

				if (cmp < 0){
					// this + 1 is because the element is checked already
					// see standard binary search implementation.
					// took me two days to figure it out :)
					node_index = ll.fw(node_pos) + 1;

					// go right
					node_pos = 2 * node_pos + 2;

					bs_left = dataid;

					log__("\t R:", node_pos, "BS:", bs_left, "-", bs_right, "KEYX", keyx);
				}else{
					node_index = ll.fw(node_pos);

					// go left
					node_pos = 2 * node_pos + 1;

					bs_right = dataid;

					needRight = false;

					log__("\t L:", node_pos, "BS:", bs_left, "-", bs_right, "KEYX", keyx);
				}
			}
		}
		// EO MODIFIED LEVEL ORDERED MINI-BINARY SEARCH



		if (leaf){
			// leaf
			// fallback to binary search :)

			log__("BTREE LEAF:", pos);
			log__("Fallback to binary search", bs_left, bs_right, "diff", bs_right - bs_left);

			return binarySearch(*this, bs_left, bs_right, key, BinarySearchCompList{}, result, BIN_SEARCH_MINIMUM_DISTANCE);
		}

		if (needRight){ //node_index == size){
			// Go Right
			pos = pos * BRANCHES + VALUES + 1;

			log__("BTREE R:", pos);
		}else{
			// Go Left
			pos = pos * BRANCHES + node_index + 1;

			log__("BTREE L:", pos, "node[", node_index, "]");
		}
	}

	// never go there
}

Pair DiskTable::get(const StringRef &key) const{
	size_type result;
	bool const found = search_(key, result);

	return found ? getAt( result ) : nullptr;
}

auto DiskTable::lowerBound(const StringRef &key) const -> Iterator{
	size_type result;
	/* bool const found = */ search_(key, result);

	return Iterator(*this, result, header_.getSorted());
}

int DiskTable::cmpAt(size_type const index, const StringRef &key) const{
	const PairBlob *p = getAtFromDisk_(index);
	// StringRef is not null terminated
	return p ? p->cmp(key.data(), key.size()) : Pair::CMP_ZERO;
}

const PairBlob *DiskTable::validateFromDisk_(const PairBlob *blob) const{
	if (blob == nullptr)
		return nullptr;

	if (! validate_)
		return blob;

	return blob->validChecksum() ? blob : nullptr;
}

#if 0
size_t DiskTable::getAtOffset(size_type const index) const{
	const uint64_t *ptr_be = (const uint64_t *) mmapIndx_.safeAccess( (size_t) index * sizeof(uint64_t) );

	if (ptr_be){
		size_t const offset = (size_t) be64toh( *ptr_be );

		return offset;
	}

	return (size_t) -1;
}
#endif

const PairBlob *DiskTable::getAtFromDisk_(size_type const index) const{
	const uint64_t *ptr_be = (const uint64_t *) mmapIndx_.safeAccess( (size_t) index * sizeof(uint64_t) );

	if (ptr_be){
		size_t const offset = (size_t) be64toh( *ptr_be );

		const PairBlob *blob = (const PairBlob *) mmapData_.safeAccess( offset );
		return validateFromDisk_(blob);
	}

	return nullptr;
}

const PairBlob *DiskTable::getNextFromDisk_(const PairBlob *blob, size_t size) const{
	if (size == 0)
		size = blob->getMemSize();

	const char *blobc = (const char *) blob;

	const PairBlob *blobNext = (const PairBlob *) mmapData_.safeAccess( blobc + size );
	return validateFromDisk_(blobNext);
}

// ===================================

DiskTable::Iterator::Iterator(const DiskTable &list, size_type const pos, bool const useFastForward) :
			list_(list),
			pos_(pos),
			useFastForward_(useFastForward){}

const Pair &DiskTable::Iterator::operator*() const{
	if (tmp_pod && tmp_pos == pos_)
		return tmp_pair;

	if (useFastForward_ && tmp_pod && tmp_pos == pos_ - 1){
		// get data without seek, walk forward
		// this gives 50% performance
		tmp_pod = list_.getNextFromDisk_(tmp_pod, tmp_pair.getMemSize() );
	}else{
		// get data via seek
		tmp_pod = list_.getAtFromDisk_(pos_);
	}

	tmp_pos = pos_;

	tmp_pair = tmp_pod;

	return tmp_pair;
}


} // namespace





#if 0

		for(branch_type i = 0; i < size; ++i){
			// ---
			const uint64_t offset = be64toh(node->values[i]);

			const NodeData *nd = (const NodeData *) mmapKeys_.safeAccess((size_t) offset);

			if (!nd){
				// go try with binary search
				log__("Problem, switch to binary search\n");
				return binarySearch_(key, result);
			}

			const uint16_t keysize = be16toh(nd->keysize);
			const uint64_t dataid  = be64toh(nd->dataid);

			const char *keyptr = (const char *) nd + sizeof(NodeData);

			const StringRef keyx{ keyptr, keysize };
			// ---

			int const cmp = key.compare(keyx);

			if (cmp == 0){
			//	printf("Found\n");
				result = be64toh(nd->dataid);
				return true;
			}

			if (cmp < 0){
				pos = pos * BRANCHES + i + 1;
			//	printf("L -> %zu %u\n", pos, i);

				bs_right = dataid;

				needGoRight = false;
				break;
			}

			bs_left = dataid;
		}



		// MODIFIED MINI-BINARY SEARCH
		{
			/*
			 * Lazy based from Linux kernel...
			 * http://lxr.free-electrons.com/source/lib/bsearch.c
			 */

			branch_type start = 0;
			branch_type end   = size;

			while (start < end){
			//	branch_type mid = start + ((end - start) /  2);
				branch_type mid = branch_type(start + ((end - start) >> 1)); // 4% faster


				// ACCESS ELEMENT
				// ---
				const uint64_t offset = be64toh(node->values[ mid ]);

				const NodeData *nd = (const NodeData *) mmapKeys_.safeAccess((size_t) offset);

				if (!nd){
					// go try with binary search
					log__("Problem, switch to binary search");
					return binarySearch_(key, result);
				}

				const uint16_t keysize = be16toh(nd->keysize);
				const uint64_t dataid  = be64toh(nd->dataid);

				const char *keyptr = (const char *) nd + sizeof(NodeData);

				const StringRef keyx{ keyptr, keysize };
				// ---
				// EO ACCESS ELEMENT

				int const cmp = keyx.compare(key);

				// not optimal way, but more clear
				if (cmp == 0){
					// found

					log__("node F: POS:", mid);

					result = dataid;
					return true;
				}

				if (cmp < 0){
					// go right
					start = branch_type(mid + 1);

					bs_left = dataid;

					log__("node R:", start, end, "BS:", bs_left, "-", bs_right, "KEYX", keyx);
				}else{
					// go left
					end = mid;

					bs_right = dataid;

					log__("node L:", start, end, "BS:", bs_left, "-", bs_right, "KEYX", keyx);
				}
			}

			node_index = start;
		}
		// EO MODIFIED BINARY SEARCH
#endif

