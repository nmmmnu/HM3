#ifndef _DISK_FILE_FLUSH_H
#define _DISK_FILE_FLUSH_H

#include "diskfile.h"

#include <cstdio>

// ==============================

template<class IDGENERATOR>
class DiskFileFlush{
public:
	DiskFileFlush(IDGENERATOR &&idGenerator, const StringRef &path, const StringRef &ext):
				_idGenerator(std::move(idGenerator)),
				_path(path),
				_ext(ext){
	}

	template<class LIST>
	bool operator()(LIST &list, bool const keepInvalid, bool const keepTombstones) const{
		if (list.isEmpty())
			return false;

		printf("Flushing data to disk...\n");

		const auto &filename = _path + _idGenerator() + _ext;

		DiskFile df{ filename };
		df.createFromList(list, true, true);

		list.removeAll();

		return true;
	}

private:
	IDGENERATOR	_idGenerator;
	std::string	_path;
	std::string	_ext;
};

// ==============================

#endif

