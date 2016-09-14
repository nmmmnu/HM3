#ifndef _MY_LIST_H
#define _MY_LIST_H

#include <cstdint>

#include "pair.h"


namespace hm3{


class IList{
public:
	using size_type		= size_t;
//	using difference_type	= ssize_t;
};

// ==============================

template <class T>
class List : public IList{
protected:
	constexpr
	static size_t PRINT_COUNT = 10;

public:
	void print(size_t count = PRINT_COUNT) const{
		for(const Pair &p : *self() ){
			p.print();
			if (--count == 0)
				return;
		}
	}

	bool isEmpty() const{
		return ! getCount(true);
	}

public:
	size_type getCount(bool const estimated = false) const{
		return static_cast<size_type>( self()->getCount(estimated) );
	}

private:
	const T *self() const{
		return static_cast<const T*>(this);
	}
};

// ==============================

template <class T>
class IMutableList : public List<T>{
public:
	bool put(const Pair &pair){
		return self()->putT_(pair);
	}

	bool put(Pair &&pair){
		return self()->putT_(std::move(pair));
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


} // namespace

#endif

