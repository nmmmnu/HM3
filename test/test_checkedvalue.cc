#include "checkedvalue.h"

#include <cstdio>

constexpr const char *module = "checkedvalue";

#define PRINTF_TEST(test, result) \
	printf("%-15s Testing %-20s %s\n", module, test, result ? "OK" : "Fail")

enum class CalcError{
	OK,
	ERROR,
	DIV_BY_ZERO
};

template<class T, T OK, T NOT_OK>
CheckedValue<float, T, OK> test_function(float const a, float const b){
	if (b == 0)
		return NOT_OK;

	return a / b;
}

template <class T, T OK, T ERR>
void test_pass(bool const success){
	constexpr int n = 5;

	const float dividor	= success ? 1 : 0;
	const auto  expected	= success ? OK : ERR;

	const auto v = test_function<T, OK, ERR>(n, dividor);

	PRINTF_TEST("bool",	v == success		);
	PRINTF_TEST("code()",	v.code() == expected	);

	if (success)
	PRINTF_TEST("get()",	v.get() == n		);

	printf("\n");
}

template <class T, T OK, T ERR>
void test(){
	test_pass<T, OK, ERR>(true);
	test_pass<T, OK, ERR>(false);
}

int main(){
	test<CalcError,	CalcError::OK,	CalcError::DIV_BY_ZERO	>();
	test<bool,	true,		false			>();
	test<int,	0,		1			>();
}

