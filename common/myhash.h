#ifndef _DJB_HASH_H
#define _DJB_HASH_H

// DJB Hash function from CDB
template <typename uint>
uint DJB2Hash(const char *s);

template <typename uint>
uint DJB2AHash(const char *s);

// SDBM is reimplementation of NDBM
template <typename uint>
uint SDBMHash(const char *s);

// FNV1 http://www.isthe.com/chongo/tech/comp/fnv/index.html
template <typename uint>
uint FNV1Hash(const char *s);

template <typename uint>
uint FNV1AHash(const char *s);


#endif

