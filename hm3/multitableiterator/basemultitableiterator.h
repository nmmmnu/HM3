#ifndef _MULTI_TABLE_ITERATOR_MATRIX_HELPER_H
#define _MULTI_TABLE_ITERATOR_MATRIX_HELPER_H

#include "iiterator.h"

#include "stringref.h"

namespace hm3{
namespace multitableiterator{


namespace helpers{

	template <class TABLE>
	class IteratorPair_{
	private:
		using Iterator		= typename TABLE::Iterator;

	public:
		IteratorPair_(Iterator &&cur, Iterator &&end) :
						cur(std::move(cur)),
						end(std::move(end)){}

		IteratorPair_(const TABLE &table, bool const endIt = false) :
						IteratorPair_(
							endIt ? table.end() : table.begin(),
							table.end()
						){}

		IteratorPair_(const TABLE &table, const StringRef &key) :
						IteratorPair_(
							table.lowerBound(key),
							table.end()
						){}

	public:
		const Pair &operator *() const{
			if (cur == end)
				return Pair::zero();

			return *cur;
		}

		void operator ++(){
			++cur;
		}

		bool operator==(const IteratorPair_ &other) const{
			return cur == other.cur && end == other.end;
		}

		bool operator!=(const IteratorPair_ &other) const{
			return ! operator==(other);
		}

	public:
		Iterator cur;
		Iterator end;
	};

} // namespace base


} // namespace multitableiterator
} // namespace

#endif

