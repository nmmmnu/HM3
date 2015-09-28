#ifndef _ITable_H
#define _ITable_H

#include "iiterator.h"
#include "icountable.h"
#include "iversion.h"

#include "stringref.h"

#include <memory>

template <typename T>
class ITable : public IVersion, public ICountable<ITable<T>, uint64_t>{
	friend class ICountable<ITable>;

public:
	typedef uint64_t count_type;

public:
	Pair get(const StringRef &key) const;
	Pair operator[](const StringRef &key) const;
	bool exists(const StringRef &key) const;

	size_t getSize() const;

	std::unique_ptr<IIterator> getIterator() const;

	void print() const;

private:
	Pair _get(const StringRef &key) const{
		return impl()._get(key);
	}

	size_t _getSize() const{
		return impl()._getSize();
	}

	count_type _getCount() const{
		return impl()._getCount();
	}

	std::unique_ptr<IIterator> _getIterator() const{
		return impl()._getIterator();
	};

private:
	const T &impl() const{
		return *( static_cast<const T*>(this) );
	}
};

// ==============================

template <typename T>
inline Pair ITable<T>::get(const StringRef &key) const{
	return _get(key);
}

template <typename T>
inline Pair ITable<T>::operator[](const StringRef &key) const{
	return _get(key);
}

template <typename T>
inline bool ITable<T>::exists(const StringRef &key) const{
	return (bool) _get(key);
}

template <typename T>
inline size_t ITable<T>::getSize() const{
	return _getSize();
}

template <typename T>
inline std::unique_ptr<IIterator> ITable<T>::getIterator() const{
	return _getIterator();
}

template <typename T>
inline void ITable<T>::print() const{
	_getIterator()->print();
}

#endif

