#ifndef DISK_FILE_FILENAMES_H_
#define DISK_FILE_FILENAMES_H_

#include <string>

namespace hm3{
namespace diskfile{


constexpr const char *DOT_INDX = ".indx";
constexpr const char *DOT_DATA = ".data";


inline const StringRef &filenameMeta(const StringRef &filename){
	return filename;
}

inline std::string filenameIndx(const StringRef &filename){
	return StringRef::concatenate( { filename, DOT_INDX } );
}

inline std::string filenameData(const StringRef &filename){
	return StringRef::concatenate( { filename, DOT_DATA } );
}


} // namespace diskfile
} // namespace

#endif

