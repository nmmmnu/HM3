#include <cstdio>
#include <cstring>

#include <endian.h>

#include <iostream>


namespace hm3{
namespace btreeindex{


template <class LIST>
constexpr LevelOrderLookup<NODE_LEVELS> BTreeIndexBuilder<LIST>::llHolder_;


template <class LIST>
bool BTreeIndexBuilder<LIST>::createFromList(const StringRef &filename, const LIST &list){
	auto const count = list.getCount();

	levels_ = calcDepth1__(count);

	printf("Records          : %zu\n",		size_t{ count }	);
	printf("Branching Factor : %u (const)\n",	BRANCHES	);
	printf("Tree Depth       : %u\n",		levels_		);

	file_indx_.open(filenameIndx(filename),	std::ios::out | std::ios::binary);
	file_data_.open(filenameData(filename),	std::ios::out | std::ios::binary);

	current_ = 0;

	for(level_type level = 0; level < levels_; ++level){
		printf("Processing level %u...\n", level);

		reorder(list, 0, count, level);
	}

	file_indx_.close();
	file_data_.close();

	return true;
}


template <class LIST>
auto BTreeIndexBuilder<LIST>::calcDepth__(size_type count) -> level_type{
	// Biliana
	// log 54 (123) = ln (123) / ln (54)
	// but this is true for B+Tree only...

	level_type result = 0;

	while(count > 0){
		++result; // tree is always 1 level.

		if (count > VALUES){
			// We substract VALUES,
			// because BTree have data in non-leaf nodes.
			count = (count - VALUES) / BRANCHES;
		}else{
			break;
		}
	}

	return result;
}


template <class LIST>
void BTreeIndexBuilder<LIST>::injectEmptyNode_(level_type const level, level_type const this_level){
	if (this_level == level){
		// add empty node

		Node node;

		// fill gets refference to NIL
		auto const nil = Node::NIL;
		std::fill_n(node.values, VALUES, nil);

		// push the node
		file_indx_.write( (const char *) & node, sizeof node );
	}else if (this_level < level){
		// add children
		for(branch_type i = 0; i < BRANCHES; ++i)
			injectEmptyNode_( level, level_type(this_level + 1) );
	}
}


template <class LIST>
void BTreeIndexBuilder<LIST>::injectValue_(const LIST &list, size_type const index){
	// we need to have the pair,
	// because key "live" inside it.
	const auto &p = list.getAt(index);
	const StringRef &key = p.getKey();

	{
		NodeData nd;
		nd.keysize = htobe16(key.size());
		nd.dataid  = htobe64(index);

		// push NodeData
		file_data_.write( (const char *) &nd, sizeof nd );

		// push the key
		file_data_.write( key.data(), (std::streamsize) key.size() );
	}

	current_ += sizeof(uint16_t) + sizeof(uint64_t) + key.size();
}


template <class LIST>
void BTreeIndexBuilder<LIST>::reorder(const LIST &list,
				size_type const begin, size_type const end,
				level_type const level, level_type const this_level){

	// can't happen, but is good safeguard.
	if (this_level > level || begin >= end){
		if (begin >= end)
			injectEmptyNode_(level, this_level);

		return;
	}

	const auto &ll = llHolder_.value;

	using size_type = typename LIST::size_type;

	size_type const size = end - begin;

	if (size <= VALUES){
		// IMPORTANT: BECAUSE WE SUBSTRACT LEAF LEVEL,
		// CODE WILL GO HERE ONLY IF FIRST LEVEL IS LEAF

		//printf("We are in leaf level!!!\n");

		// leaf node - just flush everything into the node...
		if (this_level == level){
			Node node;

			for(branch_type j = 0; j < VALUES; ++j){
				branch_type const i = ll[j];

				if (i >= size){
					node.values[j] = Node::NIL;
					continue;
				}

				node.values[j] = htobe64( current_ );

				size_type const index = begin + i;

				injectValue_(list, index);
				// and current_ is increased here...
			}

			// push the node
			file_indx_.write( (const char *) & node, sizeof node );

		}else{ // if (this_level < level){
			// there are no children here.
			for(branch_type i = 0; i < BRANCHES; ++i)
				injectEmptyNode_( level, level_type(this_level + 1) );
		}

		//return;
	}else{
		size_type const distance = size / BRANCHES;

		if (this_level == level){
			Node node;

			for(branch_type j = 0; j < VALUES; ++j){
				branch_type const i = ll[j];

				node.values[j] = htobe64( current_ );

				size_type const index = begin + distance * (i + 1);

				injectValue_(list, index);
				// and current_ is increased here...
			}

			// push the node
			file_indx_.write( (const char *) & node, sizeof node );

		}else{ // if (this_level < level){
			// add the children

			size_type b = begin;
			for(branch_type i = 0; i < VALUES; ++i){
				size_type e = begin + distance * (i + 1);

				reorder(list, b, e, level, level_type(this_level + 1) );

				b = e + 1;
			}

			// last right child
			reorder(list, b, end, level, level_type(this_level + 1) );
		}

		//return;
	}
}

} // namespace btreeindex
} // namespace

