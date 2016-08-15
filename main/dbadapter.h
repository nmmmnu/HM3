#ifndef DBADAPTER_H_
#define DBADAPTER_H_

#include <sstream>

template<class LIST>
class DBAdapter{
private:
	constexpr static size_t MAX_RESULTS = 50;

public:
	DBAdapter(LIST &list) : list_(list){}

	std::string get(const StringRef &key) const{
		const auto &p = list_.get(key);
		std::string s = p.getVal();
		std::cout << s << std::endl;
		return s;
	}

	std::vector<std::string> getall(const StringRef &key) const{
		std::vector<std::string> result;

		result.reserve(MAX_RESULTS * 2);

		const auto bit = list_.lowerBound(key);
		const auto eit = list_.end();

		size_t c = 0;
		for(auto it = bit; it != eit; ++it){
			result.push_back(it->getKey());
			result.push_back(es_(it->getVal()));

			if (++c >= MAX_RESULTS)
				break;
		}

		for(auto x : result)
			std::cout << x << std::endl;

		return result;
	}

	std::string info() const{
		std::stringstream ss;

		ss	<< "Keys (estimated): "	<< list_.getCount(true)	<< std::endl
			<< "Size: "		<< list_.getMemSize()	<< std::endl
		;

		return ss.str();
	}

private:
	static std::string es_(const StringRef &data){
		return data.empty() ? "0" : data;
	}

private:
	LIST &list_;
};


#endif

