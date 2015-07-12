#include "skiplist.h"

#include <stddef.h>	// offsetof

#include "defs.h"

#include <memory>

/*
We do ***NOT*** store next[] array size,
***NOR*** we store NULL after last next node.

It turn out it does not need, because NULL lanes are already NULL.

Disadvantage is once allocated, no one knows the size,
except probably with malloc_usable_size();

Uncommend DEBUG_PRINT_LANES for visualisation.

#define DEBUG_PRINT_LANES
*/

struct SkipList::Node{
	const Pair	*data;		// system dependent
	SkipList::Node	*next[1];	// system dependent, dynamic, at least 1
};

SkipList::SkipList(uint8_t height){
	if (height == 0 || height > MAX_HEIGHT)
		height = DEFAULT_HEIGHT;

	_height = height;
	_heads = new SkipList::Node*[height];
	_loc   = new SkipList::Node*[height];
	//_loc   = & _heads[height]; // C, but much faster

	_clear();
}

SkipList::SkipList(SkipList &&other):
		_height		(other._height		),
		_heads		(other._heads		),
		_loc		(other._loc		),
		_dataCount	(other._dataCount	),
		_dataSize	(other._dataSize	),
		_itHead		(other._itHead		){
	other._heads = nullptr;
	other._loc = nullptr;
	other._itHead = nullptr;
}

SkipList::~SkipList(){
	// _heads may be nullptr, when move constructor is on the way...
	if (_heads != nullptr)
		removeAll();

	delete[] _heads;
	delete[] _loc;
}

void SkipList::removeAll(){
	for(SkipList::Node *node = _heads[0]; node; ){
		SkipList::Node *copy = node;

		node = node->next[0];

		const Pair *data = copy->data;
		Pair::destroy(data);
		delete[] copy;
	}

	_clear();
}

bool SkipList::put(const Pair *newdata){
	invalidate();

	const char *key = newdata->getKey();

	SkipList::Node *node = (SkipList::Node *) _locate(key);

	if (node){
		// update in place. node MUST be not NULL...

		const Pair *olddata = node->data;

		// check if the data in database is valid
		if (! newdata->valid2(olddata) ){
			// prevent memory leak
			Pair::destroy(newdata);

			return false;
		}

		// leave node, update only the data
		node->data = newdata;

		_dataSize = _dataSize
			- olddata->getSize()
			+ newdata->getSize();

		// release old data
		Pair::destroy(olddata);

		return true;
	}

	// create new node

	uint8_t height = _getRandomHeight();

	SkipList::Node *newnode = (SkipList::Node *) new(std::nothrow) char[
				offsetof(SkipList::Node, next) +
				height * sizeof(SkipList::Node *)
			];

	if (newnode == nullptr){
		// prevent memory leak
		Pair::destroy(newdata);
		return false;
	}

	/* SEE REMARK ABOUT NEXT[] SIZE AT THE TOP */
	// newnode->height = height

	newnode->data = newdata;

	// place new node where it belongs

	for(uint8_t i = 0; i < height; ++i){
		if (_loc[i]){
			// we are at the middle
			SkipList::Node *node = _loc[i];

			newnode->next[i] = node->next[i];
			node->next[i] = newnode;
		}else{
			newnode->next[i] = _heads[i];
			_heads[i] = newnode;
		}
	}

#ifdef DEBUG_PRINT_LANES
	printf("%3u Lanes-> ", height);
	for(uint8_t i = 0; i < height; ++i)
		printf("%p ", newnode->next[i]);
	printf("\n");
#endif

	/* SEE REMARK ABOUT NEXT[] SIZE AT THE TOP */
	// newnode->next[i] = NULL;

	_dataSize += newdata->getSize();
	_dataCount++;

	return true;
}

const Pair *SkipList::get(const char *key) const{
	const SkipList::Node *node = _locate(key);

	return node ? node->data : nullptr;
}

const Pair *SkipList::getAfter(const char *key) const{
	const SkipList::Node *node = _locate(key, true);

	if (node == nullptr){
		// not found. use internal
		node = _loc[0];
	}else{
		// found
	}

	// go to next in both cases
	node = node ? node->next[0] : nullptr;

	return node ? node->data : nullptr;
}

bool SkipList::remove(const char *key){
	invalidate();

	const SkipList::Node *node = _locate(key, true);

	if (node == nullptr)
		return true;

	uint8_t i;
	for(i = 0; i < _height; ++i){
		SkipList::Node *prev = (SkipList::Node *) _loc[i];

		if (prev){
			// check if lane go to this node...
			if (prev->next[i] == node)
				prev->next[i] = node->next[i];
		}else{
			// must be first
			if (_heads[i] == node)
				_heads[i] = node->next[i];
		}
	}

	const Pair *data = node->data;

	_dataSize -= data->getSize();
	_dataCount--;

	Pair::destroy(data);
	delete[] node;

	return true;
}

uint64_t SkipList::getCount() const{
	return _dataCount;
}

size_t SkipList::getSize() const{
	return _dataSize;
}

// ==============================

void SkipList::_rewind(const char *key){
	if (!key){
		_itHead = _heads[0];
		return;
	}

	const SkipList::Node *node = _locate(key, true);

	if (node){
		// found
		_itHead = node;
		return;
	}

	// fuzzy found, but we are on previous element.
	_itHead = _loc[0] ? _loc[0]->next[0] : nullptr;
}

const Pair *SkipList::_next(){
	if (_itHead == nullptr)
		return nullptr;

	auto node = _itHead;

	_itHead = _itHead->next[0];

	return node->data;
}

// ==============================

void SkipList::printLanes() const{
	uint8_t i;
	for(i = _height; i > 0; --i){
		printf("Lane # %5u :\n", i - 1);
		printLane(i - 1);
	}
}

void SkipList::printLane(uint8_t lane) const{
	uint64_t i = 0;
	const SkipList::Node *node;
	for(node = _heads[lane]; node; node = node->next[lane]){
		const Pair *data = node->data;
		data->print();

		if (++i > 10)
			break;
	}
}

// ==============================

void SkipList::_clear(){
	_dataSize = 0;
	_dataCount = 0;

	memset(_heads, 0, _height * sizeof(SkipList::Node *) );

	// no need to clean _loc
	//memset(_loc, 0, _height * sizeof(SkipList::Node *) );

	invalidate();
}

const SkipList::Node *SkipList::_locate(const char *key, bool complete_evaluation) const{
	// it is extremly dangerous to have key == NULL here.
	if (key == nullptr){
		// probably should be throw exception here.
		my_error("FATAL ERROR", __FILE__, __LINE__);
	}

	// smart over-optimizations, such skip NULL lanes or
	// start from the middle of the list did not pay off.

	int cmp = 1;

	const SkipList::Node *node = NULL;
	const SkipList::Node *prev = NULL;

	uint8_t height = _height;
	while(height){
		node = prev ? prev : _heads[height - 1];

		while(node){
			const Pair *data = node->data;
			cmp = data->cmp(key);

			if (cmp >= 0)
				break;

			prev = node;
			node = node->next[height - 1];
		}


		if (cmp == 0 && complete_evaluation == false)
			return node;

		_loc[height - 1] = (SkipList::Node *) prev;

		--height;
	}

	return cmp ? nullptr : node;
};

uint8_t SkipList::_getRandomHeight(){
	// This gives slightly better performance,
	// than divide by 3 or multply by 0.33
	int part = RAND_MAX >> 1;

	// We execute rand() inside the loop,
	// but performance is fast enought.

	uint8_t h = 1;
	while(h < _height && rand() > part)
		h++;

	return h;
}

