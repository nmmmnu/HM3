#ifndef MY_HASH_H_
#define MY_HASH_H_

#include <cstdint>
#include <cstdlib>	// size_t

#include "myhash_impl.h"

namespace myhash{

// DJB Hash function from CDB
template <typename uint>
using DJB2 = impl::MyHashBase<impl::DJB2<uint> >;

template <typename uint>
using DJB2A = impl::MyHashBase<impl::DJB2A<uint> >;


// SDBM is reimplementation of NDBM
template <typename uint>
using SDBM = impl::MyHashBase<impl::SDBM<uint> >;


// FNV1 http://www.isthe.com/chongo/tech/comp/fnv/index.html
template <typename uint>
using FNV1 = impl::MyHashBase<impl::FNV1<uint> >;

template <typename uint>
using FNV1A = impl::MyHashBase<impl::FNV1A<uint> >;


// NMEA0183
using NMEA0183 = impl::MyHashBase<impl::NMEA0183>;

} // namespace

#endif

