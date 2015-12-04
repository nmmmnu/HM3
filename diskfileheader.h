#ifndef _DISK_FILE_HEADER_H
#define _DISK_FILE_HEADER_H

#include <cstdint>
#include <iostream>

#include "stringref.h"

class DiskFileHeader{
public:
	constexpr static const size_t	TITLE_LOGO_SIZE		= 4;		// size of ZUSE string
	constexpr static const size_t	TITLE_SIZE		= 8;
	constexpr static const char	*TITLE			= "ZUSE" "003";	// must be TITLE_SIZE
	constexpr static const uint8_t	VERSION			= 3;

	constexpr static const uint8_t	HEADER_NOT_SORTED	= 0;
	constexpr static const uint8_t	HEADER_SORTED		= 1;

public:
	struct PODBase{
		char		logo[TITLE_SIZE];	// 8
		uint8_t		version;	// 1
		uint64_t	size;		// 8
	} __attribute__((__packed__));

	struct POD{
		char		logo[8];	// 8
		uint8_t		version;	// 1
		uint64_t	size;		// 8
		// base ends here
		uint64_t	created;	// 8
		uint8_t		sorted;		// 1
		uint64_t	tombstones;	// 8
		uint64_t	createdMin;	// 8
		uint64_t	createdMax;	// 8
	} __attribute__((__packed__));

public:
	DiskFileHeader() = default;

	DiskFileHeader(size_t count, bool sorted = false, size_t tombstones = 0, uint64_t createdMin = 0, uint64_t createdMax = 0);

	bool open(const StringRef &filename);
	bool open(std::ifstream &file);

	operator bool() const{
		return _version;
	}

	bool writeToFile(std::ofstream &file) const;

	void print() const;

public:
	uint8_t getVersion() const{
		return _version;
	}

	size_t getCount() const{
		return _count;
	}

	bool getSorted() const{
		return _sorted;
	}

private:
	void _clear();

	POD _createPOD() const;

private:
	uint8_t		_version	= 0;
	size_t		_count		= 0;

	uint64_t	_created	= 0;
	bool		_sorted		= false;
	size_t		_tombstones	= 0;
	uint64_t	_createdMin	= 0;
	uint64_t	_createdMax	= 0;
};

#endif

