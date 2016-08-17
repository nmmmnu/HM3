
namespace{
	template<typename T>
	void log__print__(T first){
		std::cout << first << " ";
	}

	template<typename T, typename... ARGS>
	void log__print__(T first, ARGS... args){
		log__print__(first);
		log__print__(args...);
	}

	template<typename... ARGS>
	void log__(ARGS... args){
		log__print__(args...);
		std::cout << std::endl;
	}
};

