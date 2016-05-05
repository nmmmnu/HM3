#ifndef _MULTI_TABLE_ITERATOR_MATRIX_HELPER_H
#define _MULTI_TABLE_ITERATOR_MATRIX_HELPER_H

#include "iiterator.h"

namespace hm3{
namespace multitableiterator{


class BaseMultiTableIterator{
protected:
	BaseMultiTableIterator() = default;

protected:
	template <class TABLE>
	class MatrixHelper_{
	private:
		using Iterator		= typename TABLE::Iterator;

	public:
		MatrixHelper_(Iterator &&cur, Iterator &&end) :
						cur(std::move(cur)),
						end(std::move(end)){}

		MatrixHelper_(const TABLE &table, bool const endIt = false) :
						MatrixHelper_(
							endIt ? table.end() : table.begin(),
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

		bool operator==(const MatrixHelper_ &other) const{
			return cur == other.cur && end == other.end;
		}

		bool operator!=(const MatrixHelper_ &other) const{
			return ! operator==(other);
		}

	public:
		Iterator cur;
		Iterator end;
	};
};


} // namespace multitableiterator
} // namespace

#endif

