#ifndef BTREE_INDEX_FILENAMES_H_
#define BTREE_INDEX_FILENAMES_H_

#include "stringref.h"

namespace hm3{
namespace btreeindex{


constexpr const char *DOT_INDX	= ".tree";
constexpr const char *DOT_DATA	= ".keys";


inline std::string filenameIndx(const StringRef &filename){
	return StringRef::concatenate( { filename, DOT_INDX } );
}

inline std::string filenameData(const StringRef &filename){
	return StringRef::concatenate( { filename, DOT_DATA } );
}


} // namespace btreeindex
} // namespace


#endif

