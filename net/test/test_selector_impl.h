
// selector definiton already available here.

#include "sockets.h"
#include "asyncloop.h"

int main(){
	using MySelector = MySelector;
	using MyWorker   = MyWorker;

	int const fd1 = net::socket_create(net::socket_tcp,  "localhost.not.used.yet", 2000);
	int const fd2 = net::socket_create(net::socket_tcp,  "localhost.not.used.yet", 2002);
	int const fd3 = net::socket_create(net::socket_unix, "/tmp/echo");

	net::AsyncLoop<MySelector, MyWorker> loop( MySelector{ 4 }, MyWorker{}, { fd1, fd2, fd3 } );

	while(loop.process());
}

