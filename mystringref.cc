#include <cstring>

#include <string>
#include <ostream>

class MyStringRef{
public:
	MyStringRef() = default;

	MyStringRef(const char *data, size_t const size) : 
		_size(size),
		_data(size ? data : __zeroStr){}
		
	MyStringRef(const char *data) : 
		MyStringRef(data, data ? strlen(data) : 0){}

	MyStringRef(const std::string &s) :
		_size(s.size()),
		_data(s.data()){}
	
	// ==================================
	
	bool empty() const{
		return _size == 0;
	}

	const char *data() const{
		return _data;
	}

	size_t size() const{
		return _size;
	}
	
	std::string toString() const{
		return std::string(_data, _size);
	}
	
	operator std::string() const{
		return toString();
	}

	// ==================================

	int compare(const char *data) const{
		return strcmp(_data, data);
	}
	
	int compare(const std::string &s) const{
		return compare(s.data() );
	}

	int compare(const MyStringRef &sr) const{
		return compare(sr.data() );
	}

	// ==================================

	bool operator ==(const char *data) const{
		return compare(data) == 0;
	}

	bool operator ==(const std::string &data) const{
		return compare(data) == 0;
	}

	bool operator ==(const MyStringRef &data) const{
		return compare(data) == 0;
	}

	// ==================================

	bool operator !=(const char *data) const{
		return ! (*this == data);
	}

	bool operator !=(const std::string &data) const{
		return ! (*this == data);
	}

	bool operator !=(const MyStringRef &data) const{
		return ! (*this == data);
	}

	// ==================================
	
	const char *c_str() const{
		return data();
	}

	size_t length() const{
		return size();
	}

private:
	size_t		_size	= 0;
	const char	*_data	= __zeroStr;

private:
	constexpr
	static const char	*__zeroStr = "";
};

std::ostream& operator << (std::ostream& os, const MyStringRef &sr) {
	os << sr.data();
	return os;
}       


#include <iostream>

int main(){
	const char *hello = "hello";
	
	MyStringRef sr = hello;

	std::string ss = sr;
	
	MyStringRef sr2 = ss;

	std::cout <<  sr << std::endl;
	std::cout << (sr == sr2  ) << std::endl;
	std::cout << (sr == ss   ) << std::endl;
	std::cout << (sr == hello) << std::endl;

	return 0;
}

