#include "linklist.h"

#define xmalloc(s)		malloc(s)
#define xfree(a)		free(a)

struct LinkListNode{
	LinkListNode	*next;	// system dependent
	Pair		*data;	// system dependent
};

LinkList::LinkList(){
	_clear();
}

LinkList::~LinkList(){
	removeAll();
}

void LinkList::removeAll(){
	LinkListNode *copy;
	LinkListNode *node;
	for(node = _head; node; ){
		copy = node;

		node = node->next;

		xfree(copy->data);
		xfree(copy);
	}

	_clear();
}

bool LinkList::put(Pair *newdata){
	_resetIterator();

	if (newdata == NULL)
		return false;

	LinkListNode *prev = NULL;
	LinkListNode *node;
	for(node = _head; node; node = node->next){
		const Pair *olddata = node->data;

		const int cmp = olddata->cmp(newdata);

		if (cmp == 0){
			// handle overwrite,
			// keep the node, overwrite the data

			// check if the data in database is valid
			if (! newdata->valid(olddata)){
				// prevent memory leak
				xfree(newdata);

				return false;
			}

			_dataSize = _dataSize
					- olddata->getSize()
					+ newdata->getSize();

			xfree(node->data);
			node->data = newdata;

			return true;
		}

		if (cmp > 0)
			break;

		prev = node;
	}

	LinkListNode *newnode = (LinkListNode *)xmalloc(sizeof(LinkListNode));
	if (newnode == NULL){
		// prevent memory leak
		xfree(newdata);

		return false;
	}

	newnode->data = newdata;

	// put new pair here...
	if (prev){
		// we are at the middle
		newnode->next = prev->next;
		prev->next = newnode;
	}else{
		newnode->next = _head;
		_head = newnode;
	}

	_dataSize += newdata->getSize();
	_dataCount++;

	return true;
}

const Pair *LinkList::get(const char *key) const{
	if (key == NULL)
		return NULL;

	const LinkListNode *node = _locate(key);

	if (node == NULL)
		return NULL;

	return node->data;
}

bool LinkList::remove(const char *key){
	_resetIterator();

	if (key == NULL)
		return false;

	LinkListNode *prev = NULL;
	LinkListNode *node;
	for(node = _head; node; node = node->next){
		const Pair *data = node->data;
		const int cmp = data->cmp(key);

		if (cmp == 0){
			if (prev){
				prev->next = node->next;
			}else{
				// First node...
				_head = node->next;
			}

			_dataSize -= data->getSize();
			_dataCount--;

			xfree(node->data);
			xfree(node);

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

IIterator *LinkList::getIterator(){
	return this;
}

// ==============================

const Pair *LinkList::first(const char *key){
	if (key){
		_itHead = _locate(key);
	}else
		_itHead = _head;

	return next();
}

const Pair *LinkList::next(){
	if (_itHead == NULL)
		return NULL;

	LinkListNode *node = _itHead;
	_itHead = _itHead->next;

	return node->data;
}

// ==============================

void LinkList::_clear(){
	_dataCount = 0;
	_dataSize = 0;
	_head = NULL;

	_resetIterator();
}

LinkListNode *LinkList::_locate(const char *key) const{
	LinkListNode *node;
	for(node = _head; node; node = node->next){
		const Pair *data = node->data;

		const int cmp = data->cmp(key);

		if (cmp == 0)
			return node;

		if (cmp > 0)
			break;
	}

	return NULL;
};

void LinkList::_resetIterator(){
	_itHead = NULL;
}

