#include "multitable.h"

#include <vector>

Pair MultiTable::_get(const StringRef &key) const{
	for(size_t i = 0; i < _collection.getCount(); ++i){
		Pair pair = _collection[i].get(key);

		if (pair)
			return pair;
	}

	return nullptr;
}

// ==============================

class MultiTableIterator : public IIterator{
public:
	MultiTableIterator(const MultiTable & list);

private:
	void _rewind(const StringRef &key) final;
	Pair _next() final;
	uint64_t _getVersion() const final;

private:
	typedef std::vector<std::unique_ptr<IIterator>>	ivector;
	typedef ivector::size_type			size_type;

	const MultiTable	&_list;
	ivector			_iterators;
};

MultiTableIterator::MultiTableIterator(const MultiTable & list) : _list(list){
	auto count = list._collection.getCount();
	_iterators.reserve(count);
	for(size_type i = 0; i < count; ++i){
		auto it = list._collection[i].getIterator();
		_iterators.push_back(std::move(it));
	}
}

void MultiTableIterator::_rewind(const StringRef &key){
	for(size_t i = 0; i < _iterators.size(); ++i ){
		_iterators[i]->first(key);
	}
}

uint64_t MultiTableIterator::_getVersion() const{
	uint64_t ver = 0;

	for(size_type i = 0; i < _iterators.size(); ++i ){
		auto ver2 = _iterators[i]->getVersion();
		if (ver < ver2)
			ver = ver2;
	}

	return ver;
}

Pair MultiTableIterator::_next(){
	Pair r_pair = nullptr;

	// step 1: find minimal in reverse order to find most recent.
	for(size_type i = 0; i < _iterators.size(); ++i ){
		Pair pair = _iterators[i]->current();

		// skip if is null
		if (! pair)
			continue;

		// initialize
		if (! r_pair){
			r_pair = std::move(pair);
			continue;
		}

		// compare and swap pair
		if (pair.cmp(r_pair) < 0){
			r_pair = std::move(pair);
		}
	}

	if (!r_pair){
		return nullptr;
	}

	// step 2: increase all duplicates
	for(size_t i = 0; i < _iterators.size(); ++i ){
		const Pair &pair = _iterators[i]->current();

		if (pair && pair.cmp(r_pair) == 0)
			_iterators[i]->next();
	}

	return r_pair;
}

// ==============================

std::unique_ptr<IIterator> MultiTable::_getIterator() const{
	//return std::unique_ptr<IIterator>( new MultiTableIterator(*this) );
	return std::make_unique<MultiTableIterator>(*this);
}
