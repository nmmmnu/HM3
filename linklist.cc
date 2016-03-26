#include "linklist.h"

#include <memory>

namespace hm3{


struct LinkList::Node{
	Pair	data;
	Node	*next = nullptr;

public:
	template<class UPAIR>
	Node(UPAIR &&data) : data(std::forward<UPAIR>(data)){}
};


// ==============================


LinkList::LinkList(){
	clear_();
}

LinkList::LinkList(LinkList &&other):
		head_		(std::move(other.head_		)),
		dataCount_	(std::move(other.dataCount_	)),
		dataSize_	(std::move(other.dataSize_	)){
	other.clear_();
}

bool LinkList::removeAll(){
	for(Node *node = head_; node; ){
		Node *copy = node;

		node = node->next;

		delete copy;
	}

	clear_();

	return true;
}

template <class UPAIR>
bool LinkList::putT_(UPAIR&& newdata){
	const StringRef &key = newdata.getKey();

	Node *prev = nullptr;
	for(Node *node = head_; node; node = node->next){
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

			dataSize_ = dataSize_
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

	size_t const size = newdata.getSize();

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
		newnode->next = head_;
		head_ = newnode;
	}

	dataSize_ += size;
	dataCount_++;

	return true;
}

template bool LinkList::putT_(Pair &&newdata);
template bool LinkList::putT_(const Pair &newdata);

const Pair &LinkList::get(const StringRef &key) const{
	const Node *node = locate_(key);

	if (node == nullptr)
		return Pair::zero();

	const Pair & data = node->data;

	return data.cmp(key) == 0 ? data : Pair::zero();
}

bool LinkList::remove(const StringRef &key){
	Node *prev = nullptr;
	for(Node *node = head_; node; node = node->next){
		const Pair & data = node->data;
		const int cmp = data.cmp(key);

		if (cmp == 0){
			if (prev){
				prev->next = node->next;
			}else{
				// First node...
				head_ = node->next;
			}

			dataSize_ -= data.getSize();
			dataCount_--;

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

void LinkList::clear_(){
	dataCount_ = 0;
	dataSize_ = 0;
	head_ = nullptr;
}

LinkList::Node *LinkList::locate_(const StringRef &key) const{
	for(Node *node = head_; node; node = node->next){
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



LinkList::Iterator::Iterator(const Node *node) :
		node_(node){}

LinkList::Iterator &LinkList::Iterator::operator++(){
	if (node_)
		node_ = node_->next;

	return *this;
}

const Pair &LinkList::Iterator::operator*() const{
	return node_ ? node_->data : Pair::zero();
}

bool LinkList::Iterator::operator==(const Iterator &other) const{
	return node_ == other.node_;
}

// ==============================

LinkList::Iterator LinkList::begin() const{
	return Iterator(head_);
}

LinkList::Iterator LinkList::end() const{
	return Iterator(nullptr);
}


} // namespace


