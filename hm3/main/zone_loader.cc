#include <iostream>
#include <fstream>
#include <sstream>

#include "flushlist.h"
#include "skiplist.h"

#include "idgenerator/idgeneratordate.h"
#include "flusher/diskfileflusher.h"

#include "stringtokenizer.h"
#include "filereader.h"



//using MemList		= HashList<std::vector<SkipList> >;
using MemList		= hm3::SkipList;
using MyIDGenerator	= hm3::idgenerator::IDGeneratorDate;
using Flusher		= hm3::flusher::DiskFileFlusher<MyIDGenerator>;


using size_type	= MemList::size_type;

constexpr size_t	MEMLIST_SIZE	= (size_t ) 512 * 1024 * 1024;
constexpr size_type	PROCESS_STEP	= 1000 * 10;


static void printUsage(const char *cmd);


template <class LIST, class READER>
int listLoad(LIST &list, READER &reader){
	size_type i = 0;

	while(reader){
		std::string line = reader.getLine();

		StringTokenizer st{ line, ':' };

		const auto &parts = st.getAll();

		if (parts.size() != 3 && parts.size() != 4){
			std::cout << "Problem with line " << line << '\n';
			continue;
		}

		char const op		= parts[0][0];
		const auto &date	= parts[1];
		const auto &domain	= parts[2];

		{
			// add locator key

			std::stringstream sskey;
			sskey << 't' << ':' << date << ':' << domain;

			list.put( { sskey.str(), "1" } );
		}

		{
			// add ns key

			std::stringstream sskey;
			sskey << 'd' << ':' << domain << ':' << date;

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
					<< list.getList().getBytes() << " bytes..."
					<< '\n';
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

	FileReader reader{ filename };

	return listLoad(mylist, reader);
}



static void printUsage(const char *cmd){
	std::cout	<< "Usage:" << '\n'
			<< "\t" << cmd << " [file.txt] [lsm_path/] - load file.txt, then create / add to lsm_path/" << '\n'
			<< '\n';
}

