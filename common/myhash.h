#ifndef _DJB_HASH_H
#define _DJB_HASH_H

// DJB Hash function from CDB
unsigned long DJB2Hash(const char *str);

// modified DJB Hash function from CDB
unsigned long DJB2AHash(const char *str);

// SDBM is reimplementation of NDBM
unsigned long SDBMHash(const char *str);

#endif

