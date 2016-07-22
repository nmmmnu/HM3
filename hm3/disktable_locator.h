
#define DISK_TABLE_LOCATOR	'J'
//#define DISK_TABLE_LOCATOR	'P'
//#define DISK_TABLE_LOCATOR	'B'

// ============================================

#if DISK_TABLE_LOCATOR == 'P'

#include "arraysearch/partitionsearch.h"
#include "arraysearch/binarylocator.h"

#define DISK_TABLE_LOCATOR_SEARCH	arraysearch::PartitionSearch<arraysearch::BinaryLocator>

#elif DISK_TABLE_LOCATOR == 'J'

#include "arraysearch/simplesearch.h"
#include "arraysearch/jumplocator.h"

#define DISK_TABLE_LOCATOR_SEARCH	arraysearch::SimpleSearch<arraysearch::JumpLocator>

#elif DISK_TABLE_LOCATOR == 'B'

#include "arraysearch/simplesearch.h"
#include "arraysearch/binarylocator.h"

#define DISK_TABLE_LOCATOR_SEARCH	arraysearch::SimpleSearch<arraysearch::BinaryLocator>

#else

#error "Please provide some locator"

#endif

// ============================================

#undef DISK_TABLE_LOCATOR

