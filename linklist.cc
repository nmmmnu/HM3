#include "linklist.h"

#include "defs.h"

struct LinkListNode{
	LinkListNode	*next;	// system dependent
	const Pair	*data;	// system dependent
};

LinkList::LinkList(){
	_clear();
}

LinkList::~LinkList(){
	removeAll();
}

void LinkList::removeAll(){
	for(LinkListNode *node = _head; node; ){
		LinkListNode *copy = node;

		node = node->next;

		const Pair *data = copy->data;
		Pair::destroy(data);
		xfree(copy);
	}

	_clear();
}

bool LinkList::put(const Pair *newdata){
	rewind();

	const char *key = newdata->getKey();

	LinkListNode *prev = NULL;
	for(LinkListNode *node = _head; node; node = node->next){
		const Pair *olddata = node->data;

		const int cmp = olddata->cmp(key);

		if (cmp == 0){
			// handle overwrite,
			// keep the node, overwrite the data

			// check if the data in database is valid
			if (! newdata->valid2(olddata)){
				// prevent memory leak
				Pair::destroy(newdata);

				return false;
			}

			_dataSize = _dataSize
					- olddata->getSize()
					+ newdata->getSize();

			Pair::destroy(olddata);
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
		Pair::destroy(newdata);

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
	const LinkListNode *node = _locate(key);

	return node ? node->data : nullptr;
}

bool LinkList::remove(const char *key){
	rewind();

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

			Pair::destroy(data);
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

// ==============================

bool LinkList::rewind(){
	_itHead = _head;
	return true;
}

bool LinkList::rewind(const char *key){
	if (!key)
		return rewind();

	_itHead = _locate(key);
	return _itHead;
}

const Pair *LinkList::next(){
	if (_itHead == NULL)
		return NULL;

	const LinkListNode *node = _itHead;
	_itHead = _itHead->next;

	return node->data;
}

// ==============================

void LinkList::_clear(){
	_dataCount = 0;
	_dataSize = 0;
	_head = nullptr;

	rewind();
}

LinkListNode *LinkList::_locate(const char *key) const{
	for(LinkListNode *node = _head; node; node = node->next){
		const Pair *data = node->data;

		const int cmp = data->cmp(key);

		if (cmp == 0)
			return node;

		if (cmp > 0)
			break;
	}

	return nullptr;
};


