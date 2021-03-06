/*此文件主要是将数据以小端的形式写进内存和硬盘*/
//从这个头文件看到的：
//1 Varint 为变长int 
//2 小端大端解法简单粗爆
//3 4种cast

#ifndef STORAGE_LEVELDB_UTIL_CODING_H
#define STORAGE_LEVELDB_UTIL_CODING_H

//为什么不用cstdint
#include <stdint.h>
//为什么不用cstring
#include <string.h>
#include <string>
#include "leveldb/slice.h"
#include "port/port.h"

namespace leveldb{
//standard put...
extern void PutFixed32(std::string *dst,uint32_t value);
extern void PutFixed64(std::string *dst,uint32_t value);
extern void PutVarint32(std::string *dst,uint32_t value);
extern void PutVarint64(std::string *dst,uint32_t value);
extern void PutLenghtPrefixedSlice(std::string *dst, const Slice &value);

//standard get...
extern bool GetVarint32(Slice *input, uint32_t *value);
extern bool GetVarint64(Slice *input, uint64_t *value);
extern bool GetLengthPrefixedSlice(Slice *input, Slice *result);

extern int VarintLength(uint64_t v);

extern void EncodeFixed32(char *dst,uint32_t value);
extern void EncodeFixed64(char *dst,uint64_t value);

extern char* EncodeVarint32(char *dst,uint32_t vlaue);
extern char* EncodeVarint64(char *dst,uint64_t vlaue);

inline uint32_t DecodeFixed32(const char* ptr){
	if (port::kLitterEndian){
		uint32_t result;
		memcpy(&result,ptr,sizeof(result));
		return result;
	}else{
		return ((static_cast<uint32_t>(static_cast<unsigned char>(ptr[0])))
			| (static_cast<uint32_t>(static_cast<unsigned char>(ptr[1])) << 8 )
			| (static_cast<uint32_t>(static_cast<unsigned char>(ptr[2])) << 16 )
			| (static_cast<uint32_t>(static_cast<unsigned char>(ptr[3])) << 24 ));
	}
}

inline uint64_t DecodeFixed64(const char* ptr){
	if ( port::kLitterEndian ){
		uint64_t result;
		memcpy(&result,ptr,sizeof(result));
	}else{
		uint64_t lo = DecodeFixed32(ptr);
		uint64_t hi = DecodeFixed32(ptr+4);
		return (hi<<32) | lo;
	}
}

extern const char* GetVarint32PtrFallback(const char* p,
											const char* limit,
											uint32_t* value);
inline const char* GetVarint32Ptr(const char* p,
								const char* limit,
								uint32_t *value){
	if (p < limit){
		uint32_t result = *(reinterpret_cast<const unsigned char*>(p));
		if ((result & 128) == 0){
			*value = result;
			return p+1;
		}
	}
	return GetVarint32PtrFallback(p,limit,value);
}

}



#endif /* end of include guard: STORAGE_LEVELDB_UTIL_CODING_H */
