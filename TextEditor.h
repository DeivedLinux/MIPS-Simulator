/*
 * TextEditor.h
 *
 *  Created on: 10/05/2017
 *      Author: Deived William Jal Windows
 */

#ifndef TEXTEDITOR_H_
#define TEXTEDITOR_H_

#include <gtk/gtk.h>
#include "ArrayList.h"
#include <gtksourceview/gtksource.h>

typedef struct 
{
  GtkSourceBuffer *textSourceBuffer;
  GtkWidget *scrollTextWindow;
  GtkWidget *nameFile;
  GtkWidget *textArea;
  GtkSourceFile *sourceFile;
  unsigned key;
}TextEditorArea;

typedef struct 
{
  ArrayList *textEdit;
  unsigned tabQnt;
  void *tabPage;
  GtkSourceLanguage *syntaxHighlighted;
  GtkSourceLanguageManager* sourceLanguages;
}TextEditor;

TextEditor* newTextEditor();
TextEditorArea* newTextEditorArea();



#endif