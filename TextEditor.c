#include "TextEditor.h"
#include <stdlib.h>
#include <stdio.h>



TextEditor* newTextEditor()
{
	TextEditor *textEditor;

	textEditor = (TextEditor*)malloc(sizeof(TextEditor));
    if(textEditor == NULL)
    {
       printf("%s Linha: %i, Não foi possivel Alocar\n",__FUNCTION__, __LINE__);
       return NULL;
    }
    
	textEditor->textEdit = newArrayList();
	textEditor->tabQnt = 0;

	return textEditor;
}
TextEditorArea* newTextEditorArea()
{
	TextEditorArea *textEditorArea;

	textEditorArea = (TextEditorArea*)malloc(sizeof(TextEditorArea));

	if(textEditorArea == NULL)
    {
       printf("%s Linha: %i, Não foi possivel Alocar\n",__FUNCTION__, __LINE__);
       return NULL;
    }
    return textEditorArea;
}