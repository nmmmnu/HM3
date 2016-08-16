#ifndef BTREE_INDEX_FILENAMES_H_
#define BTREE_INDEX_FILENAMES_H_

#include <string>

namespace hm3{
namespace btreeindex{


constexpr const char *DOT_INDX	= ".tree";
constexpr const char *DOT_DATA	= ".keys";


inline std::string filenameIndx(const std::string &filename){
	return filename + DOT_INDX;
}

inline std::string filenameData(const std::string &filename){
	return filename + DOT_DATA;
}


} // namespace btreeindex
} // namespace


#endif

