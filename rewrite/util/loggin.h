#ifndef STORAGE_LEVELDB_UTIL_LOGGER_H_
#define STORAGE_LEVELDB_UTIL_LOGGER_H_

/*
 * 好像没什么可以学到的，最奇怪了
 */

#include <stdio.h>
#include <stdint.h>
#include <string>
#include "port/port.h"

namespace leveldb{
class Slice;
class WriteableFile;

extern void AppendNumberTo(std::string *str, uint64_t num);

extern void AppendEscapedStringTo(std::string *str, const Slice& value);

extern std::string NumberToString(uint64_t num);

extern std::string EscapeString(const Slice &value);

extern bool ConsumeDecimalNumber(Slice* in, uint64_t *val);

}

#endif /* end of include guard: STORAGE_LEVELDB_UTIL_LOGGER_H_ */
