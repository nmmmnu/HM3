#ifndef _DISK_MANAGER_H
#define _DISK_MANAGER_H

#include "ilist.h"
#include "iarray.h"

class DiskManager{ //: virtual public IList, virtual public IArray{
public:
	DiskManager(const char *filename, bool autoOpen = true);
	virtual ~DiskManager();

	bool open();
	void close();

	       static bool create(const char *filename, IIterator *it, uint64_t datacount);
	inline static bool create(const char *filename, IList *list);

private:
	const char	*_filename;
	bool		_isOpen		= false;

private:
	static bool _writeIteratorToFile(IIterator *it, uint64_t datacount, FILE *F);

};

// ==============================

inline bool DiskManager::create(const char *filename, IList *list){
	if (list == NULL)
		return false;

	return create(filename, list->getIterator(), list->getCount());
}

#endif
