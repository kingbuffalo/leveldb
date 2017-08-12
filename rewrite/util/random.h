/*
 1，要确保某个int数为正数： &0x7  ffff fff u (7个f)
 2, 线性同余法  seed * A % M  A-->16807  不知道为什么是这数
 *
 */

#ifndef STORAGE_LEVELDB_UTIL_RANDOM_H_
#define STORAGE_LEVELDB_UTIL_RANDOM_H_


#include <stdint.h>

namespace leveldb{
class Random{
	private:
		//变量名后缀加下划线
		uint32_t seed_;
	public:
		explicit Random(uint32_t s):seed( s & 0x7fffffffu ){
			if ( seed_ == 0 || seed_ == 2147483647L ){
				seed_ = 1;
			}
		}
		//这个函数跟标准库的，有什么好处？
		uint32_t Next(){
			static const uint32_t M = 2147483647L;
			static const uint32_t A = 16807;
			uint64_t product = seed_ * A;
			seed_ = static_cast<uint32_t>((product >> 31) + (product & M ));
			if ( seed_ > M ){
				seed -= M;
			}
			return seed_;
		}
		//在[0..n-1]区间
		uint32_t Unitform(int n){return Next() % n;}
		//多少分之一
		bool OneIn(int n){return (Next()%n) == 0}
		uint32_t Skewed(int max_log){
			return Unitform(1 << Unitform(max_log+1));
		}
};
}


#endif /* end of include guard: STORAGE_LEVELDB_UTIL_RANDOM_H_ */
