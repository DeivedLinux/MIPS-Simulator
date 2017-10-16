#ifndef CACHE_H_
#define CACHE_H_

#include <stdbool.h>
#include "CacheConfig.h"


#ifndef BLOCK_SIZE
	#define BLOCK_SIZE 16 Bytes
#endif


#if CACHE_LEVEL == SINGLE_LEVEL
	#define CACHE_LEVELS 1
    #define CACHE_SIZE 128 KB

#elif CACHE_LEVEL == MULT_LEVEL
	#define CACHE_LEVELS 3u
	#define CACHE_LEVEL1_SIZE 128  KB
	#define CACHE_LEVEL2_SIZE 1024 KB
	#define CACHE_LEVEL3_SIZE 2048 KB
#else
	#define CACHE_LEVELS 1
	#define CACHE_LEVED1_SIZE 128 KB
#endif


typedef enum _Mapping
{
	Direct = 0,
	Associativity,
	TotalAssociativity
}*Mapping;

typedef enum _Policy
{
	Lru = 0,
	Random
}*Policy;


typedef union
{
	unsigned int address;

	struct 
	{
		unsigned offsetByte:3;
		unsigned offsetBlock:3;
		unsigned indice:8;
		unsigned tag:18	
	}bits;
}Address;

typedef struct _Cache
{
	
	struct _Row
	{
		void *block;
		unsigned tag;
		bool isValid;
	}__attribute__((aligned))*Row;

	bool cacheMiss;
	bool cacheHit;
	unsigned long long missCounter;
	unsigned long long hitCounter;
	void*(*readBlock)(Address address, struct _Cache*);
	void(*writeBlock)(Address address, enum _Policy ,struct _Cache*);
}*Cache;


extern Cache cache[CACHE_LEVELS];

bool initCache();


#endif