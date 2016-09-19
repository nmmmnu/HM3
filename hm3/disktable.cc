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

inline BlobRef DiskTable::mmap2blob__(const MMAPFile &m){
	return { m.mem(), m.size() };
}

bool DiskTable::open(const std::string &filename){
	header_.open(diskfile::filenameMeta(filename));

	if (header_ == false)
		return false;

	mmapIndx_.open(diskfile::filenameIndx(filename));
	mmapData_.open(diskfile::filenameData(filename));

	mmapTree_.open(btreeindex::filenameIndx(filename));
	mmapKeys_.open(btreeindex::filenameData(filename));

	blobIndx_ =  mmap2blob__(mmapIndx_);
	blobData_ =  mmap2blob__(mmapData_);

	blobTree_ =  mmap2blob__(mmapTree_);
	blobKeys_ =  mmap2blob__(mmapKeys_);

	return true;
}

void DiskTable::close(){
	blobIndx_.reset();
	blobData_.reset();

	blobTree_.reset();
	blobKeys_.reset();

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

	size_t const nodesCount = blobTree_.size() / sizeof(Node);

	const Node *nodes = blobTree_.as<const Node>(0, nodesCount);

	if (!nodes){
		// go try with binary search
		log__("Problem, switch to binary search");
		return binarySearch_(key, result);
	}


	size_type bs_left  = 0;
	size_type bs_right = getCount();

	size_type pos = 0;

	while(pos < nodesCount){
		const Node &node = nodes[pos];

		branch_type node_index;
		bool needRight = true;

		// MODIFIED LEVEL ORDERED MINI-BINARY SEARCH
		{
			const auto &ll = g_ll;

			branch_type node_pos = 0;

			node_index = ll[node_pos];

			while (node_pos < VALUES){

				// ACCESS ELEMENT
				// ---
				const uint64_t offset = be64toh(node.values[ node_pos ]);

				if (offset == Node::NIL){
					// special case go left
					/*
					 *   1   0   2
					 * 1 2 n n n n n
					 *
					 *       n
					 *   2       n
					 * 1   n   n   n
					 *
					 */

					node_pos = branch_type(2 * node_pos + 1);

					log__("\t L: NIL");

					continue;
				}

				const NodeData *nd = blobKeys_.as<const NodeData>((size_t) offset);

				if (!nd){
					// go try with binary search
					log__("Problem, switch to binary search");
					return binarySearch_(key, result);
				}

				size_t    const keysize = static_cast<size_type>(be16toh(nd->keysize));
				size_type const dataid  = static_cast<size_type>(be64toh(nd->dataid));

				// key is just after the NodeData
				const char *keyptr = blobKeys_.as<const char>((size_t) offset + sizeof(NodeData), keysize);

				if (!keyptr){
					// go try with binary search
					log__("Problem, switch to binary search");
					return binarySearch_(key, result);
				}

				const StringRef keyx{ keyptr, keysize };
				// ---
				// EO ACCESS ELEMENT

				log__("node [", ll[node_pos], "]: POS:", node_pos, keyx);

				int const cmp = keyx.compare(key);

				if (cmp < 0){
					// this + 1 is because the element is checked already
					// see standard binary search implementation.
					// took me two days to figure it out :)
					node_index = branch_type( ll[node_pos] + 1 );

					// go right
					node_pos = branch_type( 2 * node_pos + 2 );

					bs_left = dataid;

					log__("\t R:", node_pos, "BS:", bs_left, "-", bs_right, "KEYX", keyx);
				}else if (cmp > 0){
					node_index = ll[node_pos];

					// go left
					node_pos = branch_type( 2 * node_pos + 1 );

					bs_right = dataid;

					needRight = false;

					log__("\t L:", node_pos, "BS:", bs_left, "-", bs_right, "KEYX", keyx);
				}else{
					// found

					log__("\t F: POS:", node_pos);

					result = dataid;
					return true;
				}
			}
		}
		// EO MODIFIED LEVEL ORDERED MINI-BINARY SEARCH

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


	// leaf or similar
	// fallback to binary search :)

	log__("BTREE LEAF:", pos);
	log__("Fallback to binary search", bs_left, bs_right, "diff", bs_right - bs_left);

	return binarySearch(*this, bs_left, bs_right, key, BinarySearchCompList{}, result, BIN_SEARCH_MINIMUM_DISTANCE);
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
	const uint64_t *ptr_be = (const uint64_t *) mmapIndx_.safeAccess( index * sizeof(uint64_t) );

	if (ptr_be){
		size_t const offset = be64toh( *ptr_be );

		return offset;
	}

	return static_cast<size_t>(-1);
}
#endif

const PairBlob *DiskTable::getAtFromDisk_(size_type const index) const{
	const uint64_t *ptrs_be = blobIndx_.as<const uint64_t>(0, getCount());

	if (!ptrs_be)
		return nullptr;

	const uint64_t ptr_be = ptrs_be[index];

	size_t const offset = (size_t) be64toh( ptr_be );

	// here unfortunately, we overrun the buffer, since PairBlob is variable size...
	const PairBlob *blob = blobData_.as<const PairBlob>(offset);

	// so we check overrun here...
	bool const overrun = nullptr == blobData_.safeAccessMemory(offset, blob->getBytes());

	if ( blob && !overrun)
		return validateFromDisk_(blob);

	return nullptr;
}

const PairBlob *DiskTable::getNextFromDisk_(const PairBlob *blobPrevious, size_t size) const{
	if (size == 0)
		size = blobPrevious->getBytes();

	const char *blobc = (const char *) blobPrevious;

	// here unfortunately, we overrun the buffer, since PairBlob is variable size...
	const PairBlob *blob = blobData_.as<const PairBlob>(blobc + size);

	// so we check overrun here...
	bool const overrun = nullptr == blobData_.safeAccessMemory(blob, blob->getBytes());

	if ( blob && !overrun)
		return validateFromDisk_(blob);

	return nullptr;
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
		tmp_pod = list_.getNextFromDisk_(tmp_pod, tmp_pair.getBytes() );
	}else{
		// get data via seek
		tmp_pod = list_.getAtFromDisk_(pos_);
	}

	tmp_pos = pos_;

	tmp_pair = tmp_pod;

	return tmp_pair;
}


} // namespace

