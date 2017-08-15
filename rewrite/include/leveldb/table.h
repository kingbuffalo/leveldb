#ifndef STORAGE_LEVELDB_INCLUDE_TABLE_H_
#define STORAGE_LEVELDB_INCLUDE_TABLE_H_

#include <stdint.h>
#include "leveldb/iterator.h"

namespace leveldb{
class Block;
class BlockHandle;
class Footer;
struct Options;
class RandomAccessFile;
struct ReadOptions;
class TableCache;

/*
 * Table是一个排过序的 key为string value为string, Tables 是不会变
 * 一个Table可以安全地被多线程访问而不需要额外的同步
 */

class Table{
	public:
		/*
		 * 尝试打开一个table 
		 */
		static Status Open(const Options& options,
							RandomAccessFile* file,
							uint64_t file_size,
							Table** table);
		~Table();
		Iterator* NewIterator( const ReadOptions&  ) const;
		uint64_t ApproximateOffsetOf(const Slice& key)const;

	private:
		struct Rep;
		Rep* ret_;

		explicit Table(Rep* rep) {rep_ = rep;}
		static Iterator* BlockReader(void*, const ReadOptions& const Slice&);
		friend class TableCache;
		Status InternalGet(
				const ReadOptions&, const Slice& key,
				void* arg,
				void(*handle_result)(void* arg, const Slice& k, const Slice& v));

		void ReadMeta(const Footer& footer);
		void ReadFilter(const Slice& filter_handle_value);

		Table(const Table&);
		void operator=(const Table&);
};
}

#endif /* end of include guard: STORAGE_LEVELDB_INCLUDE_TABLE_H_ */
