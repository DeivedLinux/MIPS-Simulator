#ifndef CACHE_H
#define CACHE_H

#include <stdbool.h>


typedef enum _Mapping
{
	Direct,
	associativity	
}*Mapping;

typedef struct _Row 
{
	void* block;
	unsigned tag;
	bool isValid;	
}*Row;

typedef struct _Cache
{
	Row* rows;
	unsigned size;
	bool isHit;
	bool isMiss;
	unsigned missCounter;
	unsigned hitCounter;	
}*Cache;






#endif