#include "linklist.h"

#include <memory>

struct LinkList::Node{
	Pair		data;
	LinkList::Node	*next = nullptr;

public:
	Node(const Pair & data) : data(data){}
	Node(Pair && data) : data(std::move(data)){}
};

LinkList::LinkList(){
	_clear();
}

LinkList::LinkList(LinkList &&other):
		_head		(std::move(other._head		)),
		_dataCount	(std::move(other._dataCount	)),
		_dataSize	(std::move(other._dataSize	)){
	other._clear();
}

LinkList::~LinkList(){
	removeAll();
}

bool LinkList::removeAll(){
	for(Node *node = _head; node; ){
		Node *copy = node;

		node = node->next;

		delete copy;
	}

	_clear();

	return true;
}

template <typename UPAIR>
bool LinkList::_putT(UPAIR&& newdata){
	const StringRef &key = newdata.getKey();

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
			olddata = std::forward<UPAIR>(newdata);

			return true;
		}

		if (cmp > 0)
			break;

		prev = node;
	}

	Node *newnode = new(std::nothrow) Node(std::forward<UPAIR>(newdata));
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

template bool LinkList::_putT(Pair &&newdata);
template bool LinkList::_putT(const Pair &newdata);

Pair LinkList::get(const StringRef &key) const{
	const Node *node = _locate(key);

	if (node == nullptr)
		return nullptr;

	const Pair & data = node->data;

	return data.cmp(key) == 0 ? data : nullptr;
}

bool LinkList::remove(const StringRef &key){
	Node *prev = nullptr;
	for(Node *node = _head; node; node = node->next){
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

// ==============================

void LinkList::_clear(){
	_dataCount = 0;
	_dataSize = 0;
	_head = nullptr;
}

LinkList::Node *LinkList::_locate(const StringRef &key) const{
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



LinkList::Iterator::Iterator(const LinkList &list, const Node *node) :
		_list(list),
		_node(node){}

LinkList::Iterator &LinkList::Iterator::operator++(){
	if (_node)
		_node = _node->next;

	return *this;
}

const Pair &LinkList::Iterator::operator*() const{
	static Pair zero;

	if (_node)
		return _node->data;

	return zero;
}

bool LinkList::Iterator::operator==(const Iterator &other) const{
	return &_list == &other._list && _node == other._node;
}

bool LinkList::Iterator::operator!=(const Iterator &other) const{
	return ! ( *this == other );
}

// ==============================

LinkList::Iterator LinkList::begin() const{
	return Iterator(*this, _head);
}

LinkList::Iterator LinkList::end() const{
	return Iterator(*this, nullptr);
}


