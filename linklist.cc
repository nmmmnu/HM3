#include "linklist.h"

#include "defs.h"

#include <memory>

struct LinkList::Node{
	const void	*data;	// system dependent
	LinkList::Node	*next;	// system dependent
};

LinkList::LinkList(){
	_clear();
}

LinkList::LinkList(LinkList &&other):
		_head		(other._head		),
		_dataCount	(other._dataCount	),
		_dataSize	(other._dataSize	),
		_itHead		(other._itHead          ){
	other._clear();
}

LinkList::~LinkList(){
	// _head may be nullptr, when move constructor is on the way...
	if (_head != nullptr)
		removeAll();
}

void LinkList::removeAll(){
	for(Node *node = _head; node; ){
		Node *copy = node;

		node = node->next;

		Pair data = { copy->data, true };
		// data will be magically destroyed.
		
		delete copy;
	}

	_clear();
}

bool LinkList::put(const Pair &newdata){
	invalidate();

	const char *key = newdata.getKey();

	Node *prev = NULL;
	for(Node *node = _head; node; node = node->next){
		Pair olddata = node->data;

		const int cmp = olddata.cmp(key);

		if (cmp == 0){
			// handle overwrite,
			// keep the node, overwrite the data

			// check if the data in database is valid
			if (! newdata.valid(olddata)){
				// newdata will be magically destroyed.
				return false;
			}

			_dataSize = _dataSize
					- olddata.getSize()
					+ newdata.getSize();

			olddata.getBlobOwnership();
			// olddata will be magically destroyed.
			
			node->data = newdata.cloneBlob();

			return true;
		}

		if (cmp > 0)
			break;

		prev = node;
	}

	Node *newnode = new(std::nothrow) Node;
	if (newnode == NULL){
		// newdata will be magically destroyed.
		return false;
	}

	newnode->data = newdata.cloneBlob();

	// put new pair here...
	if (prev){
		// we are at the middle
		newnode->next = prev->next;
		prev->next = newnode;
	}else{
		newnode->next = _head;
		_head = newnode;
	}

	_dataSize += newdata.getSize();
	_dataCount++;

	return true;
}

Pair LinkList::get(const char *key) const{
	const Node *node = _locate(key);

	if (node == nullptr)
		return nullptr;

	const Pair data = node->data;

	return data.cmp(key) == 0 ? data : nullptr;
}

bool LinkList::remove(const char *key){
	invalidate();

	Node *prev = NULL;
	Node *node;
	for(node = _head; node; node = node->next){
		Pair data = node->data;
		const int cmp = data.cmp(key);

		if (cmp == 0){
			if (prev){
				prev->next = node->next;
			}else{
				// First node...
				_head = node->next;
			}

			_dataSize -= data.getSize();
			_dataCount--;

			data.getBlobOwnership();
			// data will be magically destroyed.
			
			delete node;

			return true;
		}

		if (cmp > 0)
			break;

		prev = node;
	}

	return true;
}

uint64_t LinkList::getCount() const{
	return _dataCount;
}

size_t LinkList::getSize() const{
	return _dataSize;
}

// ==============================

void LinkList::_rewind(const char *key){
	if (!key){
		_itHead = _head;
		return;
	}

	_itHead = _locate(key);
}

const void *LinkList::_next(){
	if (_itHead == nullptr)
		return nullptr;

	auto node = _itHead;

	_itHead = _itHead->next;

	return node->data;
}

// ==============================

void LinkList::_clear(){
	_dataCount = 0;
	_dataSize = 0;
	_head = nullptr;

	invalidate();
}

LinkList::Node *LinkList::_locate(const char *key) const{
	for(Node *node = _head; node; node = node->next){
		const Pair data = node->data;

		const int cmp = data.cmp(key);

		if (cmp == 0)
			return node;

		if (cmp > 0)
			return node;
			//break;
	}

	return nullptr;
};


