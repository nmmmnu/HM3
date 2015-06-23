#ifndef _DISK_MANAGER_H
#define _DISK_MANAGER_H

#include "ilist.h"
#include "iarray.h"

class DiskManager{ //: virtual public IList, virtual public IArray{
public:
	DiskManager(const char *filename, bool autoOpen = true);
	virtual ~DiskManager();

	bool createFromIterator(IIterator *it, uint64_t datacount) const;
	inline bool createFromList(IList *list) const;

	bool open();
	void close();
private:
	const char	*_filename;
	bool		_isOpen		= false;

private:
	bool _writeIteratorToFile(IIterator *it, uint64_t datacount, FILE *F) const;

};

// ==============================

inline bool DiskManager::createFromList(IList *list) const{
	return createFromIterator(list->getIterator(), list->getSize());
}

#endif
