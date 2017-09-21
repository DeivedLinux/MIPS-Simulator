/*
 * LexicalAnalyzer.c
 *
 *  Created on: 11/05/2017
 *      Author: Deived William Jal Windows
 */

#include "LexicalAnalyzer.h"
#include "TokenString.h"
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Messengers.h"
#include <stdbool.h>
#include <glib.h>


static const char *setMipsInstructions[] = 
{
  "add",
  "addu",
  "sub",
  "subu",
  "mult",
  "multu",
  "mul",
  "madd",
  "maddu",
  "msub",
  "msubu",
  "div",
  "divu",
  "slt",
  "sltu",
  "and",
  "or",
  "nor",
  "xor",
  "addi",
  "addiu",
  "slti",
  "sltiu",
  "andi",
  "ori",
  "xori",
  "nop",
  "sll",
  "srl",
  "sllv",
  "sra",
  "srav",
  "srlv",
  "teq",
  "tge",
  "tgeu",
  "tlt",
  "tltu",
  "mthi",
  "mtlo",
  "movz",
  "movn",
  "trunc",
  "syscall",
  "break",
  "clz",
  "clo",
  "lw",
  "sw",
  "lbu",
  "lb",
  "sb",
  "sh",
  "lui",
  "lh",
  "lwl",
  "lhu",
  "lwr",
  "beq",
  "bne",
  "blez",
  "bgtz",
  "bltz",
  "lwc1",
  "swc1",
  "bclf",
  "ll",
  "swl",
  "swr",
  "sdc1",
  "sc",
  "teqi",
  "tgei",
  "tgeiu",
  "tlti",
  "tltiu",
  "jr",
  "jalr",
  "j",
  "jal",
  "li",
  "la",
  "add.s",
  "sub.s",
  "mul.s",
  "div.s",
  "sqrt.s",
  "abs",
  "abs.s",
  "mov.s",
  "mov.d",
  "neg.s",
  "round.w.s",
  "round.w.d",
  "ceil.w.s",
  "ceil.w.d",
  "floor.w.s",
  "floor.w.d",
  "cvt.w.s",
  "cvt.w.d",
  "cvt.s.w",
  "cvt.s.d",
  "cvt.d.w",
  "cvt.d.s",
  "mfc1",
  "mfc0",
  "mfc1d",
  "mtc0",
  "bgt",
  "blt",
  "bge",
  "ble",
  "bgtu",
  "bgtz",
  "bgez",
  "bltzal",
  "bgezal",
  "b",
  "beqz",
  "rem",
  "bgeu",
  "bltu",
  "bnez",
  "beqz",
  NULL
};

static bool compareString(void* elem, void* info)
{
   if(strcmp((((LexicalToken*)elem)->tokens[0]),(((LexicalToken*)info)->tokens[0])) == 0) 
   {
      return true;
   }

   return false;
}

void printList(List* list)
{
  List* temp = NULL;

  for(temp = list; temp != NULL; temp = temp->next)
  {
    printf("%i\n",((((LexicalToken*)(temp->information)))->line));
  }

}
bool isInstruction(String tokens)
{
  unsigned i;

   for(i = 0; setMipsInstructions[i] != NULL; i++)
    {
        if((strcmp(setMipsInstructions[i],tokens) == 0) && (!isOnlyEspace(tokens)))

          return true;
    }

     return false;
}

LexicalToken* newLexicalToken(char **_tokens, int _line, int _column, unsigned _textType)
{
	LexicalToken* lexicalToken;
  unsigned i;
  int size;

	lexicalToken = (LexicalToken*)malloc(sizeof(LexicalToken));
	lexicalToken->line = _line;
	lexicalToken->column = _column;
	lexicalToken->textType = _textType;

  lexicalToken->tokens = (char**)calloc(20,sizeof(char*));

  for(i = 0; i < 20; i++)
  {
     lexicalToken->tokens[i] == NULL;
  }

  for(i = 0; _tokens[i] != NULL; i++)
  {
    size = strlen(_tokens[i]);
    lexicalToken->tokens[i] = (char*)calloc(size+1,sizeof(char*));
    strcpy(lexicalToken->tokens[i],_tokens[i]);
  }

    return lexicalToken;
}
bool checkAsp(String token)
{
    unsigned i;


}

DataTypeDirectiva checkDataTypeDirective(String token)
{   

     if(strcmp(".byte",token) == 0)
       return _Byte;

     else if(strcmp(".double",token) == 0)
       return _DWord;
     
     else if(strcmp(".float",token) == 0)
       return _Float;

     else if(strcmp(".half",token) == 0)
       return _Half;

     else if(strcmp(".word",token) == 0)
       return _Word;
   else  return _Unknown;


}
TextType checkSegment(String tokens, TextType currentSegment)
{
    if(strcmp(".data",tokens) == 0)
       return DATA_SEGMENT;

     else if(strcmp(".text",tokens) == 0)
       return TEXT_SEGMENT;

     else return currentSegment; 
}

TextType checkTextType(char** tokens)
{
    unsigned i = 0;
    
    for(i = 0; tokens[0][i] != 0; i++)
    {
        if(tokens[0][i] == ':')
        {
            return LABEL;
        }
    }

    if(tokens[0][0] == '#')
        return COMMENT;


}

FeedBackMount *mountTokens(GtkSourceBuffer *textSourceBuffer, GtkTextBuffer *textBuffer)
{
	  ArrayList *listInstructions;
    ArrayList *listLabel;
	  GtkTextIter start;
    GtkTextIter end;
    char *text;
    unsigned numberOfLines;
    unsigned counter = 0;
    TextType textType;
    TextType segmentType = UNKNOWN;
    FeedBackMount* mount = (FeedBackMount*)malloc(sizeof(FeedBackMount));
    MessengersOutPut* output = newMessengers();
    char **tokens;
    bool dataSegmentsFound = false;
    bool textSegmentsFound = false;

    unsigned i;

   	listInstructions = newArrayList();
    listLabel = newArrayList();

    mount->labels = listLabel;
    mount->instructions = listInstructions;
  
    numberOfLines = gtk_text_buffer_get_line_count(GTK_TEXT_BUFFER(textSourceBuffer));
    output->sourceBufferOut = textSourceBuffer;
    output->textBufferOut = textBuffer;

    while((counter < numberOfLines))
    {
      	gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textSourceBuffer),&start,counter);
        gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textSourceBuffer),&end,counter+1);
        text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textSourceBuffer),&start,&end,FALSE); 

        if(strlen(text) > 0)
            tokens = g_strsplit_set(text," ,\n",-1); 
         
        if(tokens[0] != NULL);
           segmentType = checkSegment(tokens[0],segmentType);


        if(segmentType == DATA_SEGMENT)
        {
          dataSegmentsFound = true;
        
          for(i = 1; i < numberOfLines; i++)
          {

            gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textSourceBuffer),&start,counter+i);
            gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textSourceBuffer),&end,counter+1+i);
            text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textSourceBuffer),&start,&end,FALSE); 

            if(strlen(text) > 0)
                tokens = g_strsplit_set(text," ,\n",-1); 


            if(tokens[0] != NULL);
                 segmentType = checkSegment(tokens[0],segmentType);
            if(segmentType == TEXT_SEGMENT)
            {
             
               break;
            }

            if(checkTextType(tokens) == LABEL)
            {
                DataTypeDirectiva dataTypeDirectiva;
                LexicalToken *lexicalToken;
  
                
                if((tokens[1] != NULL) && (!isOnlyEspace(tokens[1])))
                {

                 dataTypeDirectiva = checkDataTypeDirective(tokens[1]);

                 if(dataTypeDirectiva != _Unknown)
                 {
                   lexicalToken = newLexicalToken(tokens,i+counter,0,dataTypeDirectiva);
                   LexicalToken *temp = listLabel->seach(listLabel->list,lexicalToken,compareString);
                   if(temp == NULL)
                   {
                     listLabel->addArray(lexicalToken,&(listLabel->list));
                   }
                    else 
                   {
                    output->startLine = &start;
                    output->endLine = &end;
                    sendMessengers(output," : esta Label já foi definida!");
                    return NULL;
                   }
                 }
                 else
                 {
                    output->startLine = &start;
                    output->endLine = &end;
                    sendMessengers(output," :Label Inválida!");
                    return NULL;
                 }
                }
                else
                {
                   lexicalToken = newLexicalToken(tokens,i+counter,0,dataTypeDirectiva);
                   LexicalToken *temp = listLabel->seach(listLabel->list,lexicalToken,compareString);
                   if(temp == NULL)
                   {
                     listLabel->addArray(lexicalToken,&(listLabel->list));
                   }
                    else 
                  {
                    output->startLine = &start;
                    output->endLine = &end;
                    sendMessengers(output," : esta Label já foi definida!");
                    return NULL;
                  }

                }

                 clearToken(tokens[1]);
                 clearToken(tokens[0]);
            }
                   
        }
         counter += i;

     }
        if(segmentType == TEXT_SEGMENT)
        {
           textSegmentsFound = true;
           TextType textTypeSegmentText;


           for(i = 0; i <= (numberOfLines-counter); i++)
           {
              gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textSourceBuffer),&start,counter+i);
              gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textSourceBuffer),&end,counter+1+i);
              text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textSourceBuffer),&start,&end,FALSE); 

              if(strlen(text) > 0)
                tokens = g_strsplit_set(text," ,\n",-1); 


            if(tokens[0] != NULL);
                 segmentType = checkSegment(tokens[0],segmentType);

            if((segmentType == DATA_SEGMENT) && (dataSegmentsFound == true))
            {
              output->startLine = &start;
              output->endLine = &end;
              sendMessengers(output," :dois Segmentos de Dados?");   
              return NULL;
            }
             if(!isOnlyEspace(tokens[0]) && tokens[0][0] != '\n')
                    textTypeSegmentText = checkTextType(tokens);
            if(textTypeSegmentText == LABEL)
            {
              if((tokens[1] != NULL) && (!isOnlyEspace(tokens[1])) && (g_ascii_isdigit(tokens[0][0]) == TRUE))
              {
                output->startLine = &start;
                output->endLine = &end;
                sendMessengers(output," :Label Inválida!");
                return NULL;

              }

              else
              {
                LexicalToken *lexicalToken;
                lexicalToken = newLexicalToken(tokens,i+counter,0,LABEL);
                LexicalToken *temp = listLabel->seach(listLabel->list,lexicalToken,compareString);
                if(temp == NULL)
                {
                  listLabel->addArray(lexicalToken,&(listLabel->list));
                  ClearState(textTypeSegmentText);
                }
                else 
                {
                  output->startLine = &start;
                  output->endLine = &end;
                  sendMessengers(output," :esta Label já foi definida!");
                  return NULL;
                }
              }

              clearToken(tokens[1]);
              clearToken(tokens[0]);
            }
             else if(textTypeSegmentText == COMMENT)
             {
                 i += 1;
             }
             else if(isInstruction(tokens[0]))
             {
                 LexicalToken *lexicalToken = newLexicalToken(tokens,i+counter,0,INSTRUCTION);
                 listInstructions->addArray(lexicalToken,&(listInstructions->list));
             }
            

               clearToken(tokens[0]);
            }

           counter += i;
        }



        counter++;
  }
  
       return mount;
}