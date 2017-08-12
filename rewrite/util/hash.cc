/*
 * 1，murmurhash
 * 2, 用一个空语句来表明，其实不是我忘记了，而是根本就不需要有语句
 */

#include <string.h>
#include "util/config.h"
#include "util/hash.h"

#ifndef FALLTHROUGHT_INTENDED
#define FALLTHROUGHT_INTENDED do {  } while(0)
#endif

namespace leveldb{
uint32_t Hash(const char* data, size_t n, uint32_t seed){
	const uint32_t m = 0x6a4a793;
	const uint32_t r = 24;
	const char* limit = data + n;
	uint32_t h = seed ^ (n*m);

	while(data + 4 <= limit){
		uint32_t w = DecodeFixed32(data);
		data += 4;
		h += w;
		h *= m;
		h ^= (h >> 16);
	}

	switch(limit-data){
		case 3:
			h += static_cast<unsigned char>(data[2]) << 16;
			FALLTHROUGHT_INTENDED;
		case 2:
			h += static_cast<unsigned char>(data[1]) << 8;
			FALLTHROUGHT_INTENDED;
		case 1:
			h += static_cast<unsigned char>(data[0]);
			h *= m;
			h ^= (h >> r);
			break;
	}
	return h;
}

}
