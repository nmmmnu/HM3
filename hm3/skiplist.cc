#include "skiplist.h"

#include <stdexcept>
#include <algorithm> // fill

namespace hm3{


std::mt19937 SkipList::rand_{ (uint32_t) time(nullptr) };

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
	Pair	data;
	Node	*next[1];	// system dependent, dynamic, at least 1

public:
	template<class UPAIR>
	Node(UPAIR &&data) : data(std::forward<UPAIR>(data)){}

public:
	static void *operator new(size_t size, height_type const height, bool const nothrow = false) {
		size += (height - 1) * sizeof(Node *);

		if (nothrow)
			return ::operator new(size, std::nothrow);

		return ::operator new(size);
	}
};

SkipList::SkipList(height_type height){
	if (height == 0 || height > MAX_HEIGHT)
		height = DEFAULT_HEIGHT;

	height_ = height;

	clear_();
}

SkipList::SkipList(SkipList &&other):
		height_		(std::move(other.height_	)),
		heads_		(std::move(other.heads_		)),
		dataCount_	(std::move(other.dataCount_	)),
		dataSize_	(std::move(other.dataSize_	)){
	other.clear_();
}

SkipList::~SkipList(){
	removeAll();
}

bool SkipList::removeAll(){
	for(const Node *node = heads_[0]; node; ){
		const Node *copy = node;

		node = node->next[0];

		delete copy;
	}

	clear_();

	return true;
}

template <class UPAIR>
bool SkipList::putT_(UPAIR&& newdata){
	const StringRef &key = newdata.getKey();

	if (key.empty())
		return true;

	const auto nl = locate_(key);

	if (nl.node){
		// update in place. node MUST be not NULL...

		const Pair & olddata = nl.node->data;

		// check if the data in database is valid
		if (! newdata.valid(olddata) ){
			// newdata will be magically destroyed.
			return false;
		}

		dataSize_ = dataSize_
			- olddata.getMemSize()
			+ newdata.getMemSize();

		// copy assignment
		const_cast<Pair &>(olddata) = std::forward<UPAIR>(newdata);

		return true;
	}

	// create new node

	size_t const size = newdata.getMemSize();
	height_type const height = _getRandomHeight();

	Node *newnode = new(height, true) Node(std::forward<UPAIR>(newdata));

	if (newnode == nullptr){
		// newdata will be magically destroyed.
		return false;
	}

	/* SEE REMARK ABOUT NEXT[] SIZE AT THE TOP */
	// newnode->height = height

	// place new node where it belongs

	for(height_type i = 0; i < height; ++i){
		if (nl.loc[i]){
			// we are at the middle
			const Node *node = nl.loc[i];

			newnode->next[i] = node->next[i];
			const_cast<Node *>(node)->next[i] = newnode;
		}else{
			newnode->next[i] = heads_[i];
			heads_[i] = newnode;
		}
	}

#ifdef DEBUG_PRINT_LANES
	printf("%3u Lanes-> ", height);
	for(height_type i = 0; i < height; ++i)
		printf("%p ", newnode->next[i]);
	printf("\n");
#endif

	/* SEE REMARK ABOUT NEXT[] SIZE AT THE TOP */
	// newnode->next[i] = NULL;

	dataSize_ += size;
	++dataCount_;

	return true;
}

template bool SkipList::putT_(Pair &&newdata);
template bool SkipList::putT_(const Pair &newdata);

const Pair &SkipList::get(const StringRef &key) const{
	if (key.empty())
		return Pair::zero();

	const auto nl = locate_(key);

	return nl.node ? nl.node->data : Pair::zero();
}

SkipList::Iterator SkipList::lowerBound(const StringRef &key) const{
	if (key.empty())
		return end();

	const auto nl = locate_(key);

	if (nl.node)
		return Iterator(nl.node);

	const Node *prev = nl.loc[0];

	if (prev)
		return Iterator(prev->next[0]);
	else
		return Iterator(heads_[0]);
}

bool SkipList::remove(const StringRef &key){
	if (key.empty())
		return true;

	const auto nl = locate_(key, true);

	if (nl.node == nullptr)
		return true;

	for(height_type i = 0; i < height_; ++i){
		const Node *prev = nl.loc[i];

		if (prev){
			// check if lane go to this node...
			if (prev->next[i] == nl.node)
				const_cast<Node *>(prev)->next[i] = nl.node->next[i];
		}else{
			// must be first
			if (heads_[i] == nl.node)
				heads_[i] = nl.node->next[i];
		}
	}

	const Pair & data = nl.node->data;

	dataSize_ -= data.getMemSize();
	dataCount_--;

	delete nl.node;

	return true;
}

// ==============================

void SkipList::printLanes() const{
	for(height_type i = height_; i > 0; --i){
		printf("Lane # %5u :\n", i - 1);
		printLane((height_type) (i - 1));
	}
}

void SkipList::printLane(height_type const lane) const{
	uint64_t i = 0;
	const Node *node;
	for(node = heads_[lane]; node; node = node->next[lane]){
		const Pair & data = node->data;
		data.print();

		if (++i > 10)
			break;
	}
}

// ==============================

void SkipList::clear_(){
	dataSize_ = 0;
	dataCount_ = 0;

	std::fill(heads_.begin(), heads_.end(), nullptr);
}

auto SkipList::locate_(const StringRef &key, bool const complete_evaluation) const -> NodeLocator{
	// it is extremly dangerous to have key == nullptr here.
	if (key.empty()){
		std::logic_error exception("Key can not be nullptr in SkipList::_locate");
		throw exception;
	}

	NodeArrayC loc;

	// smart over-optimizations, such skip NULL lanes or
	// start from the middle of the list did not pay off.

	int cmp = 1;

	const Node *node = nullptr;
	const Node *prev = nullptr;

	height_type height = height_;
	while(height){
		node = prev ? prev : heads_[height - 1];

		while(node){
			const Pair & data = node->data;
			cmp = data.cmp(key);

			if (cmp >= 0)
				break;

			prev = node;
			node = node->next[height - 1];
		}


		if (complete_evaluation == false && cmp == 0)
			return { node, loc };

		loc[height - 1] = prev;

		--height;
	}

	return { cmp ? nullptr : node , loc };
}

auto SkipList::_getRandomHeight() -> height_type{
	// This gives slightly better performance,
	// than divide by 3 or multply by 0.33
	auto part = rand_.max() >> 1;

	// We execute rand() inside the loop,
	// but performance is fast enought.

	height_type h = 1;
	while(h < height_ && rand_() > part)
		h++;

	return h;
}



// ==============================


SkipList::Iterator &SkipList::Iterator::operator++(){
	node_ = node_->next[0];
	return *this;
}

const Pair &SkipList::Iterator::operator*() const{
	return node_->data;
}

bool SkipList::Iterator::operator==(const Iterator &other) const{
	return node_ == other.node_;
}


} // namespace

