#ifndef _FLUSH_LIST_H
#define _FLUSH_LIST_H

#include "ilist.h"

template <class LIST, class FLUSH>
class FlushList : public IMutableList<FlushList<LIST, FLUSH> >{
public:
	constexpr static size_t MAX_SIZE = 1 * 1024 * 1024;

	using Iterator		= typename LIST::Iterator;
	using count_type	= typename LIST::count_type;

private:
	LIST		_list;
	FLUSH		_flusher;
	size_t		_maxSize;

public:
	explicit
	FlushList(LIST &&list, FLUSH &&flusher, size_t const maxSize = MAX_SIZE) :
					_list(std::move(list)),
					_flusher(std::move(flusher)),
					_maxSize(maxSize > MAX_SIZE ? maxSize : MAX_SIZE){
	}

	FlushList(FlushList &&other) = default;

	~FlushList(){
		flush();
	}

public:
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
		return _flusher(_list, true, true);
	}

public:
	Iterator begin() const{
		return _list.begin();
	}

	Iterator end() const{
		return _list.end();
	}
};

// ===================================

#include "flushlist_implementation.h"

#endif
