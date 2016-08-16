#ifndef DISK_FILE_FILENAMES_H_
#define DISK_FILE_FILENAMES_H_

#include <string>

namespace hm3{
namespace diskfile{


constexpr const char *DOT_INDX = ".indx";
constexpr const char *DOT_DATA = ".data";


inline std::string filenameMeta(const std::string &filename){
	return filename;
}

inline std::string filenameIndx(const std::string &filename){
	return filename + DOT_INDX;
}

inline std::string filenameData(const std::string &filename){
	return filename + DOT_DATA;
}


} // namespace diskfile
} // namespace

#endif

