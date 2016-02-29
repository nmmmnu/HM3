#ifndef _FLUSH_LIST_H
#define _FLUSH_LIST_H

#include "ilist.h"

template <class LIST, class FLUSH>
class FlushList : public IMutableList<FlushList<LIST, FLUSH> >{
public:
	constexpr static size_t MAX_SIZE = 1 * 1024 * 1024;

	using Iterator		= typename LIST::Iterator;
	using count_type	= typename LIST::count_type;

public:
	template <class UFLUSH>
	FlushList(LIST &list, UFLUSH &&flusher, size_t const maxSize = MAX_SIZE) :
					_list(list),
					_flusher(std::forward<UFLUSH>(flusher)),
					_maxSize(maxSize > MAX_SIZE ? maxSize : MAX_SIZE){
	}

	FlushList(FlushList &&other) = default;

	~FlushList(){
		flush();
	}

	LIST &getList(){
		return _list;
	}

public:
	bool removeAll(){
		return _list.removeAll();
	}

	const Pair &get(const StringRef &key) const{
		return _list.get(key);
	}

	bool remove(const StringRef &key){
		return _list.remove(key);
	}

	size_t getSize() const{
		return _list.getSize();
	}

	count_type getCount() const{
		return _list.getCount();
	}

public:
	// needs to be public because of CRPT
	template <class UPAIR>
	bool _putT(UPAIR &&data);

public:
	bool flush(){
		return _flusher << _list;
	}

public:
	Iterator begin() const{
		return _list.begin();
	}

	Iterator end() const{
		return _list.end();
	}

private:
	LIST		&_list;
	FLUSH		_flusher;
	size_t		_maxSize;
};



// ===================================



template <class LIST, class FLUSH>
template <class UPAIR>
bool FlushList<LIST, FLUSH>::_putT(UPAIR &&data){
	bool const result = _list.put( std::forward<UPAIR>(data) );

	if (_list.getSize() > _maxSize)
		flush();

	return result;
}

#endif
