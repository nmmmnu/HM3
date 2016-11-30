#ifndef MY_HASH_H_
#define MY_HASH_H_

#include <cstdint>
#include <cstdlib>	// size_t

#include "myhash_impl.h"


// DJB Hash function from CDB
template <typename uint>
using DJB2Hash = myhash::impl::MyHashBase<myhash::impl::DJB2<uint> >;

template <typename uint>
using DJB2AHash = myhash::impl::MyHashBase<myhash::impl::DJB2A<uint> >;


// SDBM is reimplementation of NDBM
template <typename uint>
using SDBMHash = myhash::impl::MyHashBase<myhash::impl::SDBM<uint> >;


// FNV1 http://www.isthe.com/chongo/tech/comp/fnv/index.html
template <typename uint>
using FNV1Hash = myhash::impl::MyHashBase<myhash::impl::FNV1<uint> >;

template <typename uint>
using FNV1AHash = myhash::impl::MyHashBase<myhash::impl::FNV1A<uint> >;


// NMEA0183
using NMEA0183Hash = myhash::impl::MyHashBase<myhash::impl::NMEA0183>;

#endif

