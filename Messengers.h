/*
 * Messengers.h
 *
 *  Created on: 27 de jun de 2017
 *      Author: Deived William Jal Windows
 */

#ifndef MESSENGERS_H_
#define MESSENGERS_H_

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

#define GetSBuffer(Obj) Obj->sourceBufferOut
#define GetTBuffer(Obj) Obj->textBufferOut
#define GetStart(Obj)   Obj->startLine
#define GetEnd(Obj)     Obj->endLine


typedef struct 
{
	GtkSourceBuffer* sourceBufferOut;
	GtkTextBuffer* textBufferOut;
	GtkTextIter* startLine;
	GtkTextIter* endLine;
}MessengersOutPut;

void sendMessengers(MessengersOutPut* output, const char* messengers);

MessengersOutPut *newMessengers();





#endif