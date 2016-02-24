#ifndef _DUAL_LIST_H
#define _DUAL_LIST_H

#include "ilist.h"

#include <utility>	// std::forward

template <class MEMLIST, class TABLE>
class DualList : public IMutableList<DualList<MEMLIST,TABLE> >{
public:
//	using count_type = typename DualList::count_type;
	using Iterator = typename MEMLIST::Iterator;

public:
	constexpr static size_t MAX_SIZE = 128 * 1024 * 1024;

private:
	MEMLIST	_memlist;
	TABLE	_table;
	size_t	_maxSize;

public:
	template<class UMEMLIST, class UTABLE>
	explicit
	DualList(UMEMLIST &&memlist, UTABLE &&table, size_t const maxSize = MAX_SIZE) :
					_memlist(std::forward<UMEMLIST>(memlist)),
					_table(std::forward<UTABLE>(table)),
					_maxSize(maxSize){
	}

	DualList(DualList &&other) = default;

	~DualList(){
		flush();
	}

public:
	MEMLIST &getMemList(){
		return _memlist;
	}

	TABLE &getTable(){
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
