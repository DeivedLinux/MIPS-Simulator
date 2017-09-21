#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include "ArrayList.h"
#include "LexicalAnalyzer.h"

#define GetInText(Obj)   Obj->InstText
#define GetCode(Obj)     Obj->code


typedef struct 
{
	char*InstText;
	unsigned code;
}Instruction;

typedef enum 
{
	MissingArguments = 0,
	IncorrectInstruction,
	OperatingInvalid,
	LoadFailed,
	SymbolNotFoundInTable,
	Success
}Response;

typedef struct 
{
	LexicalToken* lexical;
    Response response;
}Status;




Status* assemblerCode(ArrayList* labels, ArrayList* instructions);


#endif