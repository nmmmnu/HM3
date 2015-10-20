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
	Node(Pair && data) : data(std::move(data)){}

public:
	static void *operator new(size_t size, uint8_t const height, bool const nothrow = false) {
		size += (height - 1) * sizeof(Node *);

		if (nothrow)
			return ::operator new(size, std::nothrow);

		return ::operator new(size);
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
		_height		(std::move(other._height	)),
		_heads		(std::move(other._heads		)),
		_loc		(std::move(other._loc		)),
		_dataCount	(std::move(other._dataCount	)),
		_dataSize	(std::move(other._dataSize	)){
	other._heads = nullptr;
	other._loc = nullptr;

	other._clear();
}
/*
SkipList &SkipList::operator = (SkipList &&other){
	// code without swap() is way too complicated.

	SkipList tmp = std::move(other);

	swap(tmp);

	return *this;
}

void SkipList::swap(SkipList &other) noexcept{
	using std::swap;

	swap(_height	, other._height		);
	swap(_heads	, other._heads		);
	swap(_loc	, other._loc		);
	swap(_dataCount	, other._dataCount	);
	swap(_dataSize	, other._dataSize	);
}
*/
SkipList::~SkipList(){
	removeAll();

	delete[] _heads;
	delete[] _loc;
}

bool SkipList::removeAll(){
	// _heads may be nullptr, when move constructor is on the way...
	if (_heads == nullptr)
		return true;

	for(const Node *node = _heads[0]; node; ){
		const Node *copy = node;

		node = node->next[0];

		delete copy;
	}

	_clear();

	return true;
}

template <class UPAIR>
bool SkipList::_putT(UPAIR&& newdata){
	const StringRef &key = newdata.getKey();

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
		olddata = std::forward<UPAIR>(newdata);

		return true;
	}

	// create new node

	uint8_t height = _getRandomHeight();

	Node *newnode = new(height, true) Node(std::forward<UPAIR>(newdata));

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
	++_dataCount;

	return true;
}

template bool SkipList::_putT(Pair &&newdata);
template bool SkipList::_putT(const Pair &newdata);

Pair SkipList::get(const StringRef &key) const{
	const Node *node = _locate(key);

	return node ? node->data : nullptr;
}

bool SkipList::remove(const StringRef &key){
	const Node *node = _locate(key, true);

	if (node == nullptr)
		return true;

	for(uint8_t i = 0; i < _height; ++i){
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

// ==============================

void SkipList::printLanes() const{
	uint8_t i;
	for(i = _height; i > 0; --i){
		printf("Lane # %5u :\n", i - 1);
		printLane(i - 1);
	}
}

void SkipList::printLane(uint8_t const lane) const{
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

	if (_heads)
		memset(_heads, 0, _height * sizeof(Node *) );

	// no need to clean _loc
	//if (_loc)
	//	memset(_loc, 0, _height * sizeof(Node *) );
}

const SkipList::Node *SkipList::_locate(const StringRef &key, bool const complete_evaluation) const{
	// it is extremly dangerous to have key == nullptr here.
	if (key.empty()){
		std::logic_error exception("Key can not be nullptr in SkipList::_locate");
		throw exception;
	}

	// smart over-optimizations, such skip NULL lanes or
	// start from the middle of the list did not pay off.

	int cmp = 1;

	const Node *node = nullptr;
	const Node *prev = nullptr;

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


		if (complete_evaluation == false && cmp == 0)
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



Pair	SkipList::Iterator::zero;

SkipList::Iterator::Iterator(const Node *node) :
		_node(node){}

SkipList::Iterator &SkipList::Iterator::operator++(){
	if (_node)
		_node = _node->next[0];

	return *this;
}

const Pair &SkipList::Iterator::operator*() const{
	return _node ? _node->data : zero;
}

bool SkipList::Iterator::operator==(const Iterator &other) const{
	return _node == other._node;
}

// ==============================

SkipList::Iterator SkipList::begin() const{
	return Iterator(_heads[0]);
}

SkipList::Iterator SkipList::end() const{
	return Iterator(nullptr);
}

