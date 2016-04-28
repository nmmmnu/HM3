#ifndef _FLUSH_LIST_H
#define _FLUSH_LIST_H

#include "ilist.h"


namespace hm3{


template <class LIST, class FLUSH>
class FlushList : public IMutableList<FlushList<LIST, FLUSH> >{
public:
	constexpr static size_t MAX_SIZE = 1 * 1024 * 1024;

	using Iterator		= typename LIST::Iterator;
	using count_type	= typename LIST::count_type;

public:
	template <class UFLUSH>
	FlushList(LIST &list, UFLUSH &&flusher, size_t const maxSize = MAX_SIZE) :
					list_(list),
					flusher_(std::forward<UFLUSH>(flusher)),
					maxSize_(maxSize > MAX_SIZE ? maxSize : MAX_SIZE){
	}

	FlushList(FlushList &&other) = default;

	~FlushList(){
		flush();
	}

	LIST &getList(){
		return list_;
	}

public:
	bool removeAll(){
		return list_.removeAll();
	}

	const Pair &get(const StringRef &key) const{
		return list_.get(key);
	}

	bool remove(const StringRef &key){
		return list_.remove(key);
	}

	size_t getSize() const{
		return list_.getSize();
	}

	count_type getCount() const{
		return list_.getCount();
	}

public:
	// needs to be public because of CRPT
	template <class UPAIR>
	bool putT_(UPAIR &&data);

public:
	bool flush(){
		return flusher_ << list_;
	}

public:
	Iterator begin() const{
		return list_.begin();
	}

	Iterator end() const{
		return list_.end();
	}

private:
	LIST		&list_;
	FLUSH		flusher_;
	size_t		maxSize_;
};



// ===================================



template <class LIST, class FLUSH>
template <class UPAIR>
bool FlushList<LIST, FLUSH>::putT_(UPAIR &&data){
	bool const result = list_.put( std::forward<UPAIR>(data) );

	if (list_.getSize() > maxSize_)
		flush();

	return result;
}


} // namespace


#endif