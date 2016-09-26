#include "selector/kqueueselector.h"
#include "worker/echoworker.h"

using MySelector	= net::selector::KQueueSelector;
using MyWorker		= net::worker::EchoWorker;

#include "test_selector_impl.h"

