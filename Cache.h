#ifndef CACHE_H_
#define CACHE_H_

#include <stdbool.h>

#define Bytes *1u

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 16 Bytes
#endif

#ifndef CACHE_LEVELS
#define CACHE_LEVELS
#endif

typedef enum _Mapping
{
	Direct = 0,
	Associativity,
	TotalAssociativity
}*Mapping;

typedef enum _Polity
{
	Lru,
	Random
}*Polity;

typedef struct _Cache
{
	struct 
	{
		void* array;
	}Block[BLOCK_SIZE];

	bool cacheMiss;
	bool cacheHit;
	unsigned long long missCounter;
	unsigned long long hitCounter;
	void*(*readBlock)(unsigned long long address, struct _Cache* cache);
	void(*writeBlock)(unsigned long long address, Subs,struct _Cache* cache);
}*Cache;





#endif
