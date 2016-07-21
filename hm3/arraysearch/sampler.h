#ifndef _SAMPLER_ARRAY__SEARCH_H
#define _SAMPLER_ARRAY__SEARCH_H

#include "arraysearch/base.h"

#include <vector>


namespace hm3{
namespace arraysearch{

template<class LOOKUP>
class Sampler{
public:
	static constexpr size_t SAMPLES  = 100;
	static constexpr size_t MIN_SIZE = SAMPLES * SAMPLES;

public:
	Sampler(){
		populate();
	}

public:
	template <class ARRAY>
	void populate(const ARRAY &array);

private:
	std::vector<std::string>	samples_;

};


} // namespace arraysearch
} // namespace

// ==============================

#include "sampler_impl.h"

#endif

