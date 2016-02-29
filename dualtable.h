#ifndef _DUAL_LIST_H
#define _DUAL_LIST_H

#include "ilist.h"

#include "multitableiterator.h"

template <class TABLE1, class TABLE2>
class DualTable : public IList<DualTable<TABLE1,TABLE2> >{
public:
	using Iterator = MultiTableIterator::Dual<TABLE1, TABLE2>;

public:
	explicit
	DualTable(const TABLE1 &table1, const TABLE2 &table2) :
					_table1(table1),
					_table2(table2){
	}

//	DualTable(DualTable &&other) = default;

public:
	Pair get(const StringRef &key) const;

//	count_type getCount() const{
//		return _table1.getCount() + _table2.getCount();
//	}

	size_t getSize() const{
		return _table1.getSize() + _table2.getSize();
	}

public:
	Iterator begin() const{
		return Iterator(_table1, _table2);
	}

	Iterator end() const{
		return Iterator(_table1, _table2, true);
	}

private:
	const TABLE1	&_table1;
	const TABLE2	&_table2;
};

// ===================================

template <class TABLE1, class TABLE2>
Pair DualTable<TABLE1, TABLE2>::get(const StringRef &key) const{
	const Pair &pair = _table1.get(key);

	if (pair)
		return pair;

	return _table2.get(key);
}

#endif
