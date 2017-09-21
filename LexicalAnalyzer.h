/*
 * LexicalAnalyzer.h
 *
 *  Created on: 27 de jun de 2017
 *      Author: Deived William Jal Windows
 */



#ifndef LEXICALANALYZER_H_
#define LEXICALANALYZER_H_

#include "GenericTypeDefs.h"
#include "ArrayList.h"
#include <gtksourceview/gtksource.h>
#include <stdbool.h>


#define GetStringToken0(Obj) ((LexicalToken*)(Obj))->tokens[0]
#define ClearState(Obj)  Obj = 0xFF

typedef enum
{
	UNKNOWN = 0,
	DATA_SEGMENT,
	TEXT_SEGMENT,
	INSTRUCTION,
	DIRECTIVA,
	LABEL,
	COMMENT
}TextType;

typedef struct 
{
	char **tokens;
	int line;
	int column;
    unsigned textType;
}LexicalToken;

typedef enum 
{
	_Byte = 0,
	_Half,
	_Word,
	_Float,
	_DWord,
	_Ascii,
	_Asciiz,
	_space,
	_align,
	_Unknown
}DataTypeDirectiva;

typedef struct 
{
	ArrayList *labels;
	ArrayList *instructions;
}FeedBackMount;




FeedBackMount *mountTokens(GtkSourceBuffer *textSourceBuffer, GtkTextBuffer *textBuffer);
LexicalToken* newLexicalToken(char** _tokens, int _line, int _column, unsigned _textType);
TextType checkTextType(char** tokens);
TextType checkSegment(String tokens, TextType currentSegment);
DataTypeDirectiva checkDataTypeDirective(String token);
bool checkAsp(String asp);
bool isInstruction(String tokens);


#endif