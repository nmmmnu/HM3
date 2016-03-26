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
		char		logo[TITLE_SIZE];	// 8
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

	DiskFileHeader(uint64_t count, bool sorted = false, uint64_t tombstones = 0, uint64_t createdMin = 0, uint64_t createdMax = 0);

	bool open(const StringRef &filename);
	bool open(std::ifstream &file);

	operator bool() const{
		return version_;
	}

	bool writeToFile(std::ofstream &file) const;

	void print() const;

public:
	uint8_t getVersion() const{
		return version_;
	}

	uint64_t getCount() const{
		return count_;
	}

	bool getSorted() const{
		return sorted_;
	}

private:
	void clear_();

	POD createPOD_() const;

private:
	uint8_t		version_	= 0;
	uint64_t	count_		= 0;

	uint64_t	created_	= 0;
	bool		sorted_		= false;
	uint64_t	tombstones_	= 0;
	uint64_t	createdMin_	= 0;
	uint64_t	createdMax_	= 0;
};

#endif

