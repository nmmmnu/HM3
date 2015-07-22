#include "dirtable.h"

#include "myglob.h"

#include <vector>

bool DirTable::open(const char *path){
	close();

	MyGlob gl;
	gl.open(path);

	for(const char *filename = gl.first(); filename; filename = gl.next()){
		DiskTable dt;
		dt.open(filename);

		_files.push_front(std::move(dt));
	}

	return true;
}

void DirTable::close(){
	incVersion();

	_files.clear();
}

Pair DirTable::_get(const char *key) const{
	for(size_t i = 0; i < _files.size(); ++i){
		Pair pair = _files[i].get(key);

		if (pair)
			return pair;
	}

	return nullptr;
}



// ==============================



class DirTableIterator : public IIterator{
public:
	DirTableIterator(const DirTable & list);

private:
	virtual void _rewind(const char *key = nullptr) override;
	virtual Pair _next() override;
	virtual uint64_t _getVersion() override{
		return _list.getVersion();
	};

private:
	const DirTable &_list;
	std::vector<std::unique_ptr<IIterator>> _iterators;
};

DirTableIterator::DirTableIterator(const DirTable & list) : _list(list){
	for(size_t i = 0; i < list._files.size(); ++i){
		auto it = list._files[i].getIterator();
		_iterators.push_back(std::move(it));
	}
}

void DirTableIterator::_rewind(const char *key){
	for(size_t i = 0; i < _iterators.size(); ++i ){
		_iterators[i]->first(key);
	}
}

Pair DirTableIterator::_next(){
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

std::unique_ptr<IIterator> DirTable::_getIterator() const{
	return std::unique_ptr<IIterator>( new DirTableIterator(*this) );
}
