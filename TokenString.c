/*
 * TokenString.c
 *
 *  Created on: 10/05/2017
 *  Author: Deived William
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "TokenString.h"
#include <stdbool.h>

bool isOnlyEspace(String token)
{
  unsigned i;

  for(i = 0; token[i] != 0; i++)
    if(token[i] != ' ')
    {
      return false;
    }

  return true;
}

bool clearToken(String token)
{
  int size;

 if(token == NULL) return false;
  
  size  = strlen(token);
  memset(token,0,size);

  return true;
}

static bool isEqualChar(String string, Character character)
{
   unsigned i;

   for(i = 0; string[i] != 0; i++)
   {
      if(string[i] == character)
        return true;
   } 
   return false;

}

static bool atLeastChar(String string, Character character)
{
   unsigned i;

   for(i = 0; string[i] != 0; i++)
   {
      if(string[i] != character)
        return true;
   } 
   return false;

}

void cpyString(String source, String dest)
{
   unsigned i = 0;

   for(i = 0; source[i] != 0; i++)
   {
      dest[i] = source[i];
   }
}

String getLineText(String string, unsigned line, unsigned numberOfLines)
{
   String text;
   unsigned index = 0;
   unsigned i;

   for(i = 0; i < numberOfLines; i++)
   {
      
      if(string[i] == 10)
         index += 1;

      if(index == line)
      {
         text = (String)calloc(getLength(&string[i])+1,sizeof(Character));
         cpyString(&string[i],text);

         return text;
      }

   }
}


unsigned getLength(String string)
{
   unsigned i = 0;

   while(string[i] != '\n')
      i++;

   return i;
}

static void clearBuffString(String string)
{
	unsigned i = 0;

	while(string[i] != 0)
	{
		string[i] = 0;
		i++;
	}
}


String* StringTokenizer(char* string,...)
{
   unsigned char i = 0;
   unsigned j = 0;
   int size = strlen(string);
   va_list args;
   String arguments;
   volatile String text = NULL;
   String tempString = (String)calloc(100,sizeof(char));
   unsigned tokens = 0;

   String *vectorString = (String*)calloc(100 ,sizeof(String));


   for(i = 0; i < 20; i++)
   {
    vectorString[i] = NULL;
   }
   va_start(args,string);
 
   i = 0;
   do
   {
     arguments = va_arg(args, String);
     if(arguments != NULL)text = arguments;
    }while(arguments != NULL);

    va_end(args);

   if(text == NULL)
   {
     text = (String)calloc(strlen(" ")+1,sizeof(Character));
     strcpy(text," ");
   }
  
   i = 0;
   
   while((i < size))
   {
     
      if(atLeastChar(text,string[i]))
      {
      
        for(j = 0; j <= (size - i); j++)
        {
          
          if(string[i + j] == 0) { goto force;}
            if(!isEqualChar(text,string[i + j]))
            {
             tempString[j] = string[i + j];
             
            }


          else
          {             
            int sizeTemp;           
            
            force:
            sizeTemp = strlen(tempString);
            vectorString[tokens] = (Character*)calloc((sizeTemp+10),sizeof(Character));
            strcpy(vectorString[tokens],tempString);
            vectorString[tokens][sizeTemp+1] = 0;
            memset(tempString,0,100);
            tokens += 1;
            i += j;
                            
              break;
          }
        }
      }

     
      i++;
   }

   free(tempString);
   free(arguments);
   
  
   return vectorString;
}


String aceptDirectory(String string)
{
   unsigned i = 0;
   String res;

   res = (String)calloc(strlen(string)+1,sizeof(Character));
   
   res[3] = 0x5C;
   res[2] = 0x5C;
   res[0] = 'C';
   res[1] = ':';

   for(i = 4; i <= strlen(string); i++)
   {
      if(string[i-1] == 0x5C)
         res[i] = '/';
      else res[i] = string[i-1];
   }
   return res;
}
String getNameFromDirectory(String dir)
{
   signed i;
   String textName;
   unsigned size = strlen(dir);

   for(i = size; i >=0; i--)
   {
      if(dir[i] == '/')
         break;
   }
   i += 1;

      textName = (String)calloc(i + 1,sizeof(Character));
   unsigned temp = i;
   for(i = temp; dir[i] != 0; i++)
      textName[i - temp] = dir[i];
    
    return textName;

}