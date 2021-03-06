#ifndef _DUAL_LIST_H
#define _DUAL_LIST_H

#include "ilist.h"

#include "multitableiterator/dualiterator.h"


namespace hm3{


template <class TABLE1, class TABLE2>
class DualTable : public List<DualTable<TABLE1,TABLE2> >{
public:
	using Iterator = multitableiterator::DualIterator<TABLE1, TABLE2>;

public:
	DualTable(const TABLE1 &table1, const TABLE2 &table2) :
					table1_(table1),
					table2_(table2){
	}

//	DualTable(DualTable &&other) = default;

public:
	Pair get(const StringRef &key) const{
		const Pair &pair = table1_.get(key);

		if (pair)
			return pair;

		return table2_.get(key);
	}

//	size_type getCount() const{
//		return _table1.getCount() + _table2.getCount();
//	}

//	size_t getBytes() const{
//		return table1_.getBytes() + table2_.getBytes();
//	}

public:
	Iterator begin() const{
		return Iterator(table1_, table2_);
	}

	Iterator end() const{
		return Iterator(table1_, table2_, true);
	}

private:
	const TABLE1	&table1_;
	const TABLE2	&table2_;
};

} // namespace


#endif
