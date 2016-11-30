#ifndef HASH_INDEX_FILENAMES_H_
#define HASH_INDEX_FILENAMES_H_

#include "stringref.h"

namespace hm3{
namespace hashindex{


constexpr const char *DOT_INDX	= ".hash";


inline std::string filenameIndx(const StringRef &filename){
	return StringRef::concatenate( { filename, DOT_INDX } );
}


} // namespace btreeindex
} // namespace


#endif
