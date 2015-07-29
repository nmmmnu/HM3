#include "skiplist.h"

#include <stdexcept>

std::mt19937 SkipList::__rand{ (uint32_t) time(nullptr) };

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
	Pair		data;
	SkipList::Node	*next[1];	// system dependent, dynamic, at least 1

public:
	Node(const Pair & data) : data(data){}

public:
	static void *operator new(size_t size, uint8_t height, const std::nothrow_t& tag) {
		size += (height - 1) * sizeof(Node *);
		return ::operator new(size, tag);
	}
};

SkipList::SkipList(uint8_t height){
	if (height == 0 || height > MAX_HEIGHT)
		height = DEFAULT_HEIGHT;

	_height = height;
	_heads = new Node*[height];
	_loc   = new Node*[height];

	_clear();
}

SkipList::SkipList(SkipList &&other):
		_height		(other._height		),
		_heads		(other._heads		),
		_loc		(other._loc		),
		_dataCount	(other._dataCount	),
		_dataSize	(other._dataSize	){
	other._heads = nullptr;
	other._loc = nullptr;
}

SkipList::~SkipList(){
	// _heads may be nullptr, when move constructor is on the way...
	if (_heads != nullptr)
		removeAll();

	delete[] _heads;
	delete[] _loc;
}

void SkipList::_removeAll(){
	for(const Node *node = _heads[0]; node; ){
		const Node *copy = node;

		node = node->next[0];

		delete copy;
	}

	_clear();
}

bool SkipList::_put(const Pair &newdata){
	const char *key = newdata.getKey();

	Node *node = (Node *) _locate(key);

	if (node){
		// update in place. node MUST be not NULL...

		Pair & olddata = node->data;

		// check if the data in database is valid
		if (! newdata.valid(olddata) ){
			// newdata will be magically destroyed.
			return false;
		}

		_dataSize = _dataSize
			- olddata.getSize()
			+ newdata.getSize();

		// copy assignment
		olddata = newdata;

		return true;
	}

	// create new node

	uint8_t height = _getRandomHeight();

	Node *newnode = new(height, std::nothrow) Node(newdata);

	if (newnode == nullptr){
		// newdata will be magically destroyed.
		return false;
	}

	/* SEE REMARK ABOUT NEXT[] SIZE AT THE TOP */
	// newnode->height = height

	// place new node where it belongs

	for(uint8_t i = 0; i < height; ++i){
		if (_loc[i]){
			// we are at the middle
			Node *node = _loc[i];

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

	_dataSize += newdata.getSize();
	_dataCount++;

	return true;
}

Pair SkipList::_get(const char *key) const{
	const Node *node = _locate(key);

	return node ? node->data : nullptr;
}

bool SkipList::_remove(const char *key){
	const Node *node = _locate(key, true);

	if (node == nullptr)
		return true;

	uint8_t i;
	for(i = 0; i < _height; ++i){
		Node *prev = (Node *) _loc[i];

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

	const Pair & data = node->data;

	_dataSize -= data.getSize();
	_dataCount--;

	delete node;

	return true;
}

uint64_t SkipList::_getCount() const{
	return _dataCount;
}

size_t SkipList::_getSize() const{
	return _dataSize;
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
	const Node *node;
	for(node = _heads[lane]; node; node = node->next[lane]){
		const Pair & data = node->data;
		data.print();

		if (++i > 10)
			break;
	}
}

// ==============================

void SkipList::_clear(){
	_dataSize = 0;
	_dataCount = 0;

	memset(_heads, 0, _height * sizeof(Node *) );

	// no need to clean _loc
	//memset(_loc, 0, _height * sizeof(Node *) );
}

const SkipList::Node *SkipList::_locate(const char *key, bool complete_evaluation) const{
	// it is extremly dangerous to have key == nullptr here.
	if (key == nullptr){
		std::logic_error exception("Key can not be nullptr in SkipList::_locate");
		throw exception;
	}

	// smart over-optimizations, such skip NULL lanes or
	// start from the middle of the list did not pay off.

	int cmp = 1;

	const Node *node = NULL;
	const Node *prev = NULL;

	uint8_t height = _height;
	while(height){
		node = prev ? prev : _heads[height - 1];

		while(node){
			const Pair & data = node->data;
			cmp = data.cmp(key);

			if (cmp >= 0)
				break;

			prev = node;
			node = node->next[height - 1];
		}


		if (cmp == 0 && complete_evaluation == false)
			return node;

		_loc[height - 1] = (Node *) prev;

		--height;
	}

	return cmp ? nullptr : node;
};

uint8_t SkipList::_getRandomHeight(){
	// This gives slightly better performance,
	// than divide by 3 or multply by 0.33
	auto part = __rand.max() >> 1;

	// We execute rand() inside the loop,
	// but performance is fast enought.

	uint8_t h = 1;
	while(h < _height && __rand() > part)
		h++;

	return h;
}



// ==============================



class SkipListIterator : public IIterator{
public:
	SkipListIterator(const SkipList & list) :
			_list(list),
			_current(list._heads[0]){}

private:
	virtual void _rewind(const char *key = nullptr) override;
	virtual Pair _next() override;
	virtual uint64_t _getVersion() override{
		return _list.getVersion();
	};

private:
	const SkipList		& _list;
	const SkipList::Node	*_current;
};

void SkipListIterator::_rewind(const char *key){
	if (!key){
		_current = _list._heads[0];
		return;
	}

	auto node = _list._locate(key, true);

	if (node){
		// found
		_current = node;
		return;
	}

	// fuzzy found, but we are on previous element.
	_current = _list._loc[0] ? _list._loc[0]->next[0] : nullptr;
}

Pair SkipListIterator::_next(){
	if (_current == nullptr)
		return nullptr;

	const Pair & pair = _current->data;

	_current = _current->next[0];

	return pair;
}

std::unique_ptr<IIterator> SkipList::_getIterator() const{
	return std::unique_ptr<IIterator>( new SkipListIterator(*this) );
};
