#include <iostream>

#ifdef log__
#undef log__
#endif

namespace{
	inline void log__(){
		std::clog << '\n';
	}

	template<typename T, typename... ARGS>
	void log__(T first, ARGS... args){
		std::clog << first << " ";
		log__(args...);
	}
}

