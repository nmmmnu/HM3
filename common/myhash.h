#ifndef _DJB_HASH_H
#define _DJB_HASH_H

// DJB Hash function from CDB
template <typename uint>
struct DJB2Hash{
	uint operator()(const char *s) const;
};

template <typename uint>
struct DJB2AHash{
	uint operator()(const char *s) const;
};


// SDBM is reimplementation of NDBM
template <typename uint>
struct SDBMHash{
	uint operator()(const char *s) const;
};


// FNV1 http://www.isthe.com/chongo/tech/comp/fnv/index.html
template <typename uint>
struct FNV1Hash{
	uint operator()(const char *s) const;
};

template <typename uint>
struct FNV1AHash{
	uint operator()(const char *s) const;
};

#endif

