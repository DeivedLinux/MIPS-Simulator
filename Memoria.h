/*
 * Memoria.h
 *
 *  Created on: 10/05/2017
 *      Author: Deived William Jal Windows
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_


typedef enum
{
	Byte = 0,
	Half,
	Word,
	Double
}TypeAlign;

typedef enum
{
	BigEndian = 0,
	LittleEndian
}ByteOrder;

typedef enum
{
	Load_Byte = 0,
	Load_Byteu,
	Load_HalfWord,
	Load_HalFWordu,
	Load_Word,
	Load_Word_Left,
	Load_Word_Right,
	Load_Double_Word
}LoadMode;

typedef enum
{
	Store_Word = 0,
	Store_Word_Left,
	Store_Word_Right,
	Store_Byte,
	Store_Half
}StoreMode;

typedef enum
{
	LoadC1_Word = 0,
	LoadC1_Double
}C1_LoadMode;

typedef enum
{
	StoreC1_Word = 0,
	StoreC1_Double
}C1_StoreMode;


typedef struct
{
	void *textBaseAddress;
	void *textLimitAddress;
	void *externBaseAddress;
	void *globalPointer;
	void *dataBaseAddress;
	void *heapBaseAddress;
	void *stackLimitAddress;
	void *stackPointer;
	void *stackBaseAddress;
	void *dataSegmentLimitAddress;
	void *userSpaceHighAddress;
	void *kTextBaseAddress;
	void *kernelBaseAddress;
	void *exceptionHandlerAddress;
	void *kernelTextLimitAddress;
	void *kdataBaseAddress;
	void *kernelDataSegmentLimitAddress;
	void *mmioBaseAddress;
	void *kernelSpaceBaseAddress;
	void *memoryMapLimitAddress;
	void *dataSegmentBaseAddress;
}__attribute__((aligned))MemoryMap;

/*****************Memory Organization*****************
 * .text
 * .data
 * .extern
 * ._data
 * .userSpace
 * .kernelText
 * .kernel
 * .exception
 * .kernelData
 * .kernelSegdata
 * .kernelSpace
 * .memoryLimit
 ****************************************************/


extern const unsigned _textBaseAddress;
extern const unsigned _textLimitAddress;
extern const unsigned _externBaseAddress;
extern const unsigned _globalPointer;
extern const unsigned _dataBaseAddress;
extern const unsigned _heapBaseAddress;
extern const unsigned _stackLimitAddress;
extern const unsigned _stackPointer;
extern const unsigned _stackBaseAddress;
extern const unsigned _dataSegmentLimitAddress;
extern const unsigned _userSpaceHighAddress;
extern const unsigned _kTextBaseAddress;
extern const unsigned _kernelBaseAddress;
extern const unsigned _exceptionHandlerAddress;
extern const unsigned _kernelTextLimitAddress;
extern const unsigned _kdataBaseAddress;
extern const unsigned _kernelDataSegmentLimitAddress;
extern const unsigned _mmioBaseAddress;
extern const unsigned _kernelSpaceBaseAddress;
extern const unsigned _memoryMapLimitAddress;
extern const unsigned _dataSegmentBaseAddress;


MemoryMap* InitMem(void);
void destroyMemory(MemoryMap* map);


#endif /* MEMORIA_H_ */
