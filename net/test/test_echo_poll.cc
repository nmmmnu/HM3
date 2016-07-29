#include "selector/pollselector.h"
#include "worker/echoworker.h"

using MySelector	= net::selector::PollSelector;
using MyWorker		= net::worker::EchoWorker;

#include "test_selector_impl.h"

