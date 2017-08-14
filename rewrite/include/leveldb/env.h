#ifndef STORAGE_LEVELDB_INCLUDE_ENV_H_
#define STORAGE_LEVELDB_INCLUDE_ENV_H_


#include <string>
#include <vector>
#include <stdarg.h>
#include <stdint.h>
#include "leveldb/status.h"

namespace leveldb{
class FileLock;
class Logger;
class RandomAccessFile;
class SequentialFile;
class Slice;
class WritableFile;

class Env{
	public:
		Env(){}
		virtual ~Env();
		static Env* Default();
		virtual Status NewSequentialFile(const std::string& fname,SequentialFile** result) = 0;
		virtual Status NewRandomAccessFile(const std::string& fname, RandomAccessFile** result) = 0;
		virtual Status NewWritableFile(const std::string& fname,WritableFile** result) = 0;
		virtual Status NewAppendableFile(const std::string& fname,WritableFile** result ) = 0;

		virtual bool FileExists(const std::string& fname) = 0;
		virtual Status GetChildren(const std::string& dir, std::vector<std::string>* result) = 0;
		virtual Status DeleteFile(const std::string& fname) = 0;
		virtual Status CreateDir(const std::string& dirname) = 0;
		virtual Status DeleteDir(const std::string& dirname) = 0;
		virtual Status GetFileSize(const std::string& fname,uint64_t* file_size) = 0;
		virtual Status RenameFile(const std::string& src, const std::string& target) = 0;
		virtual Status LockFile(const std::string& fname,FileLock** lock)=0;
		virtual Status UnlockFile(FileLock* lock) = 0;
		virtual void Schedule(
				void(*function)(void* arg),
				void* arg)=0;
		virtual void StartThread(void (*function)(void* arg),void* arg) = 0;
		virtual Status GetTestDirectory(std::string* path) = 0;
		virtual Status NewLogger(const std::string& fname, Logger** result) = 0;
		virtual uint64_t NowMicros() = 0;
		virtual void SleepForMicroseconds(int mircos) = 0;
	private:
		Env(const Env&);
		void operator=(const Env&);
};

class SequentialFile{
	public:
		SequentialFile() {}
		virtual ~SequentialFile();
		virtual Status Read(size_t n, Slice* result, char* scratch) = 0;
		virtual Status Skip(uint64_t n) = 0;
	private:
		SequentialFile(const SequentialFile&);
		void operator=(const SequentialFile&);
};

class RandomAccessFile{
	public:
		RandomAccessFile(){}
		virtual ~RandomAccessFile();
		virtual Status Read(uint64_t offset, size_t n, Slice* result, char* scratch) const = 0;
	private:
		RandomAccessFile(const RandomAccessFile&);
		void operator=(const RandomAccessFile&);
};

class WritableFile{
	public:
		WritAbleFile(){}
		virtual ~WritAbleFile();
		virtual Status Append(const Slice& data) = 0;
		virtual Status Close() = 0;
		virtual Status Flush() = 0;
		virtual Status Sync() = 0;
	private:
		WritableFile(const WritableFile&);
		void operator=(const WritableFile&);
};

class Logger{
	public:
		Logger(){}
		virtual ~Logger();
		virtual void Logv(const char* format, va_list ap) = 0;
	private:
		Logger(const Logger&);
		void operator=(const Logger&);
};

class FileLock{
	public:
		FileLock(){}
		virtual ~FileLock();
	private:
		FileLock(const FileLock&);
		void operator=(const FileLock&);
};

extern void Log(Logger* info_log, const char* format, ...)
# if defined(__GNUC_) || defined(__clang__)
	__attribute__((__format__(__printf__,2,3)))
#endif

extern Status WriteStringToFile(Env* env, const Slice& data,
								const std::string& fname);
extern Status ReadFileToString(Env *env, const std::string& fname,
								std::string* data);

class EnvWrapper : public Env{
	public:
		explicit EnvWrapper(Env* t):target_(t){}
		virtual ~EnvWrapper();

		Env* target() const { return target_;}
		
		Status NewSequentialFile(const std::string& f, SequentialFile** r){
			return target_->NewSequentialFile(f,r);
		}
};

}


#endif /* end of include guard: STORAGE_LEVELDB_INCLUDE_ENV_H_ */
