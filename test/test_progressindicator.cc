#include <cstdio>
#include <cstdint>

class ProgressIndicator{
public:
	constexpr static unsigned	STEPX	= 100000;

	constexpr static unsigned	SHIFT	= 3;
	constexpr static double		STEP	= 100.0 / (1 << SHIFT);
	
	constexpr static const char*	MSG	= "Processing";
	
public:
	ProgressIndicator(const char *msg = MSG) : _msg(msg){}
	
	template <typename T>
	void operator()(T part, T total){
		if (total == 0)
			return;
				
		++_rows;
		
		if ( _rows % (total >> SHIFT) )
			return;
		
		++_proc;
				
		printf("%s %6.2f %% %10llu items...\n", _msg, _proc * STEP, _rows);
	}

	template <typename T>
	void operator()(T part){
		++_rows;
		
		if (_rows % STEPX)
			return;
							
		printf("%s %6s %% %10llu items...\n", _msg, "?.??", _rows);
	}

private:
	uint64_t	_rows = 0;
	double		_proc = 0;
	
	const char	*_msg;
};

using INT = int;

const INT MAX = 1000000;


int main(){
	ProgressIndicator p1;
	ProgressIndicator p2;

	for(INT i=0; i < MAX; ++i){
		p1(i, MAX);
	}

	for(INT i=0; i < MAX; ++i){
		p2(i);
	}

}

