#include "Assembler.h"
#include "Instrucoes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "General_Purpose_Register.h"
#include "Coprocessador.h"
#include <glib.h>


#define GetLexical(Obj,index)  (((LexicalToken*)(Obj->information)))->tokens[index]
#define GetLexicalObj(Obj)     (((LexicalToken*)(Obj->information)))
#define GetLexicalLine(Obj)    (((LexicalToken*)Obj)->line)
#define GetLexicalObjLine(Obj) ((((LexicalToken*)(Obj->information)))->line)



extern bool loadMemoryFromVector(void* buffer, unsigned size);
extern void printList(List* list);

static bool compareString(void* elem, void* info)
{
  if(strcmp(elem,(((Instruction*)info)->InstText)) == 0)
	{
	  return  true;
	}  
	return false;
}

static bool compareInt(void*elem, void*info)
{
  return (*((int*)elem) == ((LexicalToken*)info)->line)?1:0;
}

static bool compareLabel(void* elem, void* info)
{
  unsigned i;

  for(i = 0; ((char*)elem)[i] != 0; i++)
  {
    if(((char*)elem)[i] != ((LexicalToken*)info)->tokens[0][i])
      return false;
  }

  return true;
}

static Status* newStatus(LexicalToken* lexicalToken, Response res)
{
  Status* status;

  status = (Status*)malloc(sizeof(Status));
  status->lexical = lexicalToken;
  status->response = res;

  return status;
}

private Instruction* newInstItem(char* text, unsigned _code)
{
	int size;
	Instruction* instruction = NULL;

	instruction = (Instruction*)malloc(sizeof(Instruction));


	size = strlen(text)+2;

	instruction->InstText = (char*)calloc(size,sizeof(char));
	strcpy(GetInText(instruction),text);
	instruction->code = _code;

	return instruction;
}
private ArrayList* mountTableInstructions(void)
{
	ArrayList *mapInst = newArrayList();

	  mapInst->addArray(newInstItem("add", ADD),&mapInst->list);
	  mapInst->addArray(newInstItem("addu", ADDU),&mapInst->list);
	  mapInst->addArray(newInstItem("sub", SUB),&mapInst->list);
	  mapInst->addArray(newInstItem("subu", SUBU),&mapInst->list);
	  mapInst->addArray(newInstItem("div", DIV),&mapInst->list);
	  mapInst->addArray(newInstItem("divu", DIVU),&mapInst->list);
	  mapInst->addArray(newInstItem("mult", MULT),&mapInst->list);
	  mapInst->addArray(newInstItem("multu", MULTU),&mapInst->list);
	  mapInst->addArray(newInstItem("slt", SLT),&mapInst->list);
	  mapInst->addArray(newInstItem("sltu", SLTU),&mapInst->list);
	  mapInst->addArray(newInstItem("and", AND),&mapInst->list);
	  mapInst->addArray(newInstItem("or", OR),&mapInst->list);
  	mapInst->addArray(newInstItem("nor", NOR),&mapInst->list);
	  mapInst->addArray(newInstItem("xor", XOR),&mapInst->list);
	  mapInst->addArray(newInstItem("jr", JR),&mapInst->list);
	  mapInst->addArray(newInstItem("jalr", JALR),&mapInst->list);
	  mapInst->addArray(newInstItem("nop", NOP),&mapInst->list);
	  mapInst->addArray(newInstItem("mfhi", MFHI),&mapInst->list);
	  mapInst->addArray(newInstItem("mflo", MFLO),&mapInst->list);
	  mapInst->addArray(newInstItem("sll", SLL),&mapInst->list);
	  mapInst->addArray(newInstItem("srl", SRL),&mapInst->list);
	  mapInst->addArray(newInstItem("sra", SRA),&mapInst->list);
	  mapInst->addArray(newInstItem("srav", SRAV),&mapInst->list);
	  mapInst->addArray(newInstItem("srlv", SRLV),&mapInst->list);

	  mapInst->addArray(newInstItem("teq", TEQ),&mapInst->list);
  	mapInst->addArray(newInstItem("tge", TGE),&mapInst->list);
	  mapInst->addArray(newInstItem("tgeu", TGEU),&mapInst->list);
	  mapInst->addArray(newInstItem("tlt", TLT),&mapInst->list);
	  mapInst->addArray(newInstItem("tltu", TLTU),&mapInst->list);
	  mapInst->addArray(newInstItem("mthi", MTHI),&mapInst->list);

	  mapInst->addArray(newInstItem("mtlo", MTLO),&mapInst->list);
	  mapInst->addArray(newInstItem("movz", MOVZ),&mapInst->list);
	  mapInst->addArray(newInstItem("movn", MOVN),&mapInst->list);
	  mapInst->addArray(newInstItem("trunq.w.d",TRUNC),&mapInst->list);
	  mapInst->addArray(newInstItem("trunq.w.s",TRUNC),&mapInst->list);
  	mapInst->addArray(newInstItem("syscall", SYSCALL),&mapInst->list);
	  mapInst->addArray(newInstItem("break", BREAK),&mapInst->list);

	  mapInst->addArray(newInstItem("madd", MADD),&mapInst->list);
	  mapInst->addArray(newInstItem("maddu", MADDU),&mapInst->list);
	  mapInst->addArray(newInstItem("mul", MUL),&mapInst->list);
	  mapInst->addArray(newInstItem("msub", MSUB),&mapInst->list);
  	mapInst->addArray(newInstItem("msubu", MSUBU),&mapInst->list);
	  mapInst->addArray(newInstItem("clz", CLZ),&mapInst->list);
	  mapInst->addArray(newInstItem("clo", CLO),&mapInst->list);

  	mapInst->addArray(newInstItem("addi",ADDI),&mapInst->list);
  	mapInst->addArray(newInstItem("addiu",ADDIU),&mapInst->list);
	  mapInst->addArray(newInstItem("slti", SLTI),&mapInst->list);
	  mapInst->addArray(newInstItem("sltiu", SLTIU),&mapInst->list);
	  mapInst->addArray(newInstItem("andi", ANDI),&mapInst->list);
	  mapInst->addArray(newInstItem("ori", ORI),&mapInst->list);
	  mapInst->addArray(newInstItem("xori", XORI),&mapInst->list);
	  mapInst->addArray(newInstItem("lw", LW),&mapInst->list);
    mapInst->addArray(newInstItem("sw", SW),&mapInst->list);
    mapInst->addArray(newInstItem("lbu",LBU),&mapInst->list);
    mapInst->addArray(newInstItem("lb", LB),&mapInst->list);
    mapInst->addArray(newInstItem("sb", SB),&mapInst->list);
    mapInst->addArray(newInstItem("sh", SH),&mapInst->list);
    mapInst->addArray(newInstItem("lui", LUI),&mapInst->list);
    mapInst->addArray(newInstItem("lh", LH),&mapInst->list);
    mapInst->addArray(newInstItem("lwl", LWL),&mapInst->list);
    mapInst->addArray(newInstItem("lhu", LHU),&mapInst->list);
    mapInst->addArray(newInstItem("lwr", LWR),&mapInst->list);
    mapInst->addArray(newInstItem("beq", BEQ),&mapInst->list);
    mapInst->addArray(newInstItem("bne", BNE),&mapInst->list);
    mapInst->addArray(newInstItem("blez", BLEZ),&mapInst->list);
    mapInst->addArray(newInstItem("bgtz", BGTZ),&mapInst->list);
    mapInst->addArray(newInstItem("bltz", BLTZ),&mapInst->list);
    mapInst->addArray(newInstItem("bgez", BGEZ),&mapInst->list);
    mapInst->addArray(newInstItem("bltzal", BLTZAL),&mapInst->list);
    mapInst->addArray(newInstItem("bgezal", BGEZAL),&mapInst->list);

    mapInst->addArray(newInstItem("lwc1", LWC1),&mapInst->list);
    mapInst->addArray(newInstItem("swc1", SWC1),&mapInst->list);
    mapInst->addArray(newInstItem("bclf", BCLF),&mapInst->list);
    mapInst->addArray(newInstItem("ll", LL),&mapInst->list);
    mapInst->addArray(newInstItem("swl", SWL),&mapInst->list);
    mapInst->addArray(newInstItem("swr", SWR),&mapInst->list);
    mapInst->addArray(newInstItem("sdc1", SDC1),&mapInst->list);
    mapInst->addArray(newInstItem("sc", SC),&mapInst->list);
    mapInst->addArray(newInstItem("teqi", TEQI),&mapInst->list);
    mapInst->addArray(newInstItem("tgei", TGEI),&mapInst->list);
    mapInst->addArray(newInstItem("tgeiu", TGEIU),&mapInst->list);
    mapInst->addArray(newInstItem("tlti", TLTI),&mapInst->list);
    mapInst->addArray(newInstItem("tltiu", TLTIU),&mapInst->list);

    mapInst->addArray(newInstItem("j", J),&mapInst->list);
    mapInst->addArray(newInstItem("jal", JAL),&mapInst->list);

    mapInst->addArray(newInstItem("add.s", ADDf),&mapInst->list);
    mapInst->addArray(newInstItem("sub.s",SUBf),&mapInst->list);
    mapInst->addArray(newInstItem("div.s", DIVf),&mapInst->list);
    mapInst->addArray(newInstItem("mul.s", MULf),&mapInst->list);
    mapInst->addArray(newInstItem("sqrt.s", SQRT),&mapInst->list);
    mapInst->addArray(newInstItem("sqrt.d", SQRT),&mapInst->list);
    mapInst->addArray(newInstItem("abs.s", ABS),&mapInst->list);
    mapInst->addArray(newInstItem("abs.d", ABS),&mapInst->list);

    mapInst->addArray(newInstItem("mov.s", MOV),&mapInst->list);
    mapInst->addArray(newInstItem("mov.d", MOV),&mapInst->list);
    mapInst->addArray(newInstItem("neg.s", NEGf),&mapInst->list);
    mapInst->addArray(newInstItem("neg.d", NEGf),&mapInst->list);
    mapInst->addArray(newInstItem("round.w.s",ROUND),&mapInst->list);
    mapInst->addArray(newInstItem("round.w.d",ROUND),&mapInst->list);

    mapInst->addArray(newInstItem("ceil.w.s",CEIL_W),&mapInst->list);
    mapInst->addArray(newInstItem("ceil.w.d", CEIL_W),&mapInst->list);
    mapInst->addArray(newInstItem("floor.w.s", FLOOR_W),&mapInst->list);
    mapInst->addArray(newInstItem("floor.w.d",FLOOR_W),&mapInst->list);
    mapInst->addArray(newInstItem("cvt.s.w", CVT_S),&mapInst->list);
    mapInst->addArray(newInstItem("cvt.s.d", CVT_S),&mapInst->list);
    mapInst->addArray(newInstItem("cvt.d.s",CVT_D),&mapInst->list);
    mapInst->addArray(newInstItem("cvt.d.w",CVT_D),&mapInst->list);
    mapInst->addArray(newInstItem("cvt.w.s",CVT_W),&mapInst->list);
    mapInst->addArray(newInstItem("cvt.w.d",CVT_W),&mapInst->list);

    mapInst->addArray(newInstItem("mfc1", MFC1),&mapInst->list);
    mapInst->addArray(newInstItem("mtc1", MTC1),&mapInst->list);
    mapInst->addArray(newInstItem("mfc0", MFC0),&mapInst->list);
    mapInst->addArray(newInstItem("mtc0", MTC0),&mapInst->list);

    mapInst->addArray(newInstItem("la",PSEUDOINSTRUCTION),&mapInst->list);
    mapInst->addArray(newInstItem("li",PSEUDOINSTRUCTION),&mapInst->list);
    mapInst->addArray(newInstItem("bgt",PSEUDOINSTRUCTION),&mapInst->list);
    mapInst->addArray(newInstItem("blt",PSEUDOINSTRUCTION),&mapInst->list);
    mapInst->addArray(newInstItem("bltu",PSEUDOINSTRUCTION),&mapInst->list);
    mapInst->addArray(newInstItem("bge",PSEUDOINSTRUCTION),&mapInst->list);
    mapInst->addArray(newInstItem("bgeu",PSEUDOINSTRUCTION),&mapInst->list);
    mapInst->addArray(newInstItem("ble",PSEUDOINSTRUCTION),&mapInst->list);
    mapInst->addArray(newInstItem("bgtu",PSEUDOINSTRUCTION),&mapInst->list);
    mapInst->addArray(newInstItem("bnez",PSEUDOINSTRUCTION),&mapInst->list);
    mapInst->addArray(newInstItem("b",PSEUDOINSTRUCTION),&mapInst->list);
    mapInst->addArray(newInstItem("beqz",PSEUDOINSTRUCTION),&mapInst->list);
    mapInst->addArray(newInstItem("$zero",Zero),&mapInst->list);
    mapInst->addArray(newInstItem("$at", At),&mapInst->list);
    mapInst->addArray(newInstItem("$v0",V0),&mapInst->list);
    mapInst->addArray(newInstItem("$v1",V1),&mapInst->list);
    mapInst->addArray(newInstItem("$a0",A0),&mapInst->list);
    mapInst->addArray(newInstItem("$a1",A1),&mapInst->list);
    mapInst->addArray(newInstItem("$a2",A2),&mapInst->list);
    mapInst->addArray(newInstItem("$a3",A3),&mapInst->list);
    mapInst->addArray(newInstItem("$t0",T0),&mapInst->list);
    mapInst->addArray(newInstItem("$t1",T1),&mapInst->list);
    mapInst->addArray(newInstItem("$t2",T2),&mapInst->list);
    mapInst->addArray(newInstItem("$t3",T3),&mapInst->list);
    mapInst->addArray(newInstItem("$t4",T4),&mapInst->list);
    mapInst->addArray(newInstItem("$t5",T5),&mapInst->list);
    mapInst->addArray(newInstItem("$t6",T6),&mapInst->list);
    mapInst->addArray(newInstItem("$t7",T7),&mapInst->list);
    mapInst->addArray(newInstItem("$s0",S0),&mapInst->list);
    mapInst->addArray(newInstItem("$s1",S1),&mapInst->list);
    mapInst->addArray(newInstItem("$s2",S2),&mapInst->list);
    mapInst->addArray(newInstItem("$s3",S3),&mapInst->list);
    mapInst->addArray(newInstItem("$s4",S4),&mapInst->list);
    mapInst->addArray(newInstItem("$s5",S5),&mapInst->list);
    mapInst->addArray(newInstItem("$s6",S6),&mapInst->list);
    mapInst->addArray(newInstItem("$s7",S7),&mapInst->list);

    mapInst->addArray(newInstItem("$t8",T8),&mapInst->list);
    mapInst->addArray(newInstItem("$t9",T9),&mapInst->list);
    mapInst->addArray(newInstItem("$k0",K0),&mapInst->list);
    mapInst->addArray(newInstItem("$k1",K1),&mapInst->list);
    mapInst->addArray(newInstItem("$gp",Gp),&mapInst->list);
    mapInst->addArray(newInstItem("$sp",Sp),&mapInst->list);
    mapInst->addArray(newInstItem("$fp",Fp),&mapInst->list);
    mapInst->addArray(newInstItem("$ra",Ra),&mapInst->list);


    return mapInst;

}

private ArrayList* moutTableRegisterCoprocessor(void)
{
	ArrayList *tableCopr =  newArrayList();

	tableCopr->addArray(newInstItem("$f0",f0),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f1",f1),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f2",f2),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f3",f3),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f4",f4),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f5",f5),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f6",f6),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f7",f7),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f8",f8),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f9",f9),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f10",f10),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f11",f11),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f12",f12),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f13",f13),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f14",f14),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f15",f15),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f16",f16),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f17",f17),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f18",f18),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f19",f19),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f20",f20),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f21",f21),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f22",f22),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f23",f23),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f24",f24),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f25",f25),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f26",f26),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f27",f27),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f28",f28),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f29",f29),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f30",f30),&tableCopr->list);
	tableCopr->addArray(newInstItem("$f31",f31),&tableCopr->list);

	return tableCopr;
}

Status* assemblerCode(ArrayList* labels, ArrayList* instructions)
{
	List *temp;
	char *txtTemp = NULL;
	ArrayList* tableInstructions = NULL;
	ArrayList* tableCoprocessor = NULL;
	Instruction* instrc = NULL;
	unsigned sizeListInstructions = 0;
	void* buffer;
	int index = 0;
  Status* status;

  sizeListInstructions = instructions->size(instructions->list);
  buffer = calloc(sizeListInstructions+10,sizeof(unsigned));

	tableInstructions = mountTableInstructions();


	for(temp = instructions->list; temp != NULL; temp = temp->next)
	{

		  txtTemp = GetLexical(temp,0);
	    instrc = tableInstructions->seach(tableInstructions->list,txtTemp,compareString);

        if(instrc == NULL)
        {
          status = newStatus(GetLexicalObj(temp),IncorrectInstruction);
          return status;
        }
        else if(strcmp(GetInText(instrc),"add") == 0)
        {
           	Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;
            GenericInstruction generic;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
            	   return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rs = GetCode(operating);
               	           
               	             break;

               	    case 3: rType.acess.rt = GetCode(operating);
                           
               	            break;
               	    default:
               	             break;

               }
           }  
           generic.word = rType.word; 
           ((GenericInstruction*)buffer)[index] = generic;
           index += 1;

        }

        else if(strcmp(GetInText(instrc),"addu") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;
            GenericInstruction generic;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                 status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                 return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rs = GetCode(operating);
               	           
               	             break;

               	    case 3: rType.acess.rt = GetCode(operating);
                           
               	            break;
               	    default:
               	             break;

               }
           }

           generic.word = rType.word; 
           ((GenericInstruction*)buffer)[index] = generic;
           index += 1;   

        }
        else if(strcmp(GetInText(instrc),"sub") == 0)
        {
          	Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;
            GenericInstruction generic;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rs = GetCode(operating);
               	           
               	             break;

               	    case 3: rType.acess.rt = GetCode(operating);
                           
               	            break;
               	    default:
               	             break;

               }
           }   
            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;

        }
        else if(strcmp(GetInText(instrc),"subu") == 0)
        {
          	Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;
            GenericInstruction generic;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rs = GetCode(operating);
               	           
               	             break;

               	    case 3: rType.acess.rt = GetCode(operating);
                           
               	            break;
               	    default:
               	             break;

               }
           } 

            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 

        }
        else if(strcmp(GetInText(instrc),"mult") == 0)
        {
          	Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;
            GenericInstruction generic;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;
            rType.acess.rd = 0;

            for(i = 1; i <= 2; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rs = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rt = GetCode(operating);
               	           
               	             break;

               	    default:
               	             break;

               }
           }   

            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;
        }
        else if(strcmp(GetInText(instrc),"multu") == 0)
        {
          	Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;
            GenericInstruction generic;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;
            rType.acess.rd = 0;


            for(i = 1; i <= 2; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                 status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                 return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rs = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rt = GetCode(operating);
               	           
               	             break;

               	    default:
               	             break;

               }
           } 

            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;  
        }
        else if(strcmp(GetInText(instrc),"slt") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;
            GenericInstruction generic;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rt = GetCode(operating);
               	           
               	             break;

               	    case 3: rType.acess.rs = GetCode(operating);
                           
               	            break;
               	    default:
               	             break;

               }
           }   
            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;

        }
        else if(strcmp(GetInText(instrc),"sltu") == 0)
        {
          	Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;
            GenericInstruction generic;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rt = GetCode(operating);
               	           
               	             break;

               	    case 3:  rType.acess.rs = GetCode(operating);
                           
               	             break;
               	    default:
               	             break;

               }
           }   
            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;

        }
        else if(strcmp(GetInText(instrc),"and") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;
            GenericInstruction generic;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rt = GetCode(operating);
               	           
               	             break;

               	    case 3:  rType.acess.rs = GetCode(operating);
                           
               	             break;
               	    default:
               	             break;

               }
           }   
            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;

        }
         else if(strcmp(GetInText(instrc),"or") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;
            GenericInstruction generic;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rt = GetCode(operating);
               	           
               	             break;

               	    case 3:  rType.acess.rs = GetCode(operating);
                           
               	            break;
               	    default:
               	             break;

               }
           }   

            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;

        }
        else if(strcmp(GetInText(instrc),"nor") == 0)
        {
          	Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;
            GenericInstruction generic;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rt = GetCode(operating);
               	           
               	             break;

               	    case 3:  rType.acess.rs = GetCode(operating);
                           
               	             break;
               	    default:
               	             break;

               }
           }   

            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;

        }
        else if(strcmp(GetInText(instrc),"xor") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;
            GenericInstruction generic;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rt = GetCode(operating);
               	           
               	             break;

               	    case 3:  rType.acess.rs = GetCode(operating);
                           
               	             break;
               	    default:
               	             break;

               }
           }  

            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 

        }
        else if(strcmp(GetInText(instrc),"jr") == 0)
        {
          	Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;
            GenericInstruction generic;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;
            rType.acess.rt = 0;
            rType.acess.rd = 0;
       
            txtOperating = GetLexical(temp,1);
            
            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            } 

             rType.acess.rs =  GetCode(operating);   

             generic.word = rType.word; 
             ((GenericInstruction*)buffer)[index] = generic;
             index += 1;
        }

        else if(strcmp(GetInText(instrc),"jalr") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;
            GenericInstruction generic;
            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;
            rType.acess.rt = 0;

            for(i = 1; i <= 2; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rs = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rd = GetCode(operating);
               	           
               	             break;
               	    default:
               	             break;

               }
           }  

            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 

        }
        else if(strcmp(GetInText(instrc),"nop") == 0)
        {
        	  Instruction *operating = NULL;
            GenericInstruction generic;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;

            rType.acess.funct = GetCode(instrc);
            rType.word = 0;

             generic.word = rType.word; 
             ((GenericInstruction*)buffer)[index] = generic;
             index += 1;

        }
        else if(strcmp(GetInText(instrc),"mfhi") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            GenericInstruction generic;
            unsigned i;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;
            rType.acess.rt = 0;
            rType.acess.rs = 0;

           
            txtOperating = GetLexical(temp,1);
            
            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status; 
            }
            rType.acess.rd = GetCode(operating);

             generic.word = rType.word; 
             ((GenericInstruction*)buffer)[index] = generic;
             index += 1;
               
        }
        else if(strcmp(GetInText(instrc),"mflo") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            GenericInstruction generic;
            RTypeInstruction rType;
            unsigned i;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;
            rType.acess.rt = 0;
            rType.acess.rs = 0;

           
            txtOperating = GetLexical(temp,1);
            
            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            { 
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            }
            rType.acess.rd = GetCode(operating);

             generic.word = rType.word; 
             ((GenericInstruction*)buffer)[index] = generic;
             index += 1;
               
        }
        else if(strcmp(GetInText(instrc),"syscall") == 0)
        {
            RTypeInstruction rType;
            GenericInstruction generic;
  
            rType.word = 0;
            rType.acess.funct = GetCode(instrc);

            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;

        }
        else if(strcmp(GetInText(instrc),"mthi") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            GenericInstruction generic;
            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;
            rType.acess.rt = 0;
            rType.acess.rd = 0;

           
            txtOperating = GetLexical(temp,1);
            
            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            }
            rType.acess.rs = GetCode(operating);

             generic.word = rType.word; 
             ((GenericInstruction*)buffer)[index] = generic;
             index += 1;
               
        }
        else if(strcmp(GetInText(instrc),"mtlo") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            GenericInstruction generic;
       
            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;
            rType.acess.rt = 0;
            rType.acess.rd = 0;

           
            txtOperating = GetLexical(temp,1);
            
            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
               status = newStatus(GetLexicalObj(temp),MissingArguments);
               return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            }
            rType.acess.rs = GetCode(operating);

             generic.word = rType.word; 
             ((GenericInstruction*)buffer)[index] = generic;
             index += 1;
               
        }
        else if(strcmp(GetInText(instrc),"sllv") == 0)
        {
        	Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            GenericInstruction generic;
            unsigned i;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                 status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                 return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rt = GetCode(operating);
               	           
               	             break;

               	    case 3: rType.acess.rs = GetCode(operating);
                           
               	            break;
               	    default:
               	             break;

               }
           }   

            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;

        }
        else if(strcmp(GetInText(instrc),"srlv") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;
            GenericInstruction generic;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                 status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                 return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rt = GetCode(operating);
               	           
               	             break;

               	    case 3: rType.acess.rs = GetCode(operating);
                           
               	            break;
               	    default:
               	             break;

               }
           }  
             generic.word = rType.word; 
             ((GenericInstruction*)buffer)[index] = generic;
             index += 1; 

        }
        else if(strcmp(GetInText(instrc),"srav") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            RTypeInstruction rType;
            unsigned i;
            GenericInstruction generic;

            rType.acess.funct = GetCode(instrc);
            rType.acess.op = 0;
            rType.acess.shamt = 0;

            for(i = 1; i <= 3; i++)
            {

               txtOperating = GetLexical(temp,i);
            
               if(txtOperating == NULL || strlen(txtOperating) == 0)
               {
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
               }
               operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
               if(operating ==  NULL)
               {
                 status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                 return status;
               }
               switch(i)
               {
               	    case 1:  rType.acess.rd = GetCode(operating);
               	    
               	             break;

               	    case 2:  rType.acess.rt = GetCode(operating);
               	           
               	             break;

               	    case 3:  rType.acess.rs = GetCode(operating);
                           
               	            break;
               	    default:
               	             break;

               }
           }   

            generic.word = rType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;

        }
        
        else if(strcmp(GetInText(instrc),"addi") == 0)                                                                                                 
        {
        	Instruction *operating = NULL;
          char* txtOperating;
          ITypeInstruction iType;
          GenericInstruction generic;
          unsigned i;

            iType.acess.op = GetCode(instrc);

            for(i = 1; i <= 2; i++)
            {

            	txtOperating = GetLexical(temp,i);

                if(txtOperating == NULL || strlen(txtOperating) == 0)
                { 
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
                }
                operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
                if(operating ==  NULL)
                {
                   status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                   return status;
                }

                switch(i)
                {
                	case 1:  iType.acess.rt = GetCode(operating);

                	         break;
                	case 2:  iType.acess.rs = GetCode(operating);

                             break;
                    default: 
                             break;

                }
                
                

            }   
                txtOperating = GetLexical(temp,3);
                if(txtOperating == NULL)
                { 
                  status = newStatus(GetLexicalObj(temp),MissingArguments);
                  return status;
                }
                iType.acess.addressOrConstant = atoi(txtOperating);
                generic.word = iType.word; 
                ((GenericInstruction*)buffer)[index] = generic;
                index += 1;

        }

        else if(strcmp(GetInText(instrc),"addiu") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            GenericInstruction generic;
            ITypeInstruction iType;
            unsigned i;

            iType.acess.op = GetCode(instrc);

            for(i = 1; i <= 2; i++)
            {

            	txtOperating = GetLexical(temp,i);

                if(txtOperating == NULL || strlen(txtOperating) == 0)
                { 
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
                }
                operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
                if(operating ==  NULL)
                {
                  status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                  return status;
                }

                switch(i)
                {
                	case 1:  iType.acess.rt = GetCode(operating);

                	         break;
                	case 2:  iType.acess.rs = GetCode(operating);

                              break;
                    default: 
                             break;

                }
               
            }

                txtOperating = GetLexical(temp,3);
                if(txtOperating == NULL)
                { 
                  status = newStatus(GetLexicalObj(temp),MissingArguments);
                  return status;
                }
                iType.acess.addressOrConstant = (unsigned short)atoi(txtOperating);
                generic.word = iType.word; 
                ((GenericInstruction*)buffer)[index] = generic;
                index += 1;

        }   
        else if(strcmp(GetInText(instrc),"slti") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            GenericInstruction generic;
            ITypeInstruction iType;
            unsigned i;

            iType.acess.op = GetCode(instrc);

            for(i = 1; i <= 2; i++)
            {

            	txtOperating = GetLexical(temp,i);

                if(txtOperating == NULL || strlen(txtOperating) == 0)
                { 
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
                }
                operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
                if(operating ==  NULL)
                {
                  status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                  return status;
                }

                switch(i)
                {
                	case 1:  iType.acess.rs = GetCode(operating);

                	         break;
                	case 2:  iType.acess.rt = GetCode(operating);

                              break;
                    default: 
                             break;

                }
                
                

            }   
                txtOperating = GetLexical(temp,3);
                if(txtOperating == NULL)
                { 
                  status = newStatus(GetLexicalObj(temp),MissingArguments);
                  return status;
                }
                iType.acess.addressOrConstant = (unsigned short)atoi(txtOperating);
                generic.word = iType.word; 
                ((GenericInstruction*)buffer)[index] = generic;
                index += 1;

        }
        else if(strcmp(GetInText(instrc),"sltiu") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            GenericInstruction generic;
            ITypeInstruction iType;
            unsigned i;

            iType.acess.op = GetCode(instrc);

            for(i = 1; i <= 2; i++)
            {

            	txtOperating = GetLexical(temp,i);

                if(txtOperating == NULL || strlen(txtOperating) == 0)
                {
                  status = newStatus(GetLexicalObj(temp),MissingArguments);
                  return status;
                }
                operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
                if(operating ==  NULL)
                {
                 status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                 return status;
                }

                switch(i)
                {
                	case 1:  iType.acess.rs = GetCode(operating);

                	         break;
                	case 2:  iType.acess.rt = GetCode(operating);

                              break;
                    default: 
                             break;

                }

            }   
                txtOperating = GetLexical(temp,3);
                if(txtOperating == NULL || strlen(txtOperating) == 0)
                { 
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
                }
                iType.acess.addressOrConstant = (unsigned short)atoi(txtOperating);
                generic.word = iType.word; 
                ((GenericInstruction*)buffer)[index] = generic;
                index += 1;

        }
        else if(strcmp(GetInText(instrc),"andi") == 0)
        {
        	  Instruction *operating = NULL;
            GenericInstruction generic;
            char* txtOperating;
            ITypeInstruction iType;
            unsigned i;

            iType.acess.op = GetCode(instrc);

            for(i = 1; i <= 2; i++)
            {

            	txtOperating = GetLexical(temp,i);

                if(txtOperating == NULL || strlen(txtOperating) == 0)
                {
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
                }
                operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
                if(operating ==  NULL)
                {
                   status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                   return status;
                }

                switch(i)
                {
                	case 1:  iType.acess.rs = GetCode(operating);

                	         break;
                	case 2:  iType.acess.rt = GetCode(operating);

                              break;
                    default: 
                             break;

                }

            }   
                txtOperating = GetLexical(temp,3);
                if(txtOperating == NULL || strlen(txtOperating) == 0)
                { 
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
                }
                iType.acess.addressOrConstant = (unsigned short)atoi(txtOperating);
                generic.word = iType.word; 
                ((GenericInstruction*)buffer)[index] = generic;
                index += 1;

        }
        else if(strcmp(GetInText(instrc),"ori") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            GenericInstruction generic;
            ITypeInstruction iType;
            unsigned i;

            iType.acess.op = GetCode(instrc);

            for(i = 1; i <= 2; i++)
            {

            	  txtOperating = GetLexical(temp,i);

                if(txtOperating == NULL || strlen(txtOperating) == 0)
                { 
                  status = newStatus(GetLexicalObj(temp),MissingArguments);
                  return status;
                }
                operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
                if(operating ==  NULL)
                {
                 status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                 return status;
                }

                switch(i)
                {
                	case 1:  iType.acess.rt = GetCode(operating);

                	         break;
                	case 2:  iType.acess.rs = GetCode(operating);

                           break;
                  default: 
                           break;

                }
                

            }   
                txtOperating = GetLexical(temp,3);
                if(txtOperating == NULL || strlen(txtOperating) == 0)
                { 
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
                }
                iType.acess.addressOrConstant = (unsigned short)atoi(txtOperating);
                generic.word = iType.word; 
                ((GenericInstruction*)buffer)[index] = generic;
                index += 1;

        }
        else if(strcmp(GetInText(instrc),"xori") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            GenericInstruction generic;
            ITypeInstruction iType;
            unsigned i;

            iType.acess.op = GetCode(instrc);

            for(i = 1; i <= 2; i++)
            {

            	txtOperating = GetLexical(temp,i);

                if(txtOperating == NULL || strlen(txtOperating) == 0)
                {
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
                }
                operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
                if(operating ==  NULL)
                {
                 status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                 return status;
                }

                switch(i)
                {
                	case 1:  iType.acess.rs = GetCode(operating);

                	         break;
                	case 2:  iType.acess.rt = GetCode(operating);

                              break;
                    default: 
                             break;

                }
                

            }   
                txtOperating = GetLexical(temp,3);
                if(txtOperating == NULL || strlen(txtOperating) == 0)
                {
                 status = newStatus(GetLexicalObj(temp),MissingArguments);
                 return status;
                }
                iType.acess.addressOrConstant = (unsigned short)atoi(txtOperating);
                generic.word = iType.word; 
                ((GenericInstruction*)buffer)[index] = generic;
                index += 1;

        }
        else if(strcmp(GetInText(instrc),"lui") == 0)
        {
        	  Instruction *operating = NULL;
            char* txtOperating;
            GenericInstruction generic;
            ITypeInstruction iType;
           
            iType.acess.op = GetCode(instrc);

           
            txtOperating = GetLexical(temp,1);

            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            }
            iType.acess.rt = GetCode(operating);
            iType.acess.rs = 0;
         
            txtOperating = GetLexical(temp,2);
            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            iType.acess.addressOrConstant = (unsigned short)atoi(txtOperating); 
            generic.word = iType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;       

        }
        else if(strcmp(GetInText(instrc),"li") == 0)
        {
            Instruction *operating = NULL;
            char* txtOperating;
            GenericInstruction generic;
            ITypeInstruction iType;
            unsigned int value;
           
                  
            txtOperating = GetLexical(temp,1);

            if((txtOperating == NULL) || (strlen(txtOperating) == 0))
            { 
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);

            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            }
             
            txtOperating = GetLexical(temp,2);
            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            value = atoi(txtOperating);

            if(value < 65535)
            {
              
              Instruction* instrTemp = NULL;

              instrTemp = tableInstructions->seach(tableInstructions->list,"addiu",compareString);
              
              iType.acess.op = GetCode(instrTemp);
              iType.acess.rt = GetCode(operating);
              iType.acess.rs = Zero; 
              iType.acess.addressOrConstant = value;
              generic.word = iType.word; 
              ((GenericInstruction*)buffer)[index] = generic;
              index += 1;    
            }
            else 
            {
              Instruction* instrTemp = NULL;

              instrTemp = tableInstructions->seach(tableInstructions->list,"ori",compareString);
              iType.acess.op = GetCode(instrTemp);
              iType.acess.rt = GetCode(operating);
              iType.acess.rs = At;

              iType.acess.addressOrConstant = value;
              generic.word = iType.word; 
              ((GenericInstruction*)buffer)[index] = generic;
              index += 1; 
    
              instrTemp = tableInstructions->seach(tableInstructions->list,"lui",compareString);
              iType.acess.op = GetCode(instrTemp);
              iType.acess.rt = At;
              iType.acess.rs = 0;
              iType.acess.addressOrConstant = (value >> 16);
              generic.word = iType.word; 
              ((GenericInstruction*)buffer)[index] = generic;
              index += 1;       

            }
       
                
        }
        else if(strcmp(GetInText(instrc),"j") == 0)
        {
            LexicalToken* lexical = NULL;
            char* target;
            GenericInstruction generic;
            JTypeInstruction jType;
            unsigned i;
            int offset = 0;
            int qntInst = 0;
           
            jType.acess.op = GetCode(instrc);

           
            target = GetLexical(temp,1);

            if(target == NULL || strlen(target) == 0)
            { 
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            lexical = labels->seach(labels->list,target,compareLabel);
               
            if(lexical ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
              return status;
            }
            if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            } 
            
            if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
            {
              offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);
            
              for(i = 0; i < offset; i++)
              {
                LexicalToken* lexicalTemp;
                int numberLine = GetLexicalObjLine(temp)+i;
                lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

                if(lexicalTemp != NULL)
                {
                 qntInst += 1;
                }

             }
            
               jType.acess.address = 0;
               jType.acess.address |= qntInst;
               generic.word = jType.word; 
               ((GenericInstruction*)buffer)[index] = generic;
               index += 1; 
            }else if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
            {
               for(i = 0; i < GetLexicalLine(lexical); i++)
               {
                 LexicalToken* lexicalTemp;

                 lexicalTemp = instructions->seach(instructions->list,&i,compareInt);

                 if(lexicalTemp != NULL)
                 {
                   qntInst += 1;
                 }

               }

               jType.acess.address = 0;
               jType.acess.address |= qntInst;
               generic.word = jType.word; 
               ((GenericInstruction*)buffer)[index] = generic;
               index += 1; 
            }  
            
        }
        else if(strcmp(GetInText(instrc),"beq") == 0)
        {
          GenericInstruction generic;
          ITypeInstruction iType;
          unsigned i;
          int qntInst = 0;
          Instruction* operating = NULL;
          char* target;
          char* txtOperating;
          LexicalToken* _lexical;
          unsigned offset;

          iType.acess.op = GetCode(instrc);
        

          for(i = 1; i <= 2; i++)
          {
             txtOperating = GetLexical(temp,i);

             if(txtOperating == NULL || strlen(txtOperating) == 0)
             { 
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
             }
             operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
             if(operating ==  NULL)
             {
               status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
             }

             switch(i)
             {
               case 1:  iType.acess.rs = GetCode(operating);

                        break;
               case 2:  iType.acess.rt = GetCode(operating);

                        break;
               default: 
                        break;

             }

          }

          target = GetLexical(temp,3);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          _lexical = labels->seach(labels->list,target,compareLabel);

          if(_lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }
          if(g_ascii_isdigit(_lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(_lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(_lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(_lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(_lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(_lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            
            generic.word = iType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 

        }
        else if(strcmp(GetInText(instrc),"bne") == 0)
        {

          GenericInstruction generic;
          ITypeInstruction iType;
          unsigned i;
          int qntInst = 0;
          Instruction* operating = NULL;
          char* target;
          char* txtOperating;
          LexicalToken* _lexical;
          unsigned offset;

          iType.acess.op = GetCode(instrc);
        
          for(i = 1; i <= 2; i++)
          {
             txtOperating = GetLexical(temp,i);

             if(txtOperating == NULL || strlen(txtOperating) == 0)
             { 
                status = newStatus(GetLexicalObj(temp),MissingArguments);
                return status;
             }
             operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
             if(operating ==  NULL)
             {
               status = newStatus(GetLexicalObj(temp),OperatingInvalid);
                return status;
             }

             switch(i)
             {
               case 1:  iType.acess.rs = GetCode(operating);

                        break;
               case 2:  iType.acess.rt = GetCode(operating);

                        break;
               default: 
                        break;

             }

          }

          target = GetLexical(temp,3);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          _lexical = labels->seach(labels->list,target,compareLabel);

          if(_lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }
          if(g_ascii_isdigit(_lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(_lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(_lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(_lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(_lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(_lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            
            generic.word = iType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 

        }
        else if(strcmp(GetInText(instrc),"blez") == 0)
        {
          GenericInstruction generic;
          ITypeInstruction iType;
          unsigned i;
          int qntInst = 0;
          Instruction* operating = NULL;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          unsigned offset;

          iType.acess.op = GetCode(instrc);

          txtOperating = GetLexical(temp,1);

          if(txtOperating == NULL || strlen(txtOperating) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
          if(operating ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          }
          iType.acess.rs = GetCode(operating);
          iType.acess.rt = 0;

          target = GetLexical(temp,2);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }
          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            
            generic.word = iType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 
        }
        else if(strcmp(GetInText(instrc),"bgtz") == 0)
        {

          GenericInstruction generic;
          ITypeInstruction iType;
          unsigned i;
          int qntInst = 0;
          Instruction* operating = NULL;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          unsigned offset;

          iType.acess.op = GetCode(instrc);

          txtOperating = GetLexical(temp,1);

          if(txtOperating == NULL || strlen(txtOperating) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
          if(operating ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          }
          iType.acess.rs = GetCode(operating);
          iType.acess.rt = 0;

          target = GetLexical(temp,2);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }
          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            
            generic.word = iType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 

        }
        else if(strcmp(GetInText(instrc),"bltz") == 0)
        {
          GenericInstruction generic;
          ITypeInstruction iType;
          unsigned i;
          int qntInst = 0;
          Instruction* operating = NULL;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          unsigned offset;

          iType.acess.op = GetCode(instrc);

          txtOperating = GetLexical(temp,1);

          if(txtOperating == NULL || strlen(txtOperating) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
          if(operating ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          }
          iType.acess.rs = GetCode(operating);
          iType.acess.rt = 0;

          target = GetLexical(temp,2);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }
          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            
            generic.word = iType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 

        }
        else if(strcmp(GetInText(instrc),"bgez") == 0)
        {
          GenericInstruction generic;
          ITypeInstruction iType;
          unsigned i;
          int qntInst = 0;
          Instruction* operating = NULL;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          unsigned offset;

          iType.acess.op = GetCode(instrc);

          txtOperating = GetLexical(temp,1);

          if(txtOperating == NULL || strlen(txtOperating) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
          if(operating ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          }
          iType.acess.rs = GetCode(operating);
          iType.acess.rt = 1;
         
          target = GetLexical(temp,2);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }
          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            
            generic.word = iType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 

        }
        else if(strcmp(GetInText(instrc),"bltzal") == 0)
        {
          GenericInstruction generic;
          ITypeInstruction iType;
          unsigned i;
          int qntInst = 0;
          Instruction* operating = NULL;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          unsigned offset;

          iType.acess.op = GetCode(instrc);

          txtOperating = GetLexical(temp,1);

          if(txtOperating == NULL || strlen(txtOperating) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
          if(operating ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          }
          iType.acess.rs = GetCode(operating);
          iType.acess.rt = 0x10;

          target = GetLexical(temp,2);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }
          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            
            generic.word = iType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 

        }

        else if(strcmp(GetInText(instrc),"bgezal") == 0)
        {
          GenericInstruction generic;
          ITypeInstruction iType;
          unsigned i;
          int qntInst = 0;
          Instruction* operating = NULL;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          unsigned offset;

          iType.acess.op = GetCode(instrc);

          txtOperating = GetLexical(temp,1);

          if(txtOperating == NULL || strlen(txtOperating) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
          if(operating ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          }
          iType.acess.rs = GetCode(operating);
          iType.acess.rt = 0x11;

          target = GetLexical(temp,2);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }
          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            
            generic.word = iType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 
        }
        else if(strcmp(GetInText(instrc),"ble") == 0)
        {
          unsigned i;
          int qntInst = 0;
          Instruction* inst = NULL;
          Instruction* operating;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          ITypeInstruction iType;
          RTypeInstruction rType;
          GenericInstruction generic;
          int offset = 0;

          inst = tableInstructions->seach(tableInstructions->list,"slt",compareString);

          rType.acess.op = 0;
          rType.acess.funct = GetCode(inst);
          rType.acess.rd = At;
          rType.acess.shamt = 0;
          
          for(i = 1; i <= 2; i++)
          { 
            txtOperating = GetLexical(temp,i);

            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            }

            switch(i)
            {
              case 1:
                      rType.acess.rt = GetCode(operating);
                      break;

              case 2: rType.acess.rs = GetCode(operating);
                      break;

              default:
                    break; 
            }
        }

          inst = tableInstructions->seach(tableInstructions->list,"beq",compareString);

          iType.acess.op = GetCode(inst);
          iType.acess.rs = At;
          iType.acess.rt = Zero;

          target = GetLexical(temp,3);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }

          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }

          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            generic.wordu = iType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;
            generic.wordu = rType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;       

        }
        else if(strcmp(GetInText(instrc),"b") == 0)
        {
          unsigned i;
          int qntInst = 0;
          Instruction* inst = NULL;
          char* target;
          LexicalToken* lexical;
          ITypeInstruction iType;
          GenericInstruction generic;
          int offset = 0;
           
          inst = tableInstructions->seach(tableInstructions->list,"bgez",compareString);
          iType.acess.op = GetCode(inst);
          iType.acess.rs = Zero;
          iType.acess.rt = 1;


          target = GetLexical(temp,1);

          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }

          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          }
           if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          } 

             generic.wordu = iType.wordu;
             ((GenericInstruction*)buffer)[index] = generic;
             index += 1;
        }
        else if(strcmp(GetInText(instrc),"bge") == 0)
        {
          unsigned i;
          int qntInst = 0;
          Instruction* inst = NULL;
          Instruction* operating;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          ITypeInstruction iType;
          RTypeInstruction rType;
          GenericInstruction generic;
          int offset = 0;

          inst = tableInstructions->seach(tableInstructions->list,"slt",compareString);

          rType.acess.op = 0;
          rType.acess.funct = GetCode(inst);
          rType.acess.rd = At;
          rType.acess.shamt = 0;
          
          for(i = 1; i <= 2; i++)
          { 
            txtOperating = GetLexical(temp,i);

            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            }

            switch(i)
            {
              case 1:
                      rType.acess.rs = GetCode(operating);
                      break;

              case 2: rType.acess.rt = GetCode(operating);
                      break;

              default:
                    break; 
            }
        }

          inst = tableInstructions->seach(tableInstructions->list,"beq",compareString);

          iType.acess.op = GetCode(inst);
          iType.acess.rs = At;
          iType.acess.rt = Zero;

          target = GetLexical(temp,3);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }

          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }

          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            generic.wordu = iType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;
            generic.wordu = rType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;       

        }
        else if(strcmp(GetInText(instrc),"bgeu") == 0)
        {
          unsigned i;
          int qntInst = 0;
          Instruction* inst = NULL;
          Instruction* operating;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          ITypeInstruction iType;
          RTypeInstruction rType;
          GenericInstruction generic;
          int offset = 0;

          inst = tableInstructions->seach(tableInstructions->list,"sltu",compareString);
          
          rType.acess.op = 0;
          rType.acess.funct = GetCode(inst);
          rType.acess.rd = At;
          rType.acess.shamt = 0;
          
          for(i = 1; i <= 2; i++)
          { 
            txtOperating = GetLexical(temp,i);

            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            }

            switch(i)
            {
              case 1:
                      rType.acess.rs = GetCode(operating);
                      break;

              case 2: rType.acess.rt = GetCode(operating);
                      break;

              default:
                    break; 
            }
        }

          inst = tableInstructions->seach(tableInstructions->list,"beq",compareString);

          iType.acess.op = GetCode(inst);
          iType.acess.rs = At;
          iType.acess.rt = Zero;

          target = GetLexical(temp,3);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }

          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }

          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            generic.wordu = iType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;
            generic.wordu = rType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;       

        }
        else if(strcmp(GetInText(instrc),"bgt") == 0)
        {
          unsigned i;
          int qntInst = 0;
          Instruction* inst = NULL;
          Instruction* operating;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          ITypeInstruction iType;
          RTypeInstruction rType;
          GenericInstruction generic;
          int offset = 0;

          inst = tableInstructions->seach(tableInstructions->list,"slt",compareString);

          rType.acess.op = 0;
          rType.acess.funct = GetCode(inst);
          rType.acess.rd = At;
          rType.acess.shamt = 0;
          
          for(i = 1; i <= 2; i++)
          { 
            txtOperating = GetLexical(temp,i);

            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            }

            switch(i)
            {
              case 1:
                      rType.acess.rt = GetCode(operating);
                      break;

              case 2: rType.acess.rs = GetCode(operating);
                      break;

              default:
                    break; 
            }
        }

          inst = tableInstructions->seach(tableInstructions->list,"bne",compareString);

          iType.acess.op = GetCode(inst);
          iType.acess.rs = At;
          iType.acess.rt = Zero;

          target = GetLexical(temp,3);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }

          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }

          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            generic.wordu = iType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;
            generic.wordu = rType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;       

        }
        else if(strcmp(GetInText(instrc),"bgtu") == 0)
        {
          unsigned i;
          int qntInst = 0;
          Instruction* inst = NULL;
          Instruction* operating;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          ITypeInstruction iType;
          RTypeInstruction rType;
          GenericInstruction generic;
          int offset = 0;

          inst = tableInstructions->seach(tableInstructions->list,"sltu",compareString);

          rType.acess.op = 0;
          rType.acess.funct = GetCode(inst);
          rType.acess.rd = At;
          rType.acess.shamt = 0;
          
          for(i = 1; i <= 2; i++)
          { 
            txtOperating = GetLexical(temp,i);

            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            }

            switch(i)
            {
              case 1:
                      rType.acess.rt = GetCode(operating);
                      break;

              case 2: rType.acess.rs = GetCode(operating);
                      break;

              default:
                    break; 
            }
        }

          inst = tableInstructions->seach(tableInstructions->list,"bne",compareString);

          iType.acess.op = GetCode(inst);
          iType.acess.rs = At;
          iType.acess.rt = Zero;

          target = GetLexical(temp,3);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }

          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }

          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            generic.wordu = iType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;
            generic.wordu = rType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;       

        }
        else if(strcmp(GetInText(instrc),"blt") == 0)
        {
          unsigned i;
          int qntInst = 0;
          Instruction* inst = NULL;
          Instruction* operating;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          ITypeInstruction iType;
          RTypeInstruction rType;
          GenericInstruction generic;
          int offset = 0;

          inst = tableInstructions->seach(tableInstructions->list,"slt",compareString);

          rType.acess.op = 0;
          rType.acess.funct = GetCode(inst);
          rType.acess.rd = At;
          rType.acess.shamt = 0;
          
          for(i = 1; i <= 2; i++)
          { 
            txtOperating = GetLexical(temp,i);

            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            }

            switch(i)
            {
              case 1:
                      rType.acess.rs = GetCode(operating);
                      break;

              case 2: rType.acess.rt = GetCode(operating);
                      break;

              default:
                    break; 
            }
        }

          inst = tableInstructions->seach(tableInstructions->list,"bne",compareString);

          iType.acess.op = GetCode(inst);
          iType.acess.rs = At;
          iType.acess.rt = Zero;

          target = GetLexical(temp,3);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }

          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }

          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            generic.wordu = iType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;
            generic.wordu = rType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;       

        }
        else if(strcmp(GetInText(instrc),"bltu") == 0)
        {
          unsigned i;
          int qntInst = 0;
          Instruction* inst = NULL;
          Instruction* operating;
          char* target;
          char* txtOperating;
          LexicalToken* lexical;
          ITypeInstruction iType;
          RTypeInstruction rType;
          GenericInstruction generic;
          int offset = 0;

          inst = tableInstructions->seach(tableInstructions->list,"sltu",compareString);

          rType.acess.op = 0;
          rType.acess.funct = GetCode(inst);
          rType.acess.rd = At;
          rType.acess.shamt = 0;
          
          for(i = 1; i <= 2; i++)
          { 
            txtOperating = GetLexical(temp,i);

            if(txtOperating == NULL || strlen(txtOperating) == 0)
            {
              status = newStatus(GetLexicalObj(temp),MissingArguments);
              return status;
            }
            operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
            if(operating ==  NULL)
            {
              status = newStatus(GetLexicalObj(temp),OperatingInvalid);
              return status;
            }

            switch(i)
            {
              case 1:
                      rType.acess.rs = GetCode(operating);
                      break;

              case 2: rType.acess.rt = GetCode(operating);
                      break;

              default:
                    break; 
            }
        }

          inst = tableInstructions->seach(tableInstructions->list,"bne",compareString);

          iType.acess.op = GetCode(inst);
          iType.acess.rs = At;
          iType.acess.rt = Zero;

          target = GetLexical(temp,3);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }

          lexical = labels->seach(labels->list,target,compareLabel);

          if(lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }

          if(g_ascii_isdigit(lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            generic.wordu = iType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;
            generic.wordu = rType.wordu;
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1;       

        }
        else if(strcmp(GetInText(instrc),"bnez") == 0)
        {

          GenericInstruction generic;
          ITypeInstruction iType;
          unsigned i;
          int qntInst = 0;
          Instruction* operating = NULL;
          char* target;
          char* txtOperating;
          LexicalToken* _lexical;
          unsigned offset;

          operating = tableInstructions->seach(tableInstructions->list,"bne",compareString);

          iType.acess.op = GetCode(operating);
        

          txtOperating = GetLexical(temp,1);

          if(txtOperating == NULL || strlen(txtOperating) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
          if(operating ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          }

            
          iType.acess.rs = GetCode(operating);
          iType.acess.rt = Zero;

          target = GetLexical(temp,2);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          _lexical = labels->seach(labels->list,target,compareLabel);

          if(_lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }
          if(g_ascii_isdigit(_lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(_lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(_lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(_lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(_lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(_lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            
            generic.word = iType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 

        }
        else if(strcmp(GetInText(instrc),"beqz") == 0)
        {
          GenericInstruction generic;
          ITypeInstruction iType;
          unsigned i;
          int qntInst = 0;
          Instruction* operating = NULL;
          char* target;
          char* txtOperating;
          LexicalToken* _lexical;
          unsigned offset;

          operating = tableInstructions->seach(tableInstructions->list,"beq",compareString);
        
          iType.acess.op = GetCode(operating);
        
          txtOperating = GetLexical(temp,1);

          if(txtOperating == NULL || strlen(txtOperating) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          operating = tableInstructions->seach(tableInstructions->list,txtOperating,compareString);
               
          if(operating ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          }

         
          iType.acess.rs = GetCode(operating);
          iType.acess.rt = Zero;

          target = GetLexical(temp,2);

          if(target == NULL || strlen(target) == 0)
          { 
            status = newStatus(GetLexicalObj(temp),MissingArguments);
            return status;
          }
          _lexical = labels->seach(labels->list,target,compareLabel);

          if(_lexical ==  NULL)
          {
            status = newStatus(GetLexicalObj(temp),SymbolNotFoundInTable);
            return status;
          }
          if(g_ascii_isdigit(_lexical->tokens[0][0]) == TRUE)
          {
            status = newStatus(GetLexicalObj(temp),OperatingInvalid);
            return status;
          } 

          if(GetLexicalLine(_lexical) < GetLexicalObjLine(temp))
          {
            offset = GetLexicalObjLine(temp) - GetLexicalLine(_lexical);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalLine(_lexical)+i;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            iType.acess.addressOrConstant = (unsigned short)((unsigned short)qntInst + 1);
            iType.acess.addressOrConstant = -iType.acess.addressOrConstant;
          }
          else if(GetLexicalLine(_lexical) > GetLexicalObjLine(temp))
          {
            offset = GetLexicalLine(_lexical) - GetLexicalObjLine(temp);

            for(i = 0; i < offset; i++)
            {
              LexicalToken* lexicalTemp;
              int numberLine = GetLexicalObjLine(temp)+i+1;
              lexicalTemp = instructions->seach(instructions->list,&numberLine,compareInt);

              if(lexicalTemp != NULL)
              {
                qntInst += 1;
              }

            }
            
            iType.acess.addressOrConstant = (unsigned short)qntInst;
          }
            
            generic.word = iType.word; 
            ((GenericInstruction*)buffer)[index] = generic;
            index += 1; 

        }
        else if(strcmp(GetInText(instrc),"lw") == 0)
        {




        }
        else if(strcmp(GetInText(instrc),"sw") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"lbu") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"lb") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"sb") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"sh") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"lh") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"lwl") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"lhu") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"lwr") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"lwc1") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"swc1") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"bclf") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"swl") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"swr") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"sdc1") == 0)
        {



        }
        else if(strcmp(GetInText(instrc),"sc") == 0)
        {



        }
        
	}
  if(instructions->list != NULL)
    if(loadMemoryFromVector(buffer,index-1) == false)
    {
     status = newStatus(NULL,LoadFailed);
     return status;
    }

    status = newStatus(NULL,Success);
    return status;
}