#include "pair.h"

#include <string.h>	// strcmp

Pair::Pair(IChecksumCalculator *checksumCalculator){
	this->_checksumCalculator = checksumCalculator;
}

const char *Pair::getKey() const{
	return & _buffer[0];
}

const char *Pair::getValue() const{
	return _valLen ? & _buffer[ _keyLen + 1 ] : NULL;
}

int Pair::cmp(const char *key) const{
	return key == NULL ? -1 : strcmp(getKey(), key);
}

bool Pair::valid() const{
	if (_checksumCalculator == NULL)
		return true;
	
	return _checksum == _getChecksum();
}

uint8_t Pair::_getChecksum() const{
	return _checksumCalculator->calcChecksum(_buffer, _bufferLen);
}
