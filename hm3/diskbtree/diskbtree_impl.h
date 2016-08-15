//#include <cmath>

#include <cstdio>
#include <cstring>

#include <iostream>

namespace hm3{
namespace diskbtree{


auto DiskBTree::calcDepth__(offset_type count) -> branch_type{
	// Biliana
	// log 54 (123) = ln (123) / ln (54)
	// but this is true for B+Tree...

	branch_type result = 0;

	while(count > 0){
		++result; // tree is always 1 level.

		if (count > VALUES){
			// We substract VALUES,
			// because BTree have data in non-leaf nodes.
			count = (count - VALUES) / BRANCHES;
		}else{
			// count = 0;
			break;
		}
	}

	return result;
}

auto DiskBTree::calcDepth1__(offset_type const count) -> branch_type{
	branch_type result = calcDepth__(count);

	return result > 1 ? result - 1 : 1;
}

template <class LIST>
bool DiskBTree::createFromList(const LIST &list) const{
	auto const count = list.getCount();

	branch_type const levels = calcDepth1__(count);

	printf("Records          : %zu\n", count);
	printf("Branching Factor : %u (const)\n", BRANCHES);
	printf("Tree Depth       : %u\n", levels);

	size_t current = 0;
	std::ofstream fileIndx(filename_indx,	std::ios::out | std::ios::binary);
	std::ofstream fileData(filename_data,	std::ios::out | std::ios::binary);

	for(branch_type level = 0; level < levels; ++level){
		printf("Processing level %u...\n", level);

		reorder_(list,
				/* begin */ 0, count,
				level, /* this level */ 0,
					current,
					fileIndx,
					fileData);
	}

	return true;
}


void DiskBTree::injectEmptyNode_(branch_type level, branch_type this_level,
					std::ofstream &file_indx) const{
	if (this_level == level){
		// add empty node

		Node node;
		node.leaf = 1;
		node.size = 0;

		// push the node
		file_indx.write( (const char *) & node, sizeof node );

		return;
	}

	if (this_level < level){
		// add children
		for(branch_type i = 0; i < BRANCHES; ++i)
			injectEmptyNode_( level, branch_type(this_level + 1), file_indx);
	}
}


template <class LIST>
void DiskBTree::reorder_(const LIST &list,
				typename LIST::size_type const begin, typename LIST::size_type const end,
				branch_type const level, branch_type const this_level,
					size_t &current,
					std::ofstream &file_indx,
					std::ofstream &file_data) const{
	if (begin >= end)
		return injectEmptyNode_(level, this_level, file_indx);

	using size_type = typename LIST::size_type;

	size_type const size = end - begin;

	if (size <= VALUES){
		// IMPORTANT: BECAUSE WE SUBSTRACT LEAF LEVEL,
		// CODE WILL GO HERE ONLY IF FIRST LEVEL IS LEAF

		//printf("We are in leaf level!!!\n");

		// leaf node - just flush everything into the node...
		if (this_level == level){
			Node node;
			node.size = htobe16(size);
			node.leaf = 1;

			for(branch_type i = 0; i < size; ++i){
				const auto index = begin + i;

				// we need to key the pair,
				// because key "live" inside it.
				const auto &p = list.getAt(index);
				const StringRef &key = p.getKey();

				// push the key
				file_data.write( key.data(), (std::streamsize) key.size() );

				NodeValue nv;
				nv.key     = htobe64( current );
				nv.data    = htobe64( list.getAtOffset(index) );
				nv.keysize = htobe16( key.size() );

				node.values[i] = nv;

				current += key.size();
			}

			if (MEMSET_UNUSED_NODES)
				memset( & node.values[size], 0, VALUES - size);

			// push the node
			file_indx.write( (const char *) & node, sizeof node );

		}else if (this_level < level){
			// there are no children here.
			for(branch_type i = 0; i < BRANCHES; ++i)
				injectEmptyNode_( level, branch_type(this_level + 1), file_indx );
		}

		//return;
	}else{
		size_type const distance = size / BRANCHES;

		if (this_level == level){
			Node node;
			node.size = htobe16(VALUES);
			node.leaf = 0;

			for(branch_type i = 0; i < VALUES; ++i){
				const auto index = begin + distance * (i + 1);

				// we need to key the pair,
				// because key "live" inside it.
				const auto &p = list.getAt(index);
				const StringRef &key = p.getKey();

				//if (level == 0)
				//	printf("%5u | %*s\n", i, (int)key.size(), key.data());

				// push the key
				file_data.write( key.data(), (std::streamsize) key.size() );

				NodeValue nv;
				nv.key     = htobe64( current );
				nv.data    = htobe64( list.getAtOffset( index ) );
				nv.keysize = htobe16( key.size() );

				node.values[i] = nv;

				current += key.size();
			}

			// push the node
			file_indx.write( (const char *) & node, sizeof node );

		}else if (this_level < level){
			// add the children

			size_type b = begin;
			for(branch_type i = 0; i < VALUES; ++i){
				branch_type e = branch_type(begin + distance * (i + 1));

				reorder_(list, b, e, level, branch_type(this_level + 1),
							current,
							file_indx,
							file_data);

				b = e + 1;
			}

			// last right child
			reorder_(list, b, end, level, branch_type(this_level + 1),
							current,
							file_indx,
							file_data);
		}

		//return;
	}
}

} // namespace diskfile
} // namespace

