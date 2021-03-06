#ifndef STORAGE_LEVELDB_INCLUDE_C_H_
#define STORAGE_LEVELDB_INCLUDE_C_H_

#ifdef _cplusplus
extern "C"{
#endif

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

typedef struct leveldb_t leveldb_t;
typedef struct leveldb_cache_t leveldb_cache_t;
typedef struct leveldb_comparator_t leveldb_comparator_t;
typedef struct leveldb_env_t leveldb_env_t;
typedef struct leveldb_filelock_t leveldb_filelock_t;
typedef struct leveldb_filterpolicy_t leveldb_filterpolicy_t;
typedef struct leveldb_iterator_t leveldb_iterator_t;
typedef struct leveldb_logger_t leveldb_logger_t;
typedef struct leveldb_options_t leveldb_options_t;
typedef struct leveldb_randomfile_t leveldb_randomfile_t;
typedef struct leveldb_readoptions_t leveldb_readoptions_t;
typedef struct leveldb_seqfile_t leveldb_seqfile_t;
typedef struct leveldb_snapshot_t leveldb_snapshot_t;
typedef struct leveldb_writeablefile_t leveldb_writeablefile_t;
typedef struct leveldb_wirtebatch_t leveldb_wirtebatch_t;
typedef struct leveldb_writeoptions_t leveldb_writeoptions_t;

extern leveldb_t* leveldb_open(
		const leveldb_options_t* options,
		const char* name,
		char** errptr);

extern void leveldb_close(leveldb_t* db);

extern void leveldb_put(
		leveldb_t* db,
		const leveldb_writeoptions_t* options,
		const char* key, size_t keylen,
		const char* val, size_t vallen,
		char** errptr);

extern void leveldb_delete(
		leveldb_t* db,
		const leveldb_writeoptions_t* options,
		leveldb_writebatch_t* batch,
		char** errptr);

extern char* leveldb_get(
		leveldb* db,
		const char leveldb_readoptions_t* options,
		const char* key, size_t keylen,
		size_t* vallen,
		char** errptr);

extern leveldb_iterator_t* leveldb_create_iterator(
		leveldb* db,
		const leveldb_readoptions_t* options);

extern const leveldb_snapshot_t* leveldb_create_snapshot(
		leveldb_t* db);

extern void leveldb_release_snapshot(
		leveldb_t* db,
		const leveldb_snapshot_t* snapshot);

extern char* leveldb_property_value(
		leveldb_t* db,
		const char* propname);

extern void leveldb_approximate_size(
		leveldb_t* db,
		int num_ranges,
		const char* const* range_start_key, const size_t* range_start_key_len,
		const char* const* range_limit_key, const size_t* range_limit_key_len,
		uint64_t* sizes);

extern void leveldb_compact_range(
		leveldb_t* db,
		const char* start_key,size_t start_key_len,
		const char* limit_key,size_t limit_key_len);

extern void leveldb_destroy_db(
		const leveldb_options_t* options,
		const char* name,
		char **errptr);

extern void leveldb_repair_db(
		const leveldb_options_t* options,
		const char* name,
		char** errptr);

/* Iterator */
extern void leveldb_iter_destory(leveldb_iterator_t *);
extern unsigned char leveldb_iter_vaild(const leveldb_iterator_t*);
extern void leveldb_iter_seek_to_first(leveldb_iterator_t*);
extern void leveldb_iter_seek_to_last(leveldb_iterator_t*);
extern void leveldb_iter_seek(leveldb_iterator_t*,const char* k, size_t klen);
extern void leveldb_iter_next(leveldb_iterator_t*);
extern void leveldb_iter_prev(leveldb_iterator_t*);
extern const char* leveldb_iter_key(const leveldb_iterator_t*,size_t* klen);
extern const char* leveldb_iter_value(const leveldb_iterator_t*,size_t* vlen);
extern void leveldb_iter_get_error(const leveldb_iterator_t*, char** errptr);

/*write batch*/

extern leveldb_writebatch_t* leveldb_writebatch_create();
extern void leveldb_writebatch_destory(leveldb_writebatch_t*);
extern void leveldb_writebatch_clear(leveldb_writebatch_t*);
extern void leveldb_writebatch_put(
		leveldb_writebatch_t*,
		const char* key,size_t klen,
		const char* val,size_t vlen);
extern void leveldb_writebatch_delete(
		leveldb_writebatch_t*,
		const char* key, size_t klen);

extern void leveldb_writebatch_iterate(
		leveldb_writebatch_t*,
		void* state,
		void (*puth)(void*, const char* k, size_t klen, const char* v, size_t vlen),
		void (*deleted)(void*, const char* k, size_t klen));

/*Options */
extern leveldb_options_t* leveldb_options_create();
extern void leveldb_options_destory(leveldb_options_t*);
extern void leveldb_options_set_comparator(
		leveldb_options_t*,
		leveldb_comparator_t*);
extern void leveldb_options_set_filter_policy(
		leveldb_options_t*,
		leveldb_filterpolicy_t*);

extern void leveldb_options_set_create_if_missing(
		leveldb_options_t*,unsigned char);
extern void levldb_options_set_error_if_exists(
		leveldb_options_t*,unsigned char);
extern void leveldb_options_set_paranoid_checks(
		leveldb_options_t*,unsigned char);
extern void leveldb_options_set_env(leveldb_options_t*,leveldb_env_t*);
extern void leveldb_options_set_info_log(leveldb_options_t*, leveldb_logger_t*);
extern void leveldb_options_set_write_buffer_size(leveldb_options_t*, size_t);
extern void leveldb_options_set_max_open_files(leveldb_options_t*,int);
extern void leveldb_options_set_cache(leveldb_options_t*,leveldb_cache_t*);
extern void leveldb_options_set_block_size(leveldb_options_t*,size_t);
extern void leveldb_options_set_block_restart_interval(leveldb_options_t*,int);

enum{
	leveldb_no_compression = 0,
	leveldb_snappy_compression = 1,
};
extern void leveldb_options_set_compression(leveldb_options_t*, int);

extern leveldb_comparator_t* leveldb_comparator_create(
		void *state,
		void *(desturctor)(void*),
		int  (*compare)(
			void*,
			const char* a, size_t alen,
			const char* b, size_t blen),
			const char* (*name)(void*) );
extern void leveldb_comparator_destory(leveldb_comparator_t*);
/*Filter policy*/
extern leveldb_filterpolicy_t* leveldb_filterpolicy_create(
		void* state,
		void (*destructor)(void*),
		char* (*cxreate_filter)(
			void*,
			const char* const* key_array,
			const size_t* key_length_array,
			int num_keys,
			size_t* filter_length),
		unsigned char (*key_may_match)(
			void*,
			const char* key, size_t length,
			const char* filter, size_t filter_length),
		const char *(*name)(void*));
extern void leveldb_filterpolicy_destory(leveldb_filterpolicy_t*);

extern leveldb_filterpolicy_t* leveldb_filterpolicy_create_bloom(
		int bits_pre_key);

extern leveldb_readoptions_t* leveldb_readoptions_create();
extern void leveldb_readoptions_destory(leveldb_options_t*);
extern void leveldb_readoptions_set_verify_checksum(
		leveldb_readoptions_t*,
		unsigned char);
extern void leveldb_readoptions_set_fill_cache(
		leveldb_readoptions_t*,
		const leveldb_snapshot_t*
		);

extern leveldb_writeoptions* leveldb_writeoptions_create();
extern void leveldb_writeoptions_destory(leveldb_writeoptions_t*);
extern void leveldb_writeoptions_set_sync(
		leveldb_writeoptions_t*,unsigned char);

extern leveldb_cache_t* leveldb_cache_create_lru(size_t capacity);
extern void leveldb_cache_destory(leveldb_cache_t* cache);

extern leveldb_env_t* leveldb_create_default_env();
extern void leveldb_env_destory(leveldb_env_t *);

extern void leveldb_free( void* ptr );
extern int leveldb_major_version();
extern int leveldb_minor_version();

#ifdef _cplusplus
}
#endif


#endif /* end of include guard: STORAGE_LEVELDB_INCLUDE_C_H_ */
