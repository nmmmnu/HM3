#include <iostream>
#include <fstream>
#include <sstream>

#include "flushlist.h"
#include "skiplist.h"

#include "idgenerator/idgeneratordate.h"
#include "flusher/diskfileflusher.h"

#include "stringtokenizer.h"



//using MemList		= HashList<std::vector<SkipList> >;
using MemList		= hm3::SkipList;
using MyIDGenerator	= hm3::idgenerator::IDGeneratorDate;
using Flusher		= hm3::flusher::DiskFileFlusher<MyIDGenerator>;


using size_type	= MemList::size_type;

constexpr size_t	MEMLIST_SIZE	= (size_t ) 512 * 1024 * 1024;
constexpr size_type	PROCESS_STEP	= 1000 * 10;


static void printUsage(const char *cmd);

static std::string &trim(std::string &line);



template <class LIST>
static int listLoad(LIST &list, const StringRef &filename){
	std::ifstream f;
	f.open(filename);

	size_type i = 0;

	for(std::string line; getline(f, line);){
		trim(line);

		StringTokenizer st{ line, ':' };

		const auto &parts = st.getAll();

		if (parts.size() != 3 && parts.size() != 4){
			std::cout << "Problem with line " << line << std::endl;
			continue;
		}

		char const op		= parts[0][0];
		const auto &date	= parts[1];
		const auto &domain	= parts[2];

		{
			// add locator key

			std::stringstream sskey;
			sskey << date << ':' << domain;

			list.put( { sskey.str(), "1" } );
		}

		{
			// add ns key

			std::stringstream sskey;
			sskey << domain << ':' << date;

			if (parts.size() == 4){
				const auto &ns = parts[3];

				std::stringstream ssval;

				ssval << op << ':' << ns;

				list.put( { sskey.str(), ssval.str() } );
			}else{
				list.put( { sskey.str(), "d" } );
			}
		}

		++i;

		if (i % PROCESS_STEP == 0){
			std::cout	<< "Processed "	<< i << " records, "
					<< "In memory "	<< list.getList().getCount() << " records, "
					<< list.getList().getMemSize() << " bytes..."
					<< std::endl;
		}
	}

	return 0;
}



int main(int argc, char **argv){
	if (argc <= 2){
		printUsage(argv[0]);
		return 1;
	}

	const auto filename	= argv[1];
	const auto lsm_path	= argv[2];
	const auto lsm_ext	= ".db";

	MemList memlist;

	using MyList	= hm3::FlushList<MemList,Flusher>;

	MyList mylist{
		memlist,
		Flusher{
			MyIDGenerator{},
			lsm_path,
			lsm_ext
		},
		MEMLIST_SIZE
	};

	return listLoad(mylist, filename);
}



static std::string &trim(std::string &line){
	constexpr const char *trim_ch = " \t\r\n";

	line.erase(line.find_last_not_of(trim_ch) + 1);

	return line;
}



static void printUsage(const char *cmd){
	std::cout	<< "Usage:" << std::endl
			<< "\t" << cmd << " [file.txt] [lsm_path/] - load file.txt, then create / add to lsm_path/" << std::endl
			<< std::endl;
}

