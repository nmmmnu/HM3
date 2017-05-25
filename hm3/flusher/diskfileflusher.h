#ifndef _DISK_FILE_FLUSH_H
#define _DISK_FILE_FLUSH_H

#include "diskfile/diskfilebuilder.h"

// ==============================

namespace hm3{
namespace flusher{


template<class IDGENERATOR>
class DiskFileFlusher{
public:
	template<class UIDGENERATOR>
	DiskFileFlusher(
			UIDGENERATOR &&idGenerator,
			const StringRef &path,
			const StringRef &ext,
			bool const keepTombstones = true
		):
				_idGenerator(std::forward<UIDGENERATOR>(idGenerator)),
				path_(path),
				_ext(ext),
				_keepTombstones(keepTombstones){}

public:
	template<class LIST>
	bool operator << (const LIST &list) const{
		if (list.isEmpty())
			return false;

		printf("Flushing data to disk...\n");

		const auto &filename = path_ + _idGenerator() + _ext;

		using DiskFileBuilder = diskfile::DiskFileBuilder;

		DiskFileBuilder df;
		df.createFromList(filename, list, _keepTombstones);

		return true;
	}

private:
	IDGENERATOR	_idGenerator;
	std::string	path_;
	std::string	_ext;
	bool		_keepTombstones;
};

} // namespace flusher
} // namespace

#endif

