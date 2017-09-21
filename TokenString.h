/*
 * TokenString.h
 *
 *  Created on: 10/05/2017
 *  Author: Deived William Jal Windows
 */

#ifndef TOKENSTRING_H_
#define TOKENSTRING_H_

#include "GenericTypeDefs.h"
#include <stdbool.h>


bool clearToken(String token);


String* StringTokenizer(char* string,...);  
String aceptDirectory(String string);
String getNameFromDirectory(String dir);
unsigned getLength(String string);
String getLineText(String string, unsigned line, unsigned numberOfLines);
void cpyString(String source, String dest);
char **mountToken(String string);
bool isOnlyEspace(String token);
#endif