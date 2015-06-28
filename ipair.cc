#include "ipair.h"

#include <stdio.h>	// printf

IPair::IPair(IPair && other) :
	IPair(other._blob, other._ownership){
	other._blob = nullptr;
	other._ownership = false;
}

IPair::~IPair(){
	if (_ownership)
		destroy();
}

void IPair::destroy(){
	delete[] _blob;
	_ownership = false;
	_blob = nullptr;
}

void IPair::print() const{
	static const char *format = "%-10s | %-20s\n";

	printf(format,
		getKey(), getVal()
	);
}

