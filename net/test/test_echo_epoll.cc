#include "selector/epollselector.h"
#include "worker/echoworker.h"

using MySelector	= net::selector::EPollSelector;
using MyWorker		= net::worker::EchoWorker;

#include "test_selector_impl.h"

