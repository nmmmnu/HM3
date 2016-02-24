#ifndef _DUAL_LIST_H
#define _DUAL_LIST_H

#include "ilist.h"

#include <utility>	// std::forward

template <class LIST1, class TABLE2>
class DualList : public IMutableList<DualList<LIST1,TABLE2> >{
public:
	constexpr static size_t MAX_SIZE = 128 * 1024 * 1024;
	
	using Iterator = typename LIST1::Iterator;

private:
	LIST1	_memlist;
	TABLE2	_table;
	size_t	_maxSize;

public:
	template<class ULIST1, class UTABLE2>
	explicit
	DualList(ULIST1 &&memlist, UTABLE2 &&table, size_t const maxSize = MAX_SIZE) :
					_memlist(std::forward<ULIST1 >(memlist)),
					_table  (std::forward<UTABLE2>(table)),
					_maxSize(maxSize){
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
	bool _putT(UPAIR &&data);

public:
	bool flush();

public:
	Iterator begin() const{
		return _memlist.begin();
	}

	Iterator end() const{
		return _memlist.end();
	}
};

// ===================================

#include "duallist_implementation.h"

#endif
