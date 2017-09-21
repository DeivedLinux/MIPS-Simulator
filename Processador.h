/*
 * Processador.h
 *
 *  Created on: 10/05/2017
 *      Author: Deived William Jal Windows
 */

#ifndef PROCESSADOR_H_
#define PROCESSADOR_H_

#include "Memoria.h"
#include "Coprocessador.h"
#include "Instrucoes.h"
#include <stdio.h>
#include <stdbool.h>
#include "General_Purpose_Register.h"





typedef enum 
{
  PlayMode = 0x100,
  DebugMode
}RunMode;


typedef struct
{
	unsigned int pc;

	union
	{
		unsigned long wordu;
		long word;

		struct
		{
			int li;
			int hi;
		};
	};

}__attribute__((aligned(8)))SpecialRegisters;



typedef struct
{
   GPRegisters* _register;
   SpecialRegisters* sRegister;
   Coprocessor1* coprocessor1;
   Coprocessor0* coprocessor0;
}__attribute__((aligned))Processor;


Processor* newProcessor(void);

GenericInstruction fetchInstruction(unsigned pc);
void unitControl(Processor* processor, RunMode runMode);
void printProcessor(Processor* processor);
bool loader(FILE* file,const char* name);
void printMemoryText(void);
void printfMemorySegmentData(void);

bool loaderDataSegment(FILE* file, const char *name);

void load(GPRegisters* rt, GPRegisters rs,int offset, LoadMode loadMode);
void store(GPRegisters rt,GPRegisters rs,int offset, StoreMode storeMode);

void loadCoprocessador1(void* word,GPRegisters reg,int offset,C1_LoadMode loadMode);
void storeWordCoprocessador1(void* word, GPRegisters reg, int offset,C1_StoreMode storeMode);
void resetCPU(Processor* processor);



#endif /* PROCESSADOR_H_ */
