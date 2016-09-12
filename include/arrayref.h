#ifndef ARRAY_REF_H_
#define ARRAY_REF_H_

//#include <vector>

template<class T>
class ArrayRef{
public:
	using size_type		= size_t;
	using value_type	= T;

	using Iterator		= const T*;

public:
	ArrayRef() = default;

	ArrayRef(const value_type *ptr, size_t const size) noexcept :
				ptr_(ptr),
				size_(size){}

	template<size_t N>
	ArrayRef(const value_type (&ptr)[N]) noexcept:
				ArrayRef(ptr, N){}

//	ArrayRef(const std::vector<T> &vector) noexcept :
//				ArrayRef(vector.data(), vector.size()){}

public:
	bool empty() const noexcept{
		return ptr_ == nullptr || size_ == 0;
	}

	size_type size() const noexcept{
		return size_;
	}

	size_t bytes() const noexcept{
		return size_ * sizeof(value_type);
	}

	const value_type &operator[](size_type const index) const noexcept{
		return ptr_[index];
	}

	const value_type *data() const noexcept{
		return ptr_;
	}

	Iterator begin() const noexcept{
		return ptr_;
	}

	Iterator end() const noexcept{
		return ptr_ + size_;
	}

private:
	const value_type	*ptr_ = nullptr;
	size_type		size_ = 0;
};

#endif

