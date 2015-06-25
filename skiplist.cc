#include "skiplist.h"

#include <stddef.h>	// offsetof

#include "defs.h"

/*
We do ***NOT*** store next[] array size,
***NOR*** we store NULL after last next node.

It turn out it does not need, because NULL lanes are already NULL.

Disadvantage is once allocated, no one knows the size,
except probably with malloc_usable_size();

Uncommend DEBUG_PRINT_LANES for visualisation.

#define DEBUG_PRINT_LANES
*/

struct SkipListNode{
	Pair		*data;		// system dependent
	SkipListNode	*next[1];	// system dependent, dynamic, at least 1
};

SkipList::SkipList(uint8_t height){
	if (height == 0 || height > MAX_HEIGHT)
		height = DEFAULT_HEIGHT;

	_height = height;
	_heads = new SkipListNode*[height * 2];
	_loc   = & _heads[height]; // C, but much faster

	_clear();
}

SkipList::~SkipList(){
	removeAll();

	delete[] _heads;
	//delete[] _loc;
}

void SkipList::removeAll(){
	SkipListNode *copy;
	SkipListNode *node;
	for(node = _heads[0]; node; ){
		copy = node;

		node = node->next[0];

		Pair::destroy(copy->data);
		xfree(copy);
	}

	_clear();
}

bool SkipList::put(Pair *newdata){
	rewind();

	const char *key = newdata->getKey();

	SkipListNode *node = (SkipListNode *) _locate(key);

	if (node){
		// update in place. node MUST be not NULL...

		Pair *olddata = node->data;

		// check if the data in database is valid
		if (! newdata->valid(olddata) ){
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

	SkipListNode *newnode = (SkipListNode *) xmalloc(
			offsetof(SkipListNode, next) +
			height * sizeof(SkipListNode *));

	if (newnode == NULL){
		// prevent memory leak
		Pair::destroy(newdata);
		return false;
	}

	/* SEE REMARK ABOUT NEXT[] SIZE AT THE TOP */
	// newnode->height = height

	newnode->data = newdata;

	// place new node where it belongs

	uint8_t i;
	for(i = 0; i < height; ++i){
		if (_loc[i]){
			// we are at the middle
			SkipListNode *node = _loc[i];

			newnode->next[i] = node->next[i];
			node->next[i] = newnode;
		}else{
			newnode->next[i] = _heads[i];
			_heads[i] = newnode;
		}
	}

#ifdef DEBUG_PRINT_LANES
	printf("%3u Lanes-> ", height);
	for(i = 0; i < height; ++i)
		printf("%p ", newnode->next[i]);
	printf("\n");
#endif

	/* SEE REMARK ABOUT NEXT[] SIZE AT THE TOP */
	// newnode->next[i] = NULL;

	_dataSize += newdata->getSize();
	_dataCount++;

	return true;
}

std_optional<const Pair> SkipList::get(const char *key) const{
	const SkipListNode *node = _locate(key);

	return node ? node->data : nullptr;
}

bool SkipList::remove(const char *key){
	rewind();

	const SkipListNode *node = _locate(key, true);

	if (node == NULL)
		return true;

	uint8_t i;
	for(i = 0; i < _height; ++i){
		SkipListNode *prev = (SkipListNode *) _loc[i];

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

	_dataSize -= node->data->getSize();
	_dataCount--;

	Pair::destroy(node->data);
	xfree((void *) node);

	return true;
}

uint64_t SkipList::getCount() const{
	return _dataCount;
}

size_t SkipList::getSize() const{
	return _dataSize;
}

// ==============================

bool SkipList::rewind(){
	_itHead = _heads[0];
	return true;
}

bool SkipList::rewind(const char *key){
	if (!key)
		return rewind();

	_itHead = _locate(key);
	return _itHead;
}


std_optional<const Pair> SkipList::next(){
	if (_itHead == NULL)
		return NULL;

	const SkipListNode *node = _itHead;
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
	const SkipListNode *node;
	for(node = _heads[lane]; node; node = node->next[lane]){
		node->data->print();

		if (i++ > 10)
			break;
	}
}

// ==============================

void SkipList::_clear(){
	_dataSize = 0;
	_dataCount = 0;

	memset(_heads, 0, _height * sizeof(SkipListNode *) );

	// no need to clean _loc
	//memset(_loc, 0, _height * sizeof(SkipListNode *) );

	rewind();
}

const SkipListNode *SkipList::_locate(const char *key, bool complete_evaluation) const{
	// it is extremly dangerous to have key == NULL here.
	if (key == NULL){
		// probably should be throw exception here.
		my_error("FATAL ERROR", __FILE__, __LINE__);
	}

	// smart over-optimizations, such skip NULL lanes or
	// start from the middle of the list did not pay off.

	int cmp = 1;

	const SkipListNode *node = NULL;
	const SkipListNode *prev = NULL;

	uint8_t height = _height;
	while(height){
		node = prev ? prev : _heads[height - 1];

		while(node){
			cmp = node->data->cmp(key);

			if (cmp >= 0)
				break;

			prev = node;
			node = node->next[height - 1];
		}


		if (cmp == 0 && complete_evaluation == false)
			return node;

		_loc[height - 1] = (SkipListNode *) prev;

		height--;
	}

	return cmp ? NULL : node;
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

