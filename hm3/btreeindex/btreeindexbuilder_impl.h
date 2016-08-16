#include <cstdio>
#include <cstring>

#include <iostream>

namespace hm3{
namespace btreeindex{


class BTreeIndexBuilder::BTreeIndexBuilderHelper_{
public:
	BTreeIndexBuilderHelper_(std::ofstream &&file_indx, std::ofstream &&file_data) :
					file_indx(std::move(file_indx)),
					file_data(std::move(file_data)){}

public:
	template <class LIST>
	void reorder(const LIST &list,
				typename LIST::size_type begin, typename LIST::size_type end,
				branch_type level, branch_type this_level = 0);

private:
	template <class LIST>
	NodeValue prepareNode_(const LIST &list, typename LIST::size_type index);

	void injectEmptyNode_(branch_type level, branch_type this_level){
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
				injectEmptyNode_( level, branch_type(this_level + 1) );
		}
	}

private:
	std::ofstream	file_indx;
	std::ofstream	file_data;
	size_t		current		= 0;
};


template <class LIST>
bool BTreeIndexBuilder::createFromList(const LIST &list) const{
	auto const count = list.getCount();

	branch_type const levels = calcDepth1__(count);

	printf("Records          : %zu\n",		count	);
	printf("Branching Factor : %u (const)\n",	BRANCHES);
	printf("Tree Depth       : %u\n",		levels	);

	std::ofstream fileIndx(filename_indx,	std::ios::out | std::ios::binary);
	std::ofstream fileData(filename_data,	std::ios::out | std::ios::binary);

	BTreeIndexBuilderHelper_ builder(std::move(fileIndx), std::move(fileData));

	for(branch_type level = 0; level < levels; ++level){
		printf("Processing level %u...\n", level);

		builder.reorder(list, 0, count, level);
	}

	return true;
}


auto BTreeIndexBuilder::calcDepth__(offset_type count) -> branch_type{
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




template <class LIST>
NodeValue BTreeIndexBuilder::BTreeIndexBuilderHelper_::prepareNode_(const LIST &list, typename LIST::size_type index){
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

	current += key.size();

	return nv;
}

template <class LIST>
void BTreeIndexBuilder::BTreeIndexBuilderHelper_::reorder(const LIST &list,
				typename LIST::size_type begin, typename LIST::size_type end,
				branch_type level, branch_type this_level){
	if (begin >= end)
		return injectEmptyNode_(level, this_level);

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
				node.values[i] = prepareNode_(list, begin + i);
			}

			//if (MEMSET_UNUSED_NODES)
			//	memset( & node.values[size], 0, VALUES - size);

			// push the node
			file_indx.write( (const char *) & node, sizeof node );

		}else if (this_level < level){
			// there are no children here.
			for(branch_type i = 0; i < BRANCHES; ++i)
				injectEmptyNode_( level, branch_type(this_level + 1) );
		}

		//return;
	}else{
		size_type const distance = size / BRANCHES;

		if (this_level == level){
			Node node;
			node.size = htobe16(VALUES);
			node.leaf = 0;

			for(branch_type i = 0; i < VALUES; ++i){
				node.values[i] = prepareNode_(list, begin + distance * (i + 1));
			}

			// push the node
			file_indx.write( (const char *) & node, sizeof node );

		}else if (this_level < level){
			// add the children

			size_type b = begin;
			for(branch_type i = 0; i < VALUES; ++i){
				branch_type e = branch_type(begin + distance * (i + 1));

				reorder(list, b, e, level, branch_type(this_level + 1) );

				b = e + 1;
			}

			// last right child
			reorder(list, b, end, level, branch_type(this_level + 1) );
		}

		//return;
	}
}

} // namespace btreeindex
} // namespace

