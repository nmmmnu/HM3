#ifndef BLOB_REF_H_
#define BLOB_REF_H_

#include <cstddef>	// size_t

class BlobRef{
public:
	BlobRef(const void *mem, size_t const size) noexcept :
				mem_( (const char *) mem),
				size_(size){}

	template<size_t N>
	BlobRef(const char(&mem)[N]) noexcept:
				BlobRef(mem, N){}

public:
	bool empty() const noexcept{
		return mem_ == nullptr || size_ == 0;
	}

	size_t size() const noexcept{
		return size_;
	}

	const void *data_() const noexcept{
		return mem_;
	}

public:
	const void *safeAccessMemory(size_t const pos, size_t const size) const noexcept{
		if (pos + size <= size_)
			return & mem_[pos];
		else
			return nullptr;
	}

	template <class T>
	const T *as(size_t const pos = 0, size_t const elements = 1) const noexcept{
		return (const T *) safeAccessMemory(pos, elements * sizeof(T));
	}

private:
	const char	*mem_ = nullptr;
	size_t		size_ = 0;
};

#endif

