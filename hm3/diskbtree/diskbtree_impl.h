//#include <cmath>

#include <cstdio>

namespace hm3{
namespace diskbtree{


#if 0

constexpr
auto DiskBTree::ln__(offset_type const count, branch_type const branches) -> branch_type{
	// Biliana
	// log 54 (123) = ln (123) / ln (54)
	return branch_type(log(count) / log(branches) + 1);
}

#else

constexpr
auto DiskBTree::ln__(offset_type const count, branch_type const branches) -> branch_type{
	// C++11 constexpr sucks, needs recursion.
	return count < 1 ? branch_type(0) : branch_type(1 + ln__(count / branches, branches));
}

#endif


template <class LIST>
bool DiskBTree::createFromList(const LIST &list) const{
	auto const count = list.getCount();

	branch_type const levels = ln__(count, BRANCHES) - 1;

	printf("Records    : %zu\n", count);
	printf("Branching  : %u\n", BRANCHES);
	printf("Tree Depth : %u\n", levels);

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
		// leaf node - just flush everything into the node...
		if (this_level == level){
			Node node;
			node.leaf = 1;
			node.size = htobe16(size);

			for(branch_type i = 0; i < size; ++i){
				const StringRef &key = list.getAt(begin + i).getKey();

				// push the key
				file_data.write( key.data(), (std::streamsize) key.size() );

				NodeValue nv;
				nv.key  = htobe64( current );
				nv.data = htobe64( list.getAtOffset(begin + i) );

				node.values[i] = nv;

				current += key.size();
			}

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
			node.leaf = 0;
			node.size = htobe16(VALUES);

			for(branch_type i = 0; i < VALUES; ++i){
				const StringRef &key = list.getAt(begin + i).getKey();

				// push the key
				file_data.write( key.data(), (std::streamsize) key.size() );

				NodeValue nv;
				nv.key  = htobe64( current );
				nv.data = htobe64( list.getAtOffset( begin + distance * (i + 1) ) );

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

