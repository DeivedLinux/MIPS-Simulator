/*
 * Memoria.c
 *
 *  Created on: 10/05/2017
 *      Author: Deived William Jal Windows
 */

#include "Memoria.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "GenericTypeDefs.h"

#ifndef TAM_MEM
#define MEM_SIZE  33554432U
#endif

#ifndef MEMORY_CONFIG
const unsigned _textBaseAddress = 0x00000000u;
const unsigned _textLimitAddress = 0x00989680u;
const unsigned _externBaseAddress = 0x00000000u;
const unsigned _globalPointer = 0x00000000;
const unsigned _dataBaseAddress = 0x00000000;
const unsigned _heapBaseAddress = 0x00000000;
const unsigned _stackLimitAddress = 0x01B3B4C0;
const unsigned _stackPointer = 0x02000000u;
const unsigned _stackBaseAddress = 0x02000000;
const unsigned _dataSegmentLimitAddress = 0x00E4E1C0u;
const unsigned _userSpaceHighAddress = 0x00000000u;
const unsigned _kTextBaseAddress = 0x00E4E1C0u;
const unsigned _kernelBaseAddress = 0x00000000;
const unsigned _exceptionHandlerAddress = 0x00000000;
const unsigned _kernelTextLimitAddress = 0x001312D00u;
const unsigned _kdataBaseAddress = 0x001312D00u;
const unsigned _kernelDataSegmentLimitAddress = 0x017D7840;
const unsigned _mmioBaseAddress = 0x00000000;
const unsigned _kernelSpaceBaseAddress = 0x00000000;
const unsigned _memoryMapLimitAddress = 0x02000000u;
const unsigned _dataSegmentBaseAddress = 0x00989680u;
#endif



static void *memory = NULL;

MemoryMap *InitMem()
{

	MemoryMap *memMap = (MemoryMap*)calloc(1,sizeof(MemoryMap));
	#ifndef TAM_MEM
	  memory = calloc(MEM_SIZE,sizeof(unsigned));
	#else
	  memory = calloc(TAM_MEM,sizeof(unsigned));
	#endif
	if(memMap == null) return null;
	if(memory == NULL)
	{
		fprintf(stderr,"%s\n","Não foi possivel alocar memória principal");
		return 0;
	}

	memMap->dataBaseAddress = &(((unsigned*)memory)[_dataBaseAddress]);
	memMap->dataSegmentLimitAddress = &(((unsigned*)memory)[_dataSegmentLimitAddress]);
    memMap->exceptionHandlerAddress = &(((unsigned*)memory)[ _exceptionHandlerAddress]);
	memMap->externBaseAddress = &(((unsigned*)memory)[_externBaseAddress]);
	memMap->globalPointer = &(((unsigned*)memory)[_globalPointer]);
	memMap->heapBaseAddress = &(((unsigned*)memory)[_heapBaseAddress]);
	memMap->kTextBaseAddress = &(((unsigned*)memory)[ _kTextBaseAddress]);
	memMap->kdataBaseAddress = &(((unsigned*)memory)[_kdataBaseAddress]);
	memMap->kernelBaseAddress = &(((unsigned*)memory)[ _kernelBaseAddress]);
	memMap->kernelDataSegmentLimitAddress = &(((unsigned*)memory)[_kernelDataSegmentLimitAddress]);
	memMap->kernelSpaceBaseAddress = &(((unsigned*)memory)[_kernelSpaceBaseAddress]);
	memMap->kernelTextLimitAddress = &(((unsigned*)memory)[_kernelTextLimitAddress]);
	memMap->memoryMapLimitAddress = &(((unsigned*)memory)[_memoryMapLimitAddress]);
	memMap->mmioBaseAddress = &(((unsigned*)memory)[_mmioBaseAddress]);
	memMap->stackBaseAddress = &(((unsigned*)memory)[_stackBaseAddress]);
	memMap->stackLimitAddress = &(((unsigned*)memory)[_stackLimitAddress]);
	memMap->stackPointer = &(((unsigned*)memory)[_stackPointer]);
	memMap->textBaseAddress = &(((unsigned*)memory)[_textBaseAddress]);
	memMap->textLimitAddress = &(((unsigned*)memory)[_textLimitAddress]);
    memMap->dataSegmentBaseAddress = &(((unsigned*)memory)[_dataSegmentBaseAddress]);
	memMap->userSpaceHighAddress = &(((unsigned*)memory)[_userSpaceHighAddress]);

    return memMap;

}

void destroyMemory(MemoryMap* map)
{
	free(map);
	free(memory);
}

