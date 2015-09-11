#include "multitable.h"

#include <vector>

Pair MultiTable::_get(const std::string &key) const{
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
	virtual void _rewind(const std::string &key) override;
	virtual Pair _next() override;
	virtual uint64_t _getVersion() override{
		return _list.getVersion();
	};

private:
	const MultiTable &_list;
	std::vector<std::unique_ptr<IIterator>> _iterators;
};

MultiTableIterator::MultiTableIterator(const MultiTable & list) : _list(list){
	for(size_t i = 0; i < list._collection.getCount(); ++i){
		auto it = list._collection[i].getIterator();
		_iterators.push_back(std::move(it));
	}
}

void MultiTableIterator::_rewind(const std::string &key){
	for(size_t i = 0; i < _iterators.size(); ++i ){
		_iterators[i]->first(key);
	}
}

Pair MultiTableIterator::_next(){
	Pair r_pair = nullptr;

	// step 1: find minimal in reverse order to find most recent.
	for(size_t i = 0; i < _iterators.size(); ++i ){
		Pair pair = _iterators[i]->current();

		// skip if is null
		if (! pair)
			continue;

		// initialize
		if (! r_pair){
			r_pair = pair;
			continue;
		}

		// compare and swap pair
		if (pair.cmp(r_pair) < 0){
			r_pair = pair;
		}
	}

	if (!r_pair){
		return nullptr;
	}

	// step 2: increase all duplicates
	for(size_t i = 0; i < _iterators.size(); ++i ){
		Pair pair = _iterators[i]->current();

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
