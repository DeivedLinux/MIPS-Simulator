/*
 * Processador.c
 *
 *  Created on: 11/05/2017
 *      Author: Deived William Jal Windows
 */

#include "Processador.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Instrucoes.h"
#include <string.h>
#include "GenericTypeDefs.h"
#include "LabelsRegisters.h"
#include <gtk/gtk.h>
#include <stdbool.h>

GtkWidget *regName[36];
GtkWidget *regNumber[33];
GtkWidget *regValue[36];

GtkWidget *fregName[33];
GtkWidget *fregValueFloat[33];
GtkWidget *fregValueDouble[33];

GtkWidget *nameC0[5];
GtkWidget *numberC0[5];
GtkWidget *valueC0[5];
GtkWidget *noteBookRegisters;
GtkWidget *gridMemoryText;


static void(*syscall[60])(Processor* processor);
static MemoryMap* memoryMap = null;


static void printInteger(Processor* processor);
static void printFloat(Processor* processor);
static void printDouble(Processor* processor);
static void printString(Processor* processor);
static void readInteger(Processor* processor);
static void readFloat(Processor* processor);
static void readDouble(Processor* processor);
static void readString(Processor* processor);
static void _allocateHeapMemory(Processor* processor);
static void exitExecution(Processor* processor);
static void printCharacter(Processor* processor);
static void readCharacter(Processor* processor);
static void openFile(Processor* processor);
static void readFromFile(Processor* processor);
static void writeToFile(Processor* processor);
static void closeFile(Processor* processor);
static void exitReturnValue(Processor* processor);
static void timeSysT(Processor* processor);
static void midiOut(Processor* processor);
static void sleep(Processor* processor);
static void printHexa(Processor* processor);
static void printBinary(Processor* processor);
static void printUnsigned(Processor* processor);
static void setSeed(Processor* processor);
static void randomInt(Processor* processor);
static void randomIntRange(Processor* processor);
static void randomFloat(Processor* processor);
static void randomDouble(Processor* processor);
static void confirmDialog(Processor* processor);
static void inputDialogInt(Processor* processor);
static void inputDialogFloat(Processor* processor);
static void inputDialogDouble(Processor* processor);
static void inputDialogString(Processor* processor);
static void messageDialog(Processor* processor);
static void messageDialogInt(Processor* processor);
static void messageDialogFloat(Processor* processor);
static void messageDialogDouble(Processor* processor);
static void messageDialogString(Processor* processor);

void resetCPU(Processor* processor)
{
	unsigned i;

	processor->sRegister->pc = 0;
	processor->sRegister->wordu = 0;

	for(i = 0; i < 32; i++)
	   processor->_register[i].wordu = 0;


}

static void printInteger(Processor* processor)
{
    printf("%i\n",processor->_register[A0].word);
}

static void printFloat(Processor* processor)
{
	printf("%f\n",processor->coprocessor1->_register[f12]);
}
static void printDouble(Processor* processor)
{
	printf("%lf\n",processor->coprocessor1->_register[f12]);
}

static void printString(Processor* processor)
{
   unsigned i;
   unsigned temp;

   temp = processor->_register[A0].word;
   processor->_register[A0].ptr = memoryMap->dataSegmentBaseAddress; //+ ((temp | _dataSegmentBaseAddress) ^ _dataSegmentBaseAddress);

   for(i = 0;*(processor->_register[A0].ptr) !=0; i++)
   {
	   printf("%c",*(processor->_register[A0].ptr));
	   processor->_register[A0].word += 1;


   }
}
static void readInteger(Processor* processor)
{
   scanf("%i",&processor->_register[V0].word);
}

static void readFloat(Processor* processor)
{
   scanf("%f",&processor->coprocessor1->_register[f12]);
}
static void readDouble(Processor* processor)
{
   scanf("%lf",(double*)&processor->coprocessor1[12]);
}
static void readString(Processor* processor)
{
	unsigned temp;
	temp = processor->_register[A0].word;
	processor->_register[A0].ptr = (memoryMap->dataSegmentBaseAddress); //+ ((temp|_dataSegmentBaseAddress) ^ _dataSegmentBaseAddress);

  printf("0x%08x\n",processor->_register[A0].word);
  for(int i = 0; i < (processor->_register[A1].word); i++)
  {
    scanf("%c",((char*)processor->_register[A0].ptr));
    processor->_register[A0].word += 1;
  }

}

static void _allocateHeapMemory(Processor* processor)
{

}
static void exitExecution(Processor* processor)
{
	  printProcessor(processor);
	  destroyMemory(memoryMap);
    free(processor->_register);
    free(processor->coprocessor1->_register);
    free(processor->sRegister);
}

static void printCharacter(Processor* processor)
{
   printf("%c\n", (unsigned char)(processor->_register[A0].acess.LB));
}
static void readCharacter(Processor* processor)
{
	scanf("%c",&processor->_register[V0].acess.LB);
}
static void openFile(Processor* processor)
{

}
static void readFromFile(Processor* processor)
{

}
static void closeFile(Processor* processor)
{

}

static void writeToFile(Processor* processor)
{

}
static void exitReturnValue(Processor* processor)
{

}
static void timeSysT(Processor* processor)
{


}
static void midiOut(Processor* processor)
{


}
static void sleep(Processor* processor)
{

}

static void printHexa(Processor* processor)
{
   fprintf(stdin,"0x%08x\n",processor->_register[A0].wordu);
}
static void printBinary(Processor* processor)
{


}

static void printUnsigned(Processor* processor)
{
   fprintf(stdin,"%u\n",processor->_register[A0].wordu);
}
static void setSeed(Processor* processor)
{

}
static void randomInt(Processor* processor)
{
    processor->_register[A0].word = rand();
}
static void randomIntRange(Processor* processor)
{
    processor->_register[A0].word = rand() % processor->_register[A1].word;
}
static void randomFloat(Processor* processor)
{
   processor->coprocessor1->_register[f0] = (float)1.0f/(rand()% 1000) + 0.1f;
}
static void randomDouble(Processor* processor)
{
   ((double*)processor->coprocessor1->_register)[f0] = (double)1.0/(rand() % 1000)+ 0.1f;
}
static void confirmDialog(Processor* processor)
{

}
static void inputDialogInt(Processor* processor)
{

}
static void inputDialogFloat(Processor* processor)
{

}
static void inputDialogDouble(Processor* processor)
{

}
static void inputDialogString(Processor* processor)
{

}
static void messageDialog(Processor* processor)
{

}
static void messageDialogInt(Processor* processor)
{

}
static void messageDialogFloat(Processor* processor)
{

}
static void messageDialogDouble(Processor* processor)
{

}
static void messageDialogString(Processor* processor)
{

}

bool loaderDataSegment(FILE* file, const char *name)
{
	unsigned i = 0;
	unsigned temp = 0;
	file = fopen(name,"rb");

	if(file == NULL)
	{
		return false;
	}
	while(1)
	{
		fread(&temp,sizeof(unsigned),1,file);
		if(temp == 0) return true;
		else
		{
			((unsigned*)memoryMap->dataSegmentBaseAddress)[i] = temp;
			i++;
		}
	}

	return true;
}

bool loader(FILE* file,const char* name)
{
	GenericInstruction generic;
	unsigned i = 0;

    file = fopen(name,"rb");

    if(file == NULL)
    {
    	return false;
    }

    while(1)
    {
    	fread(&generic.word,sizeof(generic.word),1,file);
    	if(feof(file)) return true;
    	 else
    	 {
           ((GenericInstruction*)memoryMap->textBaseAddress)[i] = generic;
            i++;
    	 }
    }
    return true;
}
void printMemoryText(void)
{
	unsigned i;

	for(i = 0; i < 10; i++)
		printf("0x%08x\n", ((GenericInstruction*)memoryMap->textBaseAddress)[i].word);

}

void printfMemorySegmentData(void)
{
	unsigned i;
	for(i = 0; i < 10; i++)
		printf("0x%08x\n",((unsigned*)memoryMap->dataSegmentBaseAddress)[i]);

}
Processor* newProcessor(void)
{
  unsigned i;
  Processor* processor;

  processor = (Processor*)malloc(sizeof(Processor));
  processor->coprocessor0 = (Coprocessor0*)malloc(sizeof(Coprocessor0));
  processor->coprocessor1 = (Coprocessor1*)malloc(sizeof(Coprocessor1));


  processor->_register = (GPRegisters*)calloc(32,sizeof(GPRegisters));
  processor->coprocessor1->_register = (float*)calloc(32,sizeof(float));



  processor->sRegister = (SpecialRegisters*)malloc(sizeof(SpecialRegisters));
  processor->sRegister->pc = 0;


  for(i = 0; i < 32; i++)
  {
	processor->_register[i].wordu = 0;
	processor->coprocessor1->_register[i] = 0.0f;
  }
  processor->_register[Sp].ptr = ((int*)(memoryMap->stackLimitAddress));
  processor->coprocessor0->vaddr = 0x00000000u;
  processor->coprocessor0->cause = 0x00000000u;
  processor->coprocessor0->epc = 0x00000000u;
  processor->coprocessor0->status = 0x0000FF11u;

  processor->sRegister->hi = 0;
  processor->sRegister->li = 0;

  syscall[1] = printInteger;
  syscall[2] = printFloat;
  syscall[3] = printDouble;
  syscall[4] = printString;
  syscall[5] = readInteger;
  syscall[6] = readFloat;
  syscall[7] = readDouble;
  syscall[8] = readString;
  syscall[9] = _allocateHeapMemory;
  syscall[10] =  exitExecution;
  syscall[11] =  printCharacter;
  syscall[12] =  readCharacter;
  syscall[13] =  openFile;
  syscall[14] =  readFromFile;
  syscall[15] =  writeToFile;
  syscall[16] =  closeFile;
  syscall[17] =  exitReturnValue;
  syscall[18] =  timeSysT;
  syscall[19] =  midiOut;
  syscall[20] =  sleep;
  syscall[21] =  printHexa;
  syscall[22] =  printBinary;
  syscall[23] =  printUnsigned;
  syscall[24] =  setSeed;
  syscall[25] =  randomInt;
  syscall[26] =  randomIntRange;
  syscall[27] =  randomFloat;
  syscall[28] =  randomDouble;
  syscall[29] =  confirmDialog;
  syscall[30] =  inputDialogInt;
  syscall[31] =  inputDialogFloat;
  syscall[32] =  inputDialogDouble;
  syscall[33] =  inputDialogString;
  syscall[34] =  messageDialog;
  syscall[35] =  messageDialogInt;
  syscall[36] =  messageDialogFloat;
  syscall[37] =  messageDialogDouble;
  syscall[38] =  messageDialogString;





  return processor;
}
GenericInstruction fetchInstruction(unsigned pc)
{
    return ((GenericInstruction*)(memoryMap->textBaseAddress))[pc];
}

void unitControl(Processor* processor, RunMode runMode)
{
	GenericInstruction genericInstr;
	JTypeInstruction jTypeInst;
	RTypeInstruction rTypeInst;
  ITypeInstruction iTypeInst;
  FRTypeInstruction fRTypeInst;
  FITypeInstruction fITypeInst;
  int counter = 0;
  unsigned int temp;


  while(1)
  {
    if(runMode == DebugMode)
    {
      printProcessor(processor);
      while(getchar() != 'P');
    }
    genericInstr = fetchInstruction(processor->sRegister->pc);

    iTypeInst.wordu = genericInstr.wordu;
    rTypeInst.wordu = genericInstr.wordu;
    jTypeInst.word = genericInstr.word;
    fRTypeInst.word = genericInstr.word;
    fITypeInst.word = genericInstr.word;



    switch(genericInstr.acess.op)
    {

      case R_OpProcessor: // instruções tipo R

    	  switch(rTypeInst.acess.funct)
    	  {
    	     case SYSCALL:
            if(processor->_register[V0].word == 10) goto fim;
          
    	       syscall[processor->_register[V0].word](processor);
            
    	       processor->sRegister->pc += 1;

    	    	 break;

    	     case ADD:

    	    	 add(processor->_register[rTypeInst.acess.rd],processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case ADDU:

    	    	 addu(processor->_register[rTypeInst.acess.rd],processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case SUB:

    	    	 sub(processor->_register[rTypeInst.acess.rd],processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case SUBU:

    	    	 subu(processor->_register[rTypeInst.acess.rd],processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
    	    	 processor->sRegister->pc += 1;

    	    	 break;

           case MULT:

              processor->sRegister->word = mult(processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
              processor->sRegister->pc += 1;

              break;
           case MULTU:

              processor->sRegister->wordu = multu(processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
              processor->sRegister->pc += 1;
              break;

    	     case DIV:

    	    	 processor->sRegister->li = _div(processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
    	    	 processor->sRegister->hi = processor->_register[rTypeInst.acess.rs].word % processor->_register[rTypeInst.acess.rt].word;

    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case DIVU:

    	    	 processor->sRegister->li = divu(processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
    	    	 processor->sRegister->hi = processor->_register[rTypeInst.acess.rs].word % processor->_register[rTypeInst.acess.rt].word;

    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case SLT:

    	    	 slt(processor->_register[rTypeInst.acess.rd],processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case SLTU:

    	    	 sltu(processor->_register[rTypeInst.acess.rd],processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case AND:

    	    	 and(processor->_register[rTypeInst.acess.rd],processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case OR:

    	    	 or(processor->_register[rTypeInst.acess.rd],processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case NOR:

    	    	 nor(processor->_register[rTypeInst.acess.rd],processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case XOR:

    	    	 xor(processor->_register[rTypeInst.acess.rd],processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case JR:

    	    	  processor->sRegister->pc = processor->_register[rTypeInst.acess.rs].wordu;

    	    	 break;

    	     case JALR:

    	    	 processor->_register[rTypeInst.acess.rd].wordu = processor->sRegister->pc + 1;
    	    	 processor->sRegister->pc = processor->_register[rTypeInst.acess.rs].wordu;

    	    	 break;

    	     case SLL:

    	    	 if(rTypeInst.acess.shamt > 0)
    	    		 processor->_register[rTypeInst.acess.rd].wordu = processor->_register[rTypeInst.acess.rt].wordu <<  rTypeInst.acess.shamt;
    	    	 else  nop();

    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case MFLO:

    	    	 processor->_register[rTypeInst.acess.rd].word = processor->sRegister->li;
    	         processor->sRegister->pc += 1;

    	    	 break;

    	     case MFHI:

    	    	 processor->_register[rTypeInst.acess.rd].word = processor->sRegister->hi;
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case SLLV:

    	    	 processor->_register[rTypeInst.acess.rd].wordu = processor->_register[rTypeInst.acess.rt].wordu << processor->_register[rTypeInst.acess.rs].wordu;
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case SRA:

    	    	 processor->_register[rTypeInst.acess.rd].word = processor->_register[rTypeInst.acess.rt].word >> rTypeInst.acess.shamt;
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case SRAV:

    	    	 processor->_register[rTypeInst.acess.rd].word = processor->_register[rTypeInst.acess.rt].word >> processor->_register[rTypeInst.acess.rs].word;
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case SRLV:

    	    	 processor->_register[rTypeInst.acess.rd].word = processor->_register[rTypeInst.acess.rt].wordu >> processor->_register[rTypeInst.acess.rs].wordu;
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case SRL:

    	    	 processor->_register[rTypeInst.acess.rd].wordu = processor->_register[rTypeInst.acess.rt].wordu >> rTypeInst.acess.shamt;
    	    	 processor->sRegister->pc += 1;

    	    	 break;

    	     case MOVN:

    	    	 if(processor->_register[rTypeInst.acess.rt].word > 0)
    	    		 processor->_register[rTypeInst.acess.rd] = processor->_register[rTypeInst.acess.rs];

    	    	     processor->sRegister->pc += 1;

    	    	 break;

    	     default:
    	    	 break;
    	  }

         /*Intruções IType or JType*/
             break;
         /*
          case R_OpProcessor_mode2:

                switch(rTypeInst.acess.funct)
                {
                   case MUL:
                            mul(processor->sRegister->word,processor->_register[rTypeInst.acess.rd],processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
                            processor->sRegister->pc += 1;
                   break;

                   case MADD:

                            madd(processor->sRegister->word,processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
                            processor->sRegister->pc += 1;

                   break;

                   case MADDU:

                            maddu(processor->sRegister->wordu,processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
                            processor->sRegister->pc += 1;

                   break;

                   case MSUB:

                            msub(processor->sRegister->word,processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
                            processor->sRegister->pc += 1;

                   break;

                   case MSUBU:

                            maddu(processor->sRegister->word,processor->_register[rTypeInst.acess.rs],processor->_register[rTypeInst.acess.rt]);
                            processor->sRegister->pc += 1;

                   break;

                   case CLZ:

                            
                              for(int i = 0; i < 32; i++)
                              {
                                if(((processor->_register[rTypeInst.acess.rs].word >> i)&0x00000001) == 0)
                                {
                                  counter += 1;
                                }
                              }
                              processor->_register[rTypeInst.acess.rd].word = counter;

                   break;

                   case CLO:

                            
                              for(int i = 0; i < 32; i++)
                              {
                                if(((processor->_register[rTypeInst.acess.rs].word >> i)&0x00000001) == 1)
                                {
                                  counter += 1;
                                }
                              }
                              processor->_register[rTypeInst.acess.rd].word = counter;

                   break;

                   default:
                         break;

                }

          break; 

         */
             case ADDI:

        	        addi(processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant);
        	        processor->sRegister->pc += 1;


        	        break;

             case ADDIU:


        	        addiu(processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant);
        	        processor->sRegister->pc += 1;
        	        break;

             case SLTI:

        	        slti(processor->_register[iTypeInst.acess.rs],processor->_register[iTypeInst.acess.rt],iTypeInst.acess.addressOrConstant);
        	        processor->sRegister->pc += 1;

        	        break;

             case SLTIU:

        	        sltiu(processor->_register[iTypeInst.acess.rs], processor->_register[iTypeInst.acess.rt],iTypeInst.acess.addressOrConstant);
        	        processor->sRegister->pc += 1;

        	        break;

             case ANDI:

        	        andi(processor->_register[iTypeInst.acess.rs],processor->_register[iTypeInst.acess.rt],iTypeInst.acess.addressOrConstant);
        	        processor->sRegister->pc += 1;

        	        break;

             case ORI:
                  

        	        ori(processor->_register[iTypeInst.acess.rs], processor->_register[iTypeInst.acess.rt], iTypeInst.acess.addressOrConstant);
        	        processor->sRegister->pc += 1;

        	        break;

             case XORI:

        	         xori(processor->_register[iTypeInst.acess.rs],processor->_register[iTypeInst.acess.rt], iTypeInst.acess.addressOrConstant);
        	         processor->sRegister->pc += 1;

        	         break;

             case LW:

                     load(&processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant/4,Load_Word);
                     processor->sRegister->pc += 1;

        	         break;

             case SW:

        	        store(processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant/4,Store_Word);
        	        processor->sRegister->pc += 1;

        	        break;

             case LBU:

        	        load(&processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant,Load_Byteu);
        	        processor->sRegister->pc += 1;

        	        break;

             case LB:

        	        load(&processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant,Load_Byte);
        	        processor->sRegister->pc += 1;

        	        break;

             case SB:
  
        	        store(processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant,Store_Byte);
        	        processor->sRegister->pc += 1;

        	        break;

             case SH:

        	        store(processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant,Store_Half);
        	        processor->sRegister->pc += 1;

        	        break;

             case LUI:

        	        processor->_register[iTypeInst.acess.rt].v[High_Order] = iTypeInst.acess.addressOrConstant;
        	        processor->sRegister->pc += 1;

        	        break;

             case LH:

        	    load(&processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant,Load_HalfWord);

        	        break;

             case LWL:

        	   load(&processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant/4,Load_Word_Left);

        	       break;

             case LHU:

        	        load(&processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant,Load_HalFWordu);
        	        processor->sRegister->pc += 1;

        	        break;

             case LWR:

        	        load(&processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant/4,Load_Word_Right);
        	        processor->sRegister->pc += 1;

        	        break;

             case BEQ:

        	      if(processor->_register[iTypeInst.acess.rs].word == processor->_register[iTypeInst.acess.rt].word)
      		      processor->sRegister->pc += iTypeInst.acess.addressOrConstant + 1;

        	      else  processor->sRegister->pc += 1;
        	        break;

             case BNE:

        	       if(processor->_register[iTypeInst.acess.rt].word != processor->_register[iTypeInst.acess.rs].word)
        		       processor->sRegister->pc += iTypeInst.acess.addressOrConstant + 1;

         	       else  processor->sRegister->pc += 1;

        	      break;

             case BLEZ:

        	      if(processor->_register[iTypeInst.acess.rs].word <= 0)
        		    processor->sRegister->pc += iTypeInst.acess.addressOrConstant + 1;

        	      else  processor->sRegister->pc += 1;;

        	      break;

             case BGTZ:

        	      if(processor->_register[iTypeInst.acess.rs].word > 0)
        		    processor->sRegister->pc += iTypeInst.acess.addressOrConstant + 1;

        	      else  processor->sRegister->pc += 1;

        	      break;

             case BLTZ:

        	    if(processor->_register[iTypeInst.acess.rs].word < 0)
        		      processor->sRegister->pc += iTypeInst.acess.addressOrConstant + 1;

        	    else  processor->sRegister->pc += 1;

        	      break;

             case LWC1:

        	      loadCoprocessador1(&processor->coprocessor1->_register[fITypeInst.acess.ft],processor->_register[fITypeInst.acess.rs],fITypeInst.acess.imm/4,LoadC1_Word);
        	      processor->sRegister->pc += 1;

        	      break;

             case SWC1:

                  storeWordCoprocessador1(&processor->coprocessor1->_register[fITypeInst.acess.ft],processor->_register[fITypeInst.acess.rs],fITypeInst.acess.imm/4,StoreC1_Word);
                  processor->sRegister->pc += 1;

        	      break;

             case LL:

            	 processor->sRegister->pc += 1;

        	     break;

             case SWL:

        	        store(processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant/4,Store_Word_Left);
        	        processor->sRegister->pc += 1;

        	        break;

             case SWR:

        	        store(processor->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant/4,Store_Word_Right);
        	        processor->sRegister->pc += 1;

        	         break;

             case SDC1:

        	     storeWordCoprocessador1(&processor->coprocessor1->_register[iTypeInst.acess.rt],processor->_register[iTypeInst.acess.rs],iTypeInst.acess.addressOrConstant/4,StoreC1_Double);
            	 processor->sRegister->pc += 1;

        	      break;

             case SC:

        	      break;

             case J:

        	       processor->sRegister->pc = (processor->sRegister->pc & 0xF0000000)+ jTypeInst.acess.address;

        	      break;

             case JAL:

        	       processor->_register[Ra].wordu = processor->sRegister->pc + 1;
        	       processor->sRegister->pc = (processor->sRegister->pc & 0xF0000000) + jTypeInst.acess.address;

        	 break;

             case R_OpCoprocessor1:

            	 switch(fRTypeInst.acess.funct)
            	 {

            	     case ADDf:

            	    	 if(fRTypeInst.acess.fmt == SinglePrecision)
            	    	 {
                           add_s(processor->coprocessor1->_register[fRTypeInst.acess.ft],  \
                        		 processor->coprocessor1->_register[fRTypeInst.acess.fs],  \
                        		 processor->coprocessor1->_register[fRTypeInst.acess.fd]);

            	    	 }else if(fRTypeInst.acess.fmt == DoublePrecision)
            	    	 {
                           add_d(processor->coprocessor1->_register,fRTypeInst.acess.ft,
                        		 processor->coprocessor1->_register,fRTypeInst.acess.fs,
                        		 processor->coprocessor1->_register,fRTypeInst.acess.fd);
            	    	 }

            	    	 processor->sRegister->pc += 1;
            	    	 break;

            	     case SUBf:

            	    	 if(fRTypeInst.acess.fmt == SinglePrecision)
            	    	 {
            	    		 sub_s(processor->coprocessor1->_register[fRTypeInst.acess.ft],     \
            	    		       processor->coprocessor1->_register[fRTypeInst.acess.fs],     \
            	    		       processor->coprocessor1->_register[fRTypeInst.acess.fd]);


            	    	 }else if(fRTypeInst.acess.fmt == DoublePrecision)
            	    	 {
            	    		 sub_d(processor->coprocessor1->_register,fRTypeInst.acess.ft,
            	    		       processor->coprocessor1->_register,fRTypeInst.acess.fs,
            	    		       processor->coprocessor1->_register,fRTypeInst.acess.fd);

            	    	 }

            	    	 processor->sRegister->pc += 1;

            	    	 break;

            	     case MULf:

            	    	 if(fRTypeInst.acess.fmt == SinglePrecision)
            	    	 {
            	    		 mul_s(processor->coprocessor1->_register[fRTypeInst.acess.ft],     \
            	    		       processor->coprocessor1->_register[fRTypeInst.acess.fs],     \
            	    		       processor->coprocessor1->_register[fRTypeInst.acess.fd]);

            	    	  }else if(fRTypeInst.acess.fmt == DoublePrecision)
            	    	  {
            	    		 mul_d(processor->coprocessor1->_register,fRTypeInst.acess.ft,
            	    		        processor->coprocessor1->_register,fRTypeInst.acess.fs,
            	    		        processor->coprocessor1->_register,fRTypeInst.acess.fd);

            	    	  }

            	    	  processor->sRegister->pc += 1;

            	    	 break;

            	     case DIVf:

            	    	  if(fRTypeInst.acess.fmt == SinglePrecision)
            	    	  {
            	    		  div_s(processor->coprocessor1->_register[fRTypeInst.acess.ft],     \
            	    		        processor->coprocessor1->_register[fRTypeInst.acess.fs],     \
            	    		        processor->coprocessor1->_register[fRTypeInst.acess.fd]);


            	    	  }else if(fRTypeInst.acess.fmt == DoublePrecision)
            	    	  {
            	    		  div_d(processor->coprocessor1->_register,fRTypeInst.acess.ft,
            	    		        processor->coprocessor1->_register,fRTypeInst.acess.fs,
            	    		        processor->coprocessor1->_register,fRTypeInst.acess.fd);

            	    	  }
            	    	  processor->sRegister->pc += 1;

            	    	 break;

                     

            	     default:
            	    	 break;

            	 }
            	break;

         default:
        	 fprintf(stderr, "%s: illegal instruction: %08x\n", __FUNCTION__, genericInstr.word);
        	 break;

     }
    }
    fim:
    printProcessor(processor);

  }


void printProcessor(Processor* processor)
{
	unsigned i;
  char v[10];

  itoa(processor->_register[0].word,v,10);

  gtk_label_set_text(GTK_LABEL(regValue[1]),v);

  itoa(processor->_register[1].word,v,10);

  gtk_label_set_text(GTK_LABEL(regValue[2]),v);

  itoa(processor->_register[2].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[3]),v);

  itoa(processor->_register[3].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[4]),v);

  itoa(processor->_register[4].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[5]),v);

  itoa(processor->_register[5].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[6]),v);

  itoa(processor->_register[6].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[7]),v);

  itoa(processor->_register[7].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[8]),v);

  itoa(processor->_register[8].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[9]),v);

  itoa(processor->_register[9].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[10]),v);

  itoa(processor->_register[10].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[11]),v);

  itoa(processor->_register[11].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[12]),v);

  itoa(processor->_register[12].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[13]),v);

  itoa(processor->_register[13].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[14]),v);

  itoa(processor->_register[14].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[15]),v);

  itoa(processor->_register[15].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[16]),v);

  itoa(processor->_register[16].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[17]),v);

  itoa(processor->_register[17].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[18]),v);

  itoa(processor->_register[18].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[19]),v);

  itoa(processor->_register[19].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[20]),v);

  itoa(processor->_register[20].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[21]),v);

  itoa(processor->_register[21].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[22]),v);

  itoa(processor->_register[22].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[23]),v);

  itoa(processor->_register[23].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[24]),v);

  itoa(processor->_register[24].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[25]),v);

  itoa(processor->_register[25].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[26]),v);

  itoa(processor->_register[26].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[27]),v);

  itoa(processor->_register[27].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[28]),v);

  itoa(processor->_register[28].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[29]),v);

  itoa(processor->_register[29].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[30]),v);

  itoa(processor->_register[30].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[31]),v);

  itoa(processor->_register[31].word,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[32]),v);

  itoa(processor->sRegister->pc,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[33]),v);

  itoa(processor->sRegister->li,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[34]),v);

  itoa(processor->sRegister->hi,v,10);
  gtk_label_set_text(GTK_LABEL(regValue[35]),v);

	gtk_widget_show_all(noteBookRegisters);
}


void loadCoprocessador1(void* word,GPRegisters rs,int offset,C1_LoadMode loadMode)
{
	unsigned temp;

    temp = rs.wordu;
	 rs.ptr = (memoryMap->dataSegmentBaseAddress + (_dataSegmentBaseAddress ^ temp));

    switch(loadMode)
    {
      case LoadC1_Word:

    	  *((float*)word) = *(((float*)rs.ptr)+offset);

    	  break;

      case LoadC1_Double:

    	  *((double*)word) = *(((double*)rs.ptr)+offset);

    	  break;

      default:
    	  break;
    }
}
void storeWordCoprocessador1(void* word, GPRegisters rs, int offset,C1_StoreMode storeMode)
{
	unsigned temp;

	temp = rs.wordu;
	rs.ptr = (memoryMap->dataSegmentBaseAddress + (_dataSegmentBaseAddress ^ temp));

	switch(storeMode)
	{
	  case StoreC1_Word:

		  ((float*)rs.ptr)[offset] = *((float*)word);

		  break;

	  case StoreC1_Double:

		  ((double*)rs.ptr)[offset] = *((double*)word);

		  break;

	}

}

void load(GPRegisters* rt, GPRegisters rs,int offset,LoadMode loadMode)
{
    unsigned temp;

	temp = rs.wordu;
	rs.ptr = (memoryMap->dataSegmentBaseAddress + ((_dataSegmentBaseAddress|temp)^_dataSegmentBaseAddress));

	switch(loadMode)
	{
	   case Load_Byte:

		   rt->word = ((signed char*)rs.ptr)[offset];
		   break;

	   case Load_Byteu:

		   rt->word = ((unsigned char*)rs.ptr)[offset];

		   		   break;

	   case Load_HalfWord:

		   rt->word = ((signed short*)rs.ptr)[offset];

		   		   break;
	   case Load_HalFWordu:

		   rt->word = ((unsigned short*)rs.ptr)[offset];

		   		   break;
	   case Load_Word:

		   rt->word= ((signed int*)rs.ptr)[offset];

		   		   break;

	   case Load_Word_Left:

		   rt->v[High_Order] = (((signed int*)rs.ptr)[offset]);

		   		   break;

	   case Load_Word_Right:

		   rt->v[Low_Order] = ((signed int*)rs.ptr)[offset];

		   		   break;
	   default:
		   break;

	}
}


void store(GPRegisters rt,GPRegisters rs,int offset, StoreMode storeMode)
{
	unsigned temp;

	temp = rs.wordu;

	rs.ptr = (memoryMap->dataSegmentBaseAddress + ((_dataSegmentBaseAddress|temp)^_dataSegmentBaseAddress));


	switch(storeMode)
	{
	  case Store_Word:

           ((int*)rs.ptr)[offset] = rt.word;

		  break;
	  case Store_Word_Left:

		   ((int*)rs.ptr)[offset] = rt.v[High_Order];

		  break;
	  case Store_Word_Right:

		  ((int*)rs.ptr)[offset] = rt.v[Low_Order];
		  break;

	  case Store_Byte:

		  ((char*)rs.ptr)[offset] = rt.acess.LB;
		  break;

	  case Store_Half:

		  ((short*)rs.ptr)[offset] = rt.v[Low_Order];

		  break;

	  default:
		  break;
	}
}

void LabelsInit(void)
{

  regName[0] = gtk_label_new("Nome");
  regName[1] = gtk_label_new("$zero");
  regName[2] = gtk_label_new("$at");
  regName[3] = gtk_label_new("$v0");
  regName[4] = gtk_label_new("$v1");
  regName[5] = gtk_label_new("$a0");
  regName[6] = gtk_label_new("$a1");
  regName[7] = gtk_label_new("$a2");
  regName[8] = gtk_label_new("$a3");
  regName[9] = gtk_label_new("$t0");
  regName[10] = gtk_label_new("$t1");
  regName[11] = gtk_label_new("$t2");
  regName[12] = gtk_label_new("$t3");
  regName[13] = gtk_label_new("$t4");
  regName[14] = gtk_label_new("$t5");
  regName[15] = gtk_label_new("$t6");
  regName[16] = gtk_label_new("$t7");
  regName[17] = gtk_label_new("$s0");
  regName[18] = gtk_label_new("$s1");
  regName[19] = gtk_label_new("$s2");
  regName[20] = gtk_label_new("$s3");
  regName[21] = gtk_label_new("$s4");
  regName[22] = gtk_label_new("$s5");
  regName[23] = gtk_label_new("$s6");
  regName[24] = gtk_label_new("$s7");
  regName[25] = gtk_label_new("$t8");
  regName[26] = gtk_label_new("$t9");
  regName[27] = gtk_label_new("$k0");
  regName[28] = gtk_label_new("$k1");
  regName[29] = gtk_label_new("$gp");
  regName[30] = gtk_label_new("$sp");
  regName[31] = gtk_label_new("$fp");
  regName[32] = gtk_label_new("$ra");
  regName[33] = gtk_label_new("PC");
  regName[34] = gtk_label_new("HI");
  regName[35] = gtk_label_new("LO");

  regNumber[0] = gtk_label_new("Número");
  regNumber[1] = gtk_label_new("0");
  regNumber[2] = gtk_label_new("1");
  regNumber[3] = gtk_label_new("2");
  regNumber[4] = gtk_label_new("3");
  regNumber[5] = gtk_label_new("4");
  regNumber[6] = gtk_label_new("5");
  regNumber[7] = gtk_label_new("6");
  regNumber[8] = gtk_label_new("7");
  regNumber[9] = gtk_label_new("8");
  regNumber[10] = gtk_label_new("9");
  regNumber[11] = gtk_label_new("10");
  regNumber[12] = gtk_label_new("11");
  regNumber[13] = gtk_label_new("12");
  regNumber[14] = gtk_label_new("13");
  regNumber[15] = gtk_label_new("14");
  regNumber[16] = gtk_label_new("15");
  regNumber[17] = gtk_label_new("16");
  regNumber[18] = gtk_label_new("17");
  regNumber[19] = gtk_label_new("18");
  regNumber[20] = gtk_label_new("19");
  regNumber[21] = gtk_label_new("20");
  regNumber[22] = gtk_label_new("21");
  regNumber[23] = gtk_label_new("22");
  regNumber[24] = gtk_label_new("23");
  regNumber[25] = gtk_label_new("24");
  regNumber[26] = gtk_label_new("25");
  regNumber[27] = gtk_label_new("26");
  regNumber[28] = gtk_label_new("27");
  regNumber[29] = gtk_label_new("28");
  regNumber[30] = gtk_label_new("29");
  regNumber[31] = gtk_label_new("30");
  regNumber[32] = gtk_label_new("31");

  regValue[0] = gtk_label_new("Valor");
  regValue[1] = gtk_label_new("0");
  regValue[2] = gtk_label_new("0");
  regValue[3] = gtk_label_new("0");
  regValue[4] = gtk_label_new("0");
  regValue[5] = gtk_label_new("0");
  regValue[6] = gtk_label_new("0");
  regValue[7] = gtk_label_new("0");
  regValue[8] = gtk_label_new("0");
  regValue[9] = gtk_label_new("0");
  regValue[10] = gtk_label_new("0");
  regValue[11] = gtk_label_new("0");
  regValue[12] = gtk_label_new("0");
  regValue[13] = gtk_label_new("0");
  regValue[14] = gtk_label_new("0");
  regValue[15] = gtk_label_new("0");
  regValue[16] = gtk_label_new("0");
  regValue[17] = gtk_label_new("0");
  regValue[18] = gtk_label_new("0");
  regValue[19] = gtk_label_new("0");
  regValue[20] = gtk_label_new("0");
  regValue[21] = gtk_label_new("0");
  regValue[22] = gtk_label_new("0");
  regValue[23] = gtk_label_new("0");
  regValue[24] = gtk_label_new("0");
  regValue[25] = gtk_label_new("0");
  regValue[26] = gtk_label_new("0");
  regValue[27] = gtk_label_new("0");
  regValue[28] = gtk_label_new("0");
  regValue[29] = gtk_label_new("0");
  regValue[30] = gtk_label_new("0");
  regValue[31] = gtk_label_new("0");
  regValue[32] = gtk_label_new("0");
  regValue[33] = gtk_label_new("0");
  regValue[34] = gtk_label_new("0");
  regValue[35] = gtk_label_new("0");

  fregName[0] = gtk_label_new("Nome");
  fregName[1] = gtk_label_new("$f0");
  fregName[2] = gtk_label_new("$f1");
  fregName[3] = gtk_label_new("$f2");
  fregName[4] = gtk_label_new("$f3");
  fregName[5] = gtk_label_new("$f4");
  fregName[6] = gtk_label_new("$f5");
  fregName[7] = gtk_label_new("$f6");
  fregName[8] = gtk_label_new("$f7");
  fregName[9] = gtk_label_new("$f8");
  fregName[10] = gtk_label_new("$f9");
  fregName[11] = gtk_label_new("$f10");
  fregName[12] = gtk_label_new("$f11");
  fregName[13] = gtk_label_new("$f12");
  fregName[14] = gtk_label_new("$f13");
  fregName[15] = gtk_label_new("$f14");
  fregName[16] = gtk_label_new("$f15");
  fregName[17] = gtk_label_new("$f16");
  fregName[18] = gtk_label_new("$f17");
  fregName[19] = gtk_label_new("$f18");
  fregName[20] = gtk_label_new("$f19");
  fregName[21] = gtk_label_new("$f20");
  fregName[22] = gtk_label_new("$f21");
  fregName[23] = gtk_label_new("$f22");
  fregName[24] = gtk_label_new("$f23");
  fregName[25] = gtk_label_new("$f24");
  fregName[26] = gtk_label_new("$f25");
  fregName[27] = gtk_label_new("$f26");
  fregName[28] = gtk_label_new("$f27");
  fregName[29] = gtk_label_new("$f28");
  fregName[30] = gtk_label_new("$f29");
  fregName[31] = gtk_label_new("$f30");
  fregName[32] = gtk_label_new("$f31");

  fregValueFloat[0] = gtk_label_new("Float");
  fregValueFloat[1] = gtk_label_new("0.0000");
  fregValueFloat[2] = gtk_label_new("0.0000");
  fregValueFloat[3] = gtk_label_new("0.0000");
  fregValueFloat[4] = gtk_label_new("0.0000");
  fregValueFloat[5] = gtk_label_new("0.0000");
  fregValueFloat[6] = gtk_label_new("0.0000");
  fregValueFloat[7] = gtk_label_new("0.0000");
  fregValueFloat[8] = gtk_label_new("0.0000");
  fregValueFloat[9] = gtk_label_new("0.0000");
  fregValueFloat[10] = gtk_label_new("0.0000");
  fregValueFloat[11] = gtk_label_new("0.0000");
  fregValueFloat[12] = gtk_label_new("0.0000");
  fregValueFloat[13] = gtk_label_new("0.0000");
  fregValueFloat[14] = gtk_label_new("0.0000");
  fregValueFloat[15] = gtk_label_new("0.0000");
  fregValueFloat[16] = gtk_label_new("0.0000");
  fregValueFloat[17] = gtk_label_new("0.0000");
  fregValueFloat[18] = gtk_label_new("0.0000");
  fregValueFloat[19] = gtk_label_new("0.0000");
  fregValueFloat[20] = gtk_label_new("0.0000");
  fregValueFloat[21] = gtk_label_new("0.0000");
  fregValueFloat[22] = gtk_label_new("0.0000");
  fregValueFloat[23] = gtk_label_new("0.0000");
  fregValueFloat[24] = gtk_label_new("0.0000");
  fregValueFloat[25] = gtk_label_new("0.0000");
  fregValueFloat[26] = gtk_label_new("0.0000");
  fregValueFloat[27] = gtk_label_new("0.0000");
  fregValueFloat[28] = gtk_label_new("0.0000");
  fregValueFloat[29] = gtk_label_new("0.0000");
  fregValueFloat[30] = gtk_label_new("0.0000");
  fregValueFloat[31] = gtk_label_new("0.0000");
  fregValueFloat[32] = gtk_label_new("0.0000");

  fregValueDouble[0] = gtk_label_new("Double");
  fregValueDouble[1] = gtk_label_new("0.0000");
  fregValueDouble[2] = gtk_label_new("0.0000");
  fregValueDouble[3] = gtk_label_new("0.0000");
  fregValueDouble[4] = gtk_label_new("0.0000");
  fregValueDouble[5] = gtk_label_new("0.0000");
  fregValueDouble[6] = gtk_label_new("0.0000");
  fregValueDouble[7] = gtk_label_new("0.0000");
  fregValueDouble[8] = gtk_label_new("0.0000");
  fregValueDouble[9] = gtk_label_new("0.0000");
  fregValueDouble[10] = gtk_label_new("0.0000");
  fregValueDouble[11] = gtk_label_new("0.0000");
  fregValueDouble[12] = gtk_label_new("0.0000");
  fregValueDouble[13] = gtk_label_new("0.0000");
  fregValueDouble[14] = gtk_label_new("0.0000");
  fregValueDouble[15] = gtk_label_new("0.0000");
  fregValueDouble[16] = gtk_label_new("0.0000");
  fregValueDouble[17] = gtk_label_new("0.0000");
  fregValueDouble[18] = gtk_label_new("0.0000");
  fregValueDouble[19] = gtk_label_new("0.0000");
  fregValueDouble[20] = gtk_label_new("0.0000");
  fregValueDouble[21] = gtk_label_new("0.0000");
  fregValueDouble[22] = gtk_label_new("0.0000");
  fregValueDouble[23] = gtk_label_new("0.0000");
  fregValueDouble[24] = gtk_label_new("0.0000");
  fregValueDouble[25] = gtk_label_new("0.0000");
  fregValueDouble[26] = gtk_label_new("0.0000");
  fregValueDouble[27] = gtk_label_new("0.0000");
  fregValueDouble[28] = gtk_label_new("0.0000");
  fregValueDouble[29] = gtk_label_new("0.0000");
  fregValueDouble[30] = gtk_label_new("0.0000");
  fregValueDouble[31] = gtk_label_new("0.0000");
  fregValueDouble[32] = gtk_label_new("0.0000");

  nameC0[0] = gtk_label_new("Nome");
  nameC0[1] = gtk_label_new("$8  vaddr");
  nameC0[2] = gtk_label_new("$12 status");
  nameC0[3] = gtk_label_new("$13 cause");
  nameC0[4] = gtk_label_new("$14 epc");

  numberC0[0] = gtk_label_new("Número");
  numberC0[1] = gtk_label_new("8");
  numberC0[2] = gtk_label_new("12");
  numberC0[3] = gtk_label_new("13");
  numberC0[4] = gtk_label_new("14");

  valueC0[0] = gtk_label_new("Valor");
  valueC0[1] = gtk_label_new("0x00000000");
  valueC0[2] = gtk_label_new("0x0000FF11");
  valueC0[3] = gtk_label_new("0x00000000");
  valueC0[4] = gtk_label_new("0x00000000");


}

bool loadMemoryFromVector(void* buffer, unsigned size)
{
  unsigned i;

   memoryMap = InitMem();
   char v[10];
   char end[10];

  if(memoryMap == NULL) return false;

  for(i = 0; i <= size; i++)
  {
     ((GenericInstruction*)(memoryMap->textBaseAddress))[i] = ((GenericInstruction*)buffer)[size - i];
  }
   gtk_grid_set_row_spacing(GTK_GRID(gridMemoryText),1);
   gtk_grid_set_column_spacing(GTK_GRID(gridMemoryText),80);


   gtk_grid_insert_column(GTK_GRID(gridMemoryText),0);
   gtk_grid_insert_column(GTK_GRID(gridMemoryText),1);
   gtk_grid_insert_column(GTK_GRID(gridMemoryText),2);

 
  for(i = 0; i <= size; i++)
  {
     gtk_grid_insert_row(GTK_GRID(gridMemoryText),i);
  }
   gtk_grid_attach(GTK_GRID(gridMemoryText),gtk_label_new("Endereços"),0,0,1,1);
   gtk_grid_attach(GTK_GRID(gridMemoryText),gtk_label_new("Código"),1,0,1,1);

  for(i = 0; i <= size; i++)
  {
     gtk_grid_attach(GTK_GRID(gridMemoryText),gtk_label_new(itoa(i*4,end,16)),0,i+1,1,1);
     gtk_grid_attach(GTK_GRID(gridMemoryText),gtk_label_new(itoa(((GenericInstruction*)(memoryMap->textBaseAddress))[i].word,v,16)),1,i+1,1,1);
  }
   
  return true;
}