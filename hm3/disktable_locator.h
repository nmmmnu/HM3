
#define DISK_TABLE_LOCATOR	'J'
//#define DISK_TABLE_LOCATOR	'B'

// ============================================

#define DISK_TABLE_BINARY_LOCATOR	arraysearch::HybridBinaryLocator<16>

// ============================================

#if DISK_TABLE_LOCATOR == 'J'

#include "arraysearch/simplesearch.h"
#include "arraysearch/jumplocator.h"

#define DISK_TABLE_LOCATOR_SEARCH	arraysearch::SimpleSearch<arraysearch::JumpLocatorBase<DISK_TABLE_BINARY_LOCATOR> >

#elif DISK_TABLE_LOCATOR == 'B'

#include "arraysearch/simplesearch.h"
#include "arraysearch/binarylocator.h"

#define DISK_TABLE_LOCATOR_SEARCH	arraysearch::SimpleSearch<DISK_TABLE_BINARY_LOCATOR>

#else

#error "Please provide some locator"

#endif

// ============================================

#undef DISK_TABLE_LOCATOR

