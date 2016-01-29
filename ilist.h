#ifndef _MY_LIST_H
#define _MY_LIST_H

#include <cstdint>

#include "pair.h"

// ==============================

namespace ListDefs{
	using count_type = size_t;
};

// ==============================

template <class T>
class IList{
protected:
	constexpr
	static size_t PRINT_COUNT = 10;

public:
	using count_type = ListDefs::count_type;

public:
	void print(size_t count = PRINT_COUNT) const{
		for(const Pair &p : *self() ){
			p.print();
			if (--count == 0)
				return;
		}
	}

	bool isEmpty() const{
		return getCount(true) == 0;
	}

public:
	count_type getCount(bool const estimated = false) const{
		return self()->getCount(estimated);
	}

private:
	const T *self() const{
		return static_cast<const T*>(this);
	}
};

// ==============================

template <class T>
class IMutableList : public IList<T>{
public:
	bool put(const Pair &pair){
		return self()->_putT(pair);
	}

	bool put(Pair &&pair){
		return self()->_putT(std::move(pair));
	}

	template <class ...ARGS>
	bool emplace(ARGS ...args){
		Pair p{ std::forward<ARGS>(args)... };
		return put(std::move(p));
	}

private:
	T *self(){
		return static_cast<T*>(this);
	}
};

#endif

