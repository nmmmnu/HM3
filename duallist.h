#ifndef _DUAL_LIST_H
#define _DUAL_LIST_H

#include "ilist.h"

#include "multitableiterator.h"

#include <utility>	// std::forward

template <class LIST1, class TABLE2>
class DualList : public IMutableList<DualList<LIST1,TABLE2> >{
public:
	constexpr static size_t MAX_SIZE = 1 * 1024 * 1024;

	using Iterator = MultiTableIterator::Dual<LIST1, TABLE2>;

private:
	LIST1	_memlist;
	TABLE2	_table;
	size_t	_maxSize;

public:
	explicit
	DualList(LIST1 &&memlist, TABLE2 &&table, size_t const maxSize = MAX_SIZE) :
					_memlist(std::move(memlist)),
					_table  (std::move(table)),
					_maxSize(maxSize > MAX_SIZE ? maxSize : MAX_SIZE){
	}

	DualList(DualList &&other) = default;

	~DualList(){
		flush();
	}

public:
	LIST1 &getMemList(){
		return _memlist;
	}

	TABLE2 &getTable(){
		return _table;
	}

public:
	Pair get(const StringRef &key) const;

	bool remove(const StringRef &key){
		// remove by inserting tombstone
		return _putT( Pair::tombstone(key) );
	}

	size_t getSize() const{
		return _memlist.getSize();
	}

public:
	// needs to be public because of CRPT
	template <class UPAIR>
	bool _putT(UPAIR &&data){
		return _memlist.put( std::forward<UPAIR>(data) );
	}

public:
	bool flush();

public:
	Iterator begin() const{
		return Iterator(_memlist, _table);
	}

	Iterator end() const{
		return Iterator(_memlist, _table, true);
	}

};

// ===================================

template <class LIST1, class TABLE2>
Pair DualList<LIST1, TABLE2>::get(const StringRef &key) const{
	const Pair &pair = _memlist.get(key);

	if (pair)
		return pair;

	// lookup into immutable table
	return _table.get(key);
}

#endif
