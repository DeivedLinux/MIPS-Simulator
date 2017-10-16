#include "Cache.h"
#include <stdlib.h>
#include <stdio.h>


static void* ReadBlock(Address address, struct _Cache* cache);
static void  WriteBlock(Address address, enum _Policy policy, struct _Cache* cache);
Cache cache[CACHE_LEVELS];

bool initCache()
{
	unsigned i;

	#if CACHE_LEVELS == SINGLE_LEVEL
		cache[0]->Row = calloc(CACHE_SIZE, sizeof(struct _Row));

	for(i = 0; i < CACHE_SIZE; i++)
		cache[0]->Row[i]->block = malloc(BLOCK_SIZE);
	#endif

	#if CACHE_LEVELS > 1
		cache[0]->Row = calloc(CACHE_SIZE, sizeof(struct _Row));

	for(i = 0; i < CACHE_SIZE; i++)
		cache[0]->Row[i]->block = malloc(BLOCK_SIZE);

	cache[1]->Row = calloc(CACHE_LEVEL2_SIZE, sizeof(struct _Row));

	for(i = 0; i < CACHE_SIZE; i++)
		cache[1]->Row[i]->block = malloc(CACHE_LEVEL2_SIZE);

	cache[2]->Row = calloc(CACHE_LEVEL3_SIZE, sizeof(struct _Row));

	for(i = 0; i < CACHE_SIZE; i++)
		cache[2]->Row[i]->block = malloc(CACHE_LEVEL3_SIZE);
	#endif

	cache->writeBlock = WriteBlock;
	cache->readBlock = ReadBlock;

	return true;


static void  WriteBlock(Address address, enum _Policy policy, struct _Cache* cache)
{

	switch(policy)
	{
		case Lru:


		break;

		case Random:


		break;

		default:
			break;
	}
}

static void* readBlock(Address address, struct _Cache* cache)
{
	if(cache->row[address.offsetByte].tag == address.tag)
	{
		
	}
}