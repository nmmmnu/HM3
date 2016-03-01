#ifndef _DISK_FILE_FLUSH_H
#define _DISK_FILE_FLUSH_H

#include "diskfile.h"

// ==============================

template<class IDGENERATOR, class TABLELOADER = std::nullptr_t>
class DiskFileFlush{
private:
	template<class UIDGENERATOR>
	DiskFileFlush(
			UIDGENERATOR &&idGenerator,
			const StringRef &path,
			const StringRef &ext,
			bool const keepTombstones,
			TABLELOADER *loader
		):
				_idGenerator(std::forward<UIDGENERATOR>(idGenerator)),
				_path(path),
				_ext(ext),
				_keepTombstones(keepTombstones),
				_loader(loader){}

public:
	template<class UIDGENERATOR>
	DiskFileFlush(
			UIDGENERATOR &&idGenerator,
			const StringRef &path,
			const StringRef &ext,
			bool const keepTombstones = true
		):
				DiskFileFlush(
						std::forward<UIDGENERATOR>(idGenerator),
						path,
						ext,
						keepTombstones,
						nullptr
				){}

	template<class UIDGENERATOR>
	DiskFileFlush(
			UIDGENERATOR &&idGenerator,
			const StringRef &path,
			const StringRef &ext,
			TABLELOADER &loader,
			bool const keepTombstones
		):
				DiskFileFlush(
						std::forward<UIDGENERATOR>(idGenerator),
						path,
						ext,
						keepTombstones,
						&loader
				){}

	template<class UIDGENERATOR>
	DiskFileFlush(
			UIDGENERATOR &&idGenerator,
			const StringRef &path,
			const StringRef &ext,
			bool const keepTombstones,
			TABLELOADER &loader
		):
				DiskFileFlush(
						std::forward<UIDGENERATOR>(idGenerator),
						path,
						ext,
						keepTombstones,
						&loader
				){}

public:
	template<class LIST>
	bool operator << (LIST &list) const{
		if (list.isEmpty())
			return false;

		printf("Flushing data to disk...\n");

		const auto &filename = _path + _idGenerator() + _ext;

		DiskFile df{ filename };
		df.createFromList(list, _keepTombstones);

		list.removeAll();

		_notifyLoader(_loader);

		return true;
	}

private:
	template<class T>
	static bool _notifyLoader(T *loader){
		if (loader)
			return loader->refresh();

		return false;
	}

	static bool _notifyLoader(std::nullptr_t *){
		return false;
	}

private:
	IDGENERATOR	_idGenerator;
	std::string	_path;
	std::string	_ext;
	bool		_keepTombstones;
	TABLELOADER	*_loader;
};

#endif

