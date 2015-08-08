#include "linklist.h"

#include <memory>

struct LinkList::Node{
	Pair		data;
	LinkList::Node	*next = nullptr;

public:
	Node(const Pair & data) : data(data){}
};

LinkList::LinkList(){
	_clear();
}

LinkList::LinkList(LinkList &&other):
		_head		(other._head		),
		_dataCount	(other._dataCount	),
		_dataSize	(other._dataSize	){
	other._clear();
}

LinkList::~LinkList(){
	// _head may be nullptr, when move constructor is on the way...
	if (_head != nullptr)
		removeAll();
}

void LinkList::_removeAll(){
	for(Node *node = _head; node; ){
		Node *copy = node;

		node = node->next;

		delete copy;
	}

	_clear();
}

bool LinkList::_put(const Pair &newdata){
	const char *key = newdata.getKey();

	Node *prev = nullptr;
	for(Node *node = _head; node; node = node->next){
		Pair & olddata = node->data;

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

			// copy assignment
			olddata = newdata;

			return true;
		}

		if (cmp > 0)
			break;

		prev = node;
	}

	Node *newnode = new(std::nothrow) Node(newdata);
	if (newnode == nullptr){
		// newdata will be magically destroyed.
		return false;
	}

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

Pair LinkList::_get(const char *key) const{
	const Node *node = _locate(key);

	if (node == nullptr)
		return nullptr;

	const Pair & data = node->data;

	return data.cmp(key) == 0 ? data : nullptr;
}

bool LinkList::_remove(const char *key){
	Node *prev = nullptr;
	Node *node;
	for(node = _head; node; node = node->next){
		const Pair & data = node->data;
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

			delete node;

			return true;
		}

		if (cmp > 0)
			break;

		prev = node;
	}

	return true;
}

uint64_t LinkList::_getCount() const{
	return _dataCount;
}

size_t LinkList::_getSize() const{
	return _dataSize;
}

// ==============================

void LinkList::_clear(){
	_dataCount = 0;
	_dataSize = 0;
	_head = nullptr;
}

LinkList::Node *LinkList::_locate(const char *key) const{
	for(Node *node = _head; node; node = node->next){
		const Pair & data = node->data;

		const int cmp = data.cmp(key);

		if (cmp == 0)
			return node;

		if (cmp > 0)
			return node;
			//break;
	}

	return nullptr;
};



// ==============================



class LinkListIterator : public IIterator{
public:
	LinkListIterator(const LinkList & list) :
			_list(list),
			_current(list._head){}

private:
	virtual void _rewind(const char *key = nullptr) override;
	virtual Pair _next() override;
	virtual uint64_t _getVersion() override{
		return _list.getVersion();
	};

private:
	const LinkList		& _list;
	const LinkList::Node	*_current;
};


void LinkListIterator::_rewind(const char *key){
	if (!key){
		_current = _list._head;
		return;
	}

	_current = _list._locate(key);
}

Pair LinkListIterator::_next(){
	if (_current == nullptr)
		return nullptr;

	const Pair & pair = _current->data;

	_current = _current->next;

	return pair;
}

std::unique_ptr<IIterator> LinkList::_getIterator() const{
	//return std::unique_ptr<IIterator>( new LinkListIterator(*this) );
	return std::make_unique<LinkListIterator>(*this);
};
