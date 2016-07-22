#ifndef _SAMPLER_SEARCH_ARRAY__SEARCH_H
#define _SAMPLER_SEARCH_ARRAY__SEARCH_H

#include "arraysearch/base.h"

#include <vector>


namespace hm3{
namespace arraysearch{

template<class LOOKUP>
class SamplerSearch{
public:
	static constexpr size_t DEFAULT_SAMPLES	= 100;
	static constexpr size_t MIN_CHUNK_SIZE	= 1000;

private:
	using SamplesContainer = std::vector<std::string>;

public:
	SamplerSearch(size_t const samplesCount = DEFAULT_SAMPLES) :
				samplesCount_(samplesCount < 2 ? samplesCount : DEFAULT_SAMPLES){
	}

	template <class ARRAY>
	result_type<ARRAY> operator()(const ARRAY &self, const StringRef &key);

private:
	template <class ARRAY>
	void populate_(const ARRAY &array);

private:
	LOOKUP			lookup_;

	size_t			samplesCount_;
	bool			populated_ = false;
	SamplesContainer	samples_;

};


} // namespace arraysearch
} // namespace

// ==============================

#include "samplersearch_impl.h"

#endif

