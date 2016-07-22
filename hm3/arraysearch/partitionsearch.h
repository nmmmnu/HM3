#ifndef _PARTITION_SEARCH_ARRAY__SEARCH_H
#define _PARTITION_SEARCH_ARRAY__SEARCH_H

#include "arraysearch/base.h"

#include "ilist.h"

#include <vector>


namespace hm3{
namespace arraysearch{

template<class LOOKUP>
class PartitionSearch{
public:
	// it is extremly difficult to get underline list's size_type here
	using size_type = IList::size_type;

	static constexpr size_type DEFAULT_PARTITIONS	= 500;
	static constexpr size_type MIN_PARTITIONS_SIZE	= 1000;

private:
	using Container = std::vector<std::string>;

public:
	PartitionSearch(size_type const partitionsCount = DEFAULT_PARTITIONS) :
				partitionsCount_(partitionsCount < 2 ? partitionsCount : DEFAULT_PARTITIONS){
	}

	bool reset(){
		partitions_.clear();
		populated_ = false;
	}

	template <class ARRAY>
	bool populate(const ARRAY &array) const;

	template <class ARRAY>
	result_type<ARRAY> operator()(const ARRAY &self, const StringRef &key) const;

private:
	LOOKUP			lookup_;

	size_type		partitionsCount_;

	mutable bool		populated_	= false;
	mutable  Container	partitions_;
};


} // namespace arraysearch
} // namespace

// ==============================

#include "partitionsearch_impl.h"

#endif

