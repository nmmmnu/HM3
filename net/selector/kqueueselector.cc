#include "KQueueSelector.h"

#include <sys/event.h>	// kqueue
#include <unistd.h>	// close, for closeKQueue_()

namespace net{
namespace selector{


namespace{

auto kQueueConvert(const FDEvent event) -> decltype(epoll_event::events){
	switch(event){
		default:
		case FDEvent::READ	: return EVFILT_READ;
		case FDEvent::WRITE	: return EVFILT_WRITE;
	}
}

}

// ===========================

KQueueSelector::KQueueSelector(uint32_t const maxFD) :
				statusData_(maxFD){
	initializeKQueue_();
}

KQueueSelector::KQueueSelector(KQueueSelector &&other) :
				kqueueFD_	(std::move(other.kqueueFD_	)),
				statusData_	(std::move(other.statusData_	)),
				statusCount_	(std::move(other.statusCount_	)){
	other.kqueueFD_ = -1;
}


KQueueSelector &KQueueSelector::operator =(KQueueSelector &&other){
	swap(other);

	return *this;
}

void KQueueSelector::swap(KQueueSelector &other){
	using std::swap;

	swap(kqueueFD_		, other.kqueueFD_	);
	swap(statusData_	, other.statusData_	);
	swap(statusCount_	, other.statusCount_	);
}


KQueueSelector::~KQueueSelector(){
	closeKQueue_();
}

// ===========================

uint32_t KQueueSelector::maxFD() const{
	return (uint32_t) statusData_.size();
}

WaitStatus KQueueSelector::wait(int const timeout){
	if (kqueueFD_ < 0)
		return WaitStatus::ERROR;

	statusCount_ = epoll_wait(kqueueFD_, statusData_.data(), (int) statusData_.size(), timeout);

	if (statusCount_ < 0)
		return WaitStatus::ERROR;

	if (statusCount_ == 0)
		return WaitStatus::NONE;

	return WaitStatus::OK;
}

FDResult KQueueSelector::getFDStatus(uint32_t const no) const{
	if (no < (uint32_t) statusCount_){
		const epoll_event &ev = statusData_[no];

		int const fd = ev.data.fd;

		if (ev.events & EPOLLERR)
			return { fd, FDStatus::ERROR };

		if ((ev.events & EPOLLIN) || (ev.events & EPOLLHUP))
			return { fd, FDStatus::READ };

		if (ev.events & EPOLLOUT)
			return { fd, FDStatus::WRITE };
	}

	return FDStatus::STOP;
}

bool KQueueSelector::insertFD(int const fd, FDEvent const event){
	return mutateFD_(fd, event, EV_ADD | EV_ENABLE);
}

bool KQueueSelector::updateFD(int const fd, FDEvent const event){
	return mutateFD_(fd, event, EV_DELETE);
}

bool KQueueSelector::mutateFD_(int const fd, FDEvent const event, int const op){
	kevent ev;
	ev.events = kQueueConvert(event);
	ev.data.fd = fd;

	int const result = EV_SET(&ev, kqueueFD_, EVFILT_READ, op, 0, 0, 0);

	return result >= 0;
}

bool KQueueSelector::removeFD(int const fd){
	kevent ev;
	int const result = EV_SET(&ev, kqueueFD_, EVFILT_READ, op, 0, 0, 0);

	return result >= 0;
}

// ===========================

void KQueueSelector::initializeKQueue_(){
	kqueueFD_ = kqueue(); // must be >= 0
}

void KQueueSelector::closeKQueue_(){
	::close(kqueueFD_);
}


} // namespace selector
} // namespace

