#ifndef MY_OPAQUE_NUMBER_
#define MY_OPAQUE_NUMBER_

#include <ostream>

template<typename T>
class Number{
	static_assert(std::is_arithmetic<T>::value, "T must be arithmetic");

public:
	constexpr Number() = default;

	template<typename UT>
	constexpr Number(UT &&value) : value(std::forward<UT>(value)){}

	// ====================================

	constexpr T get() const{
		return value;
	}

	// ====================================

	constexpr bool operator == (const Number &other) const{
		return value == other.value;
	}

	constexpr bool operator != (const Number &other) const{
		return value != other.value;
	}

	// ====================================

	constexpr bool operator > (const Number &other) const{
		return value > other.value;
	}

	constexpr bool operator < (const Number &other) const{
		return value < other.value;
	}

	// ====================================

	constexpr bool operator >= (const Number &other) const{
		return value >= other.value;
	}

	constexpr bool operator <= (const Number &other) const{
		return value <= other.value;
	}

	// ====================================

	Number &operator ++(){
		++value;
		return *this;
	}

	Number &operator --(){
		--value;
		return *this;
	}

	// ====================================
#if 0
	Number operator ++(){
		return value++;
	}

	Number operator --(){
		return value--;
	}
#endif
	// ====================================

	Number &operator += (const Number &other){
		value += other.value;
		return *this;
	}

	Number &operator -= (const Number &other){
		value -= other.value;
		return *this;
	}
	Number &operator *= (const Number &other){
		value *= other.value;
		return *this;
	}

	Number &operator /= (const Number &other){
		value /= other.value;
		return *this;
	}

	// ====================================

	constexpr Number operator + (const Number &other) const{
		return value + other.value;
	}

	constexpr Number operator - (const Number &other) const{
		return value - other.value;
	}

	constexpr Number operator * (const Number &other) const{
		return value * other.value;
	}

	constexpr Number operator / (const Number &other) const{
		return value / other.value;
	}

	// ====================================

private:
	T value = T(0);
};

template<typename T>
inline std::ostream& operator << (std::ostream& os, const Number<T> &a){
	return os << a.get();
}

#endif

