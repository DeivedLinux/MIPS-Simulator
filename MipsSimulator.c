 /*
 ============================================================================
 Name        : MipsSimulator.c
 Author      : Deived William
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in GTK+
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Processador.h"
#include "TokenString.h"
#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>
#include <string.h>
#include "ArrayList.h"
#include "TextEditor.h"
#include "LabelsRegisters.h"
#include <stdbool.h>
#include "LexicalAnalyzer.h"
#include "Assembler.h"
#include "Messengers.h"
 

/* Como compilar o programa*/
/* gcc `pkg-config --cflags gtk+-3.0 gtksourceview-3.0` -o 
run MipsSimulator.c Processador.c TokenString.c Memoria.c ArrayList.c
 AnalisadorSintaxe.c TextEditor.c `pkg-config --libs gtk+-3.0 
 gtksourceview-3.0`
*/

static void saveFileSelect(GtkMenuItem *menuitem, gpointer input);

void switchPage(GtkNotebook *notebook, GtkWidget   *page, guint page_num, gpointer user_data)
{
  gtk_widget_show_all(page);
}

bool compareTo(void* elem, void* info)
{
   return *((gint*)elem) == ((TextEditorArea*)info)->key?1:0;
}


typedef struct 
{
  TextEditor *_textEditor;
  GtkTextBuffer *messengers;  
  GtkWidget* mainNote;
  GtkWidget* noteIter;
}MainInformation;

static void undoToolBarClicked(GtkToolButton *toolbutton, gpointer input)
{
  unsigned currentPage;
  TextEditorArea *textEditorArea;



  currentPage = gtk_notebook_get_current_page(GTK_NOTEBOOK(((TextEditor*)input)->tabPage));
  
  textEditorArea = (((TextEditor*)input)->textEdit)->seach((((TextEditor*)input)->textEdit)->list,
                                                           &currentPage,
                                                           compareTo);
  if(textEditorArea == NULL) return;
  if(gtk_source_buffer_can_undo(textEditorArea->textSourceBuffer) == TRUE)
      gtk_source_buffer_undo(textEditorArea->textSourceBuffer);

}

static void redoToolBarClicked(GtkToolButton *toolbutton, gpointer input)
{
  unsigned currentPage;
  TextEditorArea *textEditorArea;



  currentPage = gtk_notebook_get_current_page(GTK_NOTEBOOK(((TextEditor*)input)->tabPage));
  
  textEditorArea = (((TextEditor*)input)->textEdit)->seach((((TextEditor*)input)->textEdit)->list,
                                                           &currentPage,
                                                           compareTo);
  if(textEditorArea == NULL) return;
  if(gtk_source_buffer_can_redo(textEditorArea->textSourceBuffer) == TRUE)
      gtk_source_buffer_redo(textEditorArea->textSourceBuffer);

}

static void newFileToolBarClicked(GtkToolButton *toolbutton, gpointer input)
{

     TextEditorArea *textEditorArea;
     char *pageNumber = (char*)calloc(3,sizeof(char));
     char pageName[] = "prog ";
     GtkSourceLanguageManager *languageManager;
  
        
     textEditorArea = newTextEditorArea();
    
     if(textEditorArea == NULL)
     {
       printf("%s Linha: %i, Não foi possivel Alocar\n",__FUNCTION__, __LINE__);
       return;
     }
      ((TextEditor*)input)->tabQnt += 1;
      textEditorArea->textSourceBuffer = gtk_source_buffer_new(NULL); 
  
       g_object_set_data_full (G_OBJECT(textEditorArea->textSourceBuffer), "language-specs",
                            ((TextEditor*)input)->sourceLanguages,(GDestroyNotify)g_object_unref);

      textEditorArea->textArea = gtk_source_view_new_with_buffer(textEditorArea->textSourceBuffer);
        
      languageManager = g_object_get_data(G_OBJECT(textEditorArea->textSourceBuffer), "language-specs"); 
      ((TextEditor*)input)->syntaxHighlighted = gtk_source_language_manager_get_language(languageManager,"mal");

      if(((TextEditor*)input)->syntaxHighlighted == NULL)
      {
        
        gtk_source_buffer_set_highlight_syntax(textEditorArea->textSourceBuffer,FALSE);
      }
      else
      { 
        gtk_source_buffer_set_language(textEditorArea->textSourceBuffer,((TextEditor*)input)->syntaxHighlighted);
        gtk_source_buffer_set_highlight_syntax(textEditorArea->textSourceBuffer,TRUE);
      }

      textEditorArea->scrollTextWindow = gtk_scrolled_window_new(NULL,NULL);
      
      gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(textEditorArea->textArea),TRUE);
      gtk_source_view_set_auto_indent(GTK_SOURCE_VIEW(textEditorArea->textArea),TRUE);
      gtk_source_view_set_indent_on_tab(GTK_SOURCE_VIEW(textEditorArea->textArea),TRUE);

      itoa(((TextEditor*)input)->tabQnt,pageNumber,10);
      strcat(pageName,pageNumber);
     
      textEditorArea->nameFile = gtk_label_new(strcat(pageName,".asm"));
    

      gtk_notebook_insert_page(GTK_NOTEBOOK(((TextEditor*)input)->tabPage),
                              textEditorArea->scrollTextWindow,
                              textEditorArea->nameFile,((TextEditor*)input)->tabQnt);

      gtk_container_add(GTK_CONTAINER(textEditorArea->scrollTextWindow),textEditorArea->textArea);

      gtk_notebook_set_current_page(GTK_NOTEBOOK(((TextEditor*)input)->tabPage),((TextEditor*)input)->tabQnt);
      textEditorArea->key = ((TextEditor*)input)->tabQnt;

      (((TextEditor*)input)->textEdit)->addArray(textEditorArea,&(((TextEditor*)input)->textEdit->list));
       
       textEditorArea->sourceFile = NULL;

      gtk_widget_show_all(((TextEditor*)input)->tabPage);    

      free(pageNumber); 

}
static void openFileToolBarClicked(GtkToolButton *toolbutton, gpointer input)
{

  GtkFileChooserNative *native;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;
  TextEditorArea *textEditorArea;
  GtkSourceLanguageManager *languageManager;
  GFile *fileToOpen;
  GtkSourceFileLoader *sourceFileLoader;

  GtkDialogFlags dialogFlags = GTK_DIALOG_DESTROY_WITH_PARENT;
  

  textEditorArea = newTextEditorArea();


  if(textEditorArea == NULL)
  {
     printf("%s Linha: %i, Não foi possivel Alocar\n",__FUNCTION__, __LINE__);
     return;
  }
   ((TextEditor*)input)->tabQnt += 1;
    textEditorArea->textSourceBuffer = gtk_source_buffer_new(NULL); 

    g_object_set_data_full (G_OBJECT(textEditorArea->textSourceBuffer), "language-specs",
                           ((TextEditor*)input)->sourceLanguages,(GDestroyNotify)g_object_unref);

    textEditorArea->textArea = gtk_source_view_new_with_buffer(textEditorArea->textSourceBuffer);
        
    languageManager = g_object_get_data(G_OBJECT(textEditorArea->textSourceBuffer), "language-specs"); 
    ((TextEditor*)input)->syntaxHighlighted = gtk_source_language_manager_get_language(languageManager,"mal");

  if(((TextEditor*)input)->syntaxHighlighted == NULL)
   {   
      gtk_source_buffer_set_highlight_syntax(textEditorArea->textSourceBuffer,FALSE);
   }
  else
   { 
      gtk_source_buffer_set_language(textEditorArea->textSourceBuffer,((TextEditor*)input)->syntaxHighlighted);
      gtk_source_buffer_set_highlight_syntax(textEditorArea->textSourceBuffer,TRUE);
   }

      textEditorArea->scrollTextWindow = gtk_scrolled_window_new(NULL,NULL);
      
      gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(textEditorArea->textArea),TRUE);
      gtk_source_view_set_auto_indent(GTK_SOURCE_VIEW(textEditorArea->textArea),TRUE);
      gtk_source_view_set_indent_on_tab(GTK_SOURCE_VIEW(textEditorArea->textArea),TRUE);
  
      native = gtk_file_chooser_native_new ("Open File",NULL, action ,"_Open", "_Cancel");

      res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(native));
  
  if(res == GTK_RESPONSE_ACCEPT)
  { 
     char *filename;
     String dir;
     GtkFileChooser *chooser = GTK_FILE_CHOOSER(native);
     filename = gtk_file_chooser_get_filename(chooser);
     dir = aceptDirectory(filename);

     fileToOpen = g_file_new_for_path(dir);

     textEditorArea->sourceFile = gtk_source_file_new();

     gtk_source_file_set_location(textEditorArea->sourceFile,fileToOpen);

     sourceFileLoader = gtk_source_file_loader_new(textEditorArea->textSourceBuffer,textEditorArea->sourceFile);

     gtk_source_file_loader_load_async(sourceFileLoader,G_PRIORITY_HIGH,
                                       NULL,NULL,NULL,NULL,NULL,NULL);

     

     textEditorArea->nameFile = gtk_label_new(getNameFromDirectory(dir));

     gtk_notebook_insert_page(GTK_NOTEBOOK(((TextEditor*)input)->tabPage),
                              textEditorArea->scrollTextWindow,
                              textEditorArea->nameFile,((TextEditor*)input)->tabQnt);

     gtk_container_add(GTK_CONTAINER(textEditorArea->scrollTextWindow),textEditorArea->textArea);

    (((TextEditor*)input)->textEdit)->addArray(textEditorArea,&(((TextEditor*)input)->textEdit->list));
     textEditorArea->key = ((TextEditor*)input)->tabQnt; 
    gtk_widget_show_all(((TextEditor*)input)->tabPage);

    g_free(filename);
    g_free(dir);

 }
    g_object_unref(native);
  
}
static void saveToolBarClicked(GtkToolButton *toolbutton, gpointer input)
{

  GtkSourceFileSaver *sourceFileSaver;
  /*Caixa de dialogo nativa do sistema operacional*/
  GtkFileChooserNative *native;
  /*Interface para manusear o arquivo*/
  GtkFileChooser *fileChooser;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  gint res;
  gint currentPage;
  TextEditorArea *textEditorArea;
  GFile *fileToSave;
  String dir;
  
  currentPage = gtk_notebook_get_current_page(GTK_NOTEBOOK(((TextEditor*)input)->tabPage));

  
  textEditorArea = (((TextEditor*)input)->textEdit)->seach((((TextEditor*)input)->textEdit)->list,
                                                           &currentPage,
                                                           compareTo);
  if(textEditorArea == NULL)
  {
    return;
  }
  if(textEditorArea->sourceFile == NULL)
  {

     native = gtk_file_chooser_native_new ("Save File", NULL, action, "_Save", "_Cancel");
     fileChooser = GTK_FILE_CHOOSER(native);
     gtk_file_chooser_set_do_overwrite_confirmation(fileChooser, TRUE);

     res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(native));
     
     if(res == GTK_RESPONSE_ACCEPT)
     {
       String filename;
       filename = gtk_file_chooser_get_filename(fileChooser);
       
       dir = aceptDirectory(filename);
       fileToSave = g_file_new_for_path(dir);
       if(fileToSave == NULL)
       {

       }
       textEditorArea->sourceFile = gtk_source_file_new();
       gtk_source_file_set_location(textEditorArea->sourceFile,fileToSave);
       gtk_label_set_text(GTK_LABEL(textEditorArea->nameFile),getNameFromDirectory(dir));
       gtk_widget_show_all(((TextEditor*)input)->tabPage); 

       g_free(dir);
       g_free (filename);
     }


       g_object_unref(native);
 }
       sourceFileSaver = gtk_source_file_saver_new(textEditorArea->textSourceBuffer,
                                                  textEditorArea->sourceFile);
       gtk_source_file_saver_save_async(sourceFileSaver,G_PRIORITY_HIGH,
                                        NULL,NULL,NULL,NULL,NULL,NULL);

  
}
static void saveAsToolBarClicked(GtkToolButton *toolbutton, gpointer input)
{
   unsigned currentPage;
   TextEditorArea *textEditorArea;
   GSList *list;
   GtkWidget *tst;
   GtkSourceFileSaver *sourceFileSaver;
    /*Caixa de dialogo nativa do sistema operacional*/
   GtkFileChooserNative *native;
    /*Interface para manusear o arquivo*/
   GtkFileChooser *fileChooser;
   GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
   gint res;
   GFile *fileToSave;
   String dir;

   currentPage = gtk_notebook_get_current_page(GTK_NOTEBOOK(((TextEditor*)input)->tabPage));
  
   textEditorArea = (((TextEditor*)input)->textEdit)->seach((((TextEditor*)input)->textEdit)->list,
                                                           &currentPage,
                                                           compareTo);
   if(textEditorArea == NULL)
   {
     return;
   }
   
     native = gtk_file_chooser_native_new ("Save File", NULL, action, "_Save", "_Cancel");
     fileChooser = GTK_FILE_CHOOSER(native);
     gtk_file_chooser_set_do_overwrite_confirmation(fileChooser, TRUE);

     res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(native));
     
     if(res == GTK_RESPONSE_ACCEPT)
     {
       String filename;
       filename = gtk_file_chooser_get_filename(fileChooser);
       
       dir = aceptDirectory(filename);
       fileToSave = g_file_new_for_path(dir);
       if(fileToSave == NULL)
       {

       }
       if(textEditorArea == NULL)
           textEditorArea->sourceFile = gtk_source_file_new();
       gtk_source_file_set_location(textEditorArea->sourceFile,fileToSave);
       gtk_label_set_text(GTK_LABEL(textEditorArea->nameFile),getNameFromDirectory(dir));
       gtk_widget_show_all(((TextEditor*)input)->tabPage); 

       g_free(dir);
       g_free (filename);
     }


       g_object_unref(native);
       sourceFileSaver = gtk_source_file_saver_new(textEditorArea->textSourceBuffer,
                                                  textEditorArea->sourceFile);
       gtk_source_file_saver_save_async(sourceFileSaver,G_PRIORITY_HIGH,
                                        NULL,NULL,NULL,NULL,NULL,NULL);


}
static void saveAllToolBarClicked(GtkToolButton *toolbutton, gpointer input)
{
    unsigned qntPages; 
    unsigned i;
    TextEditorArea *textEditorArea;
    GtkSourceFileSaver *sourceFileSaver;
    /*Caixa de dialogo nativa do sistema operacional*/
    GtkFileChooserNative *native;
    /*Interface para manusear o arquivo*/
    GtkFileChooser *fileChooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;
    GFile *fileToSave;
    String dir;
  
    qntPages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(((TextEditor*)input)->tabPage));




    for(i = 1; i < qntPages; i++)
    {
      textEditorArea = (((TextEditor*)input)->textEdit)->seach((((TextEditor*)input)->textEdit)->list,
                                                           &i,
                                                           compareTo);
       if(textEditorArea->sourceFile == NULL)
       {

          native = gtk_file_chooser_native_new ("Save File", NULL, action, "_Save", "_Cancel");
          fileChooser = GTK_FILE_CHOOSER(native);
          gtk_file_chooser_set_do_overwrite_confirmation(fileChooser, TRUE);

          res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(native));
     
         if(res == GTK_RESPONSE_ACCEPT)
         {
           String filename;
           filename = gtk_file_chooser_get_filename(fileChooser);
       
           dir = aceptDirectory(filename);
           fileToSave = g_file_new_for_path(dir);
           if(fileToSave == NULL)
           {

           }
           textEditorArea->sourceFile = gtk_source_file_new();
           gtk_source_file_set_location(textEditorArea->sourceFile,fileToSave);
           gtk_label_set_text(GTK_LABEL(textEditorArea->nameFile),getNameFromDirectory(dir));

           g_free(dir);
           g_free (filename);
          }

           sourceFileSaver = gtk_source_file_saver_new(textEditorArea->textSourceBuffer,
                                                  textEditorArea->sourceFile);
           gtk_source_file_saver_save_async(sourceFileSaver,G_PRIORITY_HIGH,
                                        NULL,NULL,NULL,NULL,NULL,NULL); 
      }
      else
      {

        sourceFileSaver = gtk_source_file_saver_new(textEditorArea->textSourceBuffer,
                                                  textEditorArea->sourceFile);
        gtk_source_file_saver_save_async(sourceFileSaver,G_PRIORITY_HIGH,
                                        NULL,NULL,NULL,NULL,NULL,NULL);     
      } 
   }
}
static void printToolBarClicked(GtkToolButton *toolbutton, gpointer input)
{
  
}

static void saveFileSelect(GtkMenuItem *menuitem, gpointer input)
{
  GtkSourceFileSaver *sourceFileSaver;
  /*Caixa de dialogo nativa do sistema operacional*/
  GtkFileChooserNative *native;
  /*Interface para manusear o arquivo*/
  GtkFileChooser *fileChooser;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  gint res;
  gint currentPage;
  TextEditorArea *textEditorArea;
  GFile *fileToSave;
  String dir;
  
  currentPage = gtk_notebook_get_current_page(GTK_NOTEBOOK(((TextEditor*)input)->tabPage));

  
  textEditorArea = (((TextEditor*)input)->textEdit)->seach((((TextEditor*)input)->textEdit)->list,
                                                           &currentPage,
                                                           compareTo);
  if(textEditorArea == NULL)
  {
    return;
  }
  if(textEditorArea->sourceFile == NULL)
  {

     native = gtk_file_chooser_native_new ("Save File", NULL, action, "_Save", "_Cancel");
     fileChooser = GTK_FILE_CHOOSER(native);
     gtk_file_chooser_set_do_overwrite_confirmation(fileChooser, TRUE);

     res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(native));
     
     if(res == GTK_RESPONSE_ACCEPT)
     {
       String filename;
       filename = gtk_file_chooser_get_filename(fileChooser);
       
       dir = aceptDirectory(filename);
       fileToSave = g_file_new_for_path(dir);
       if(fileToSave == NULL)
       {

       }
       textEditorArea->sourceFile = gtk_source_file_new();
       gtk_source_file_set_location(textEditorArea->sourceFile,fileToSave);
       gtk_label_set_text(GTK_LABEL(textEditorArea->nameFile),getNameFromDirectory(dir));
       gtk_widget_show_all(((TextEditor*)input)->tabPage); 

       g_free(dir);
       g_free (filename);
     }


       g_object_unref(native);
 }
       sourceFileSaver = gtk_source_file_saver_new(textEditorArea->textSourceBuffer,
                                                  textEditorArea->sourceFile);
       gtk_source_file_saver_save_async(sourceFileSaver,G_PRIORITY_HIGH,
                                        NULL,NULL,NULL,NULL,NULL,NULL);


}
static void openFileSelect(GtkMenuItem *menuitem, gpointer input)
{
  GtkFileChooserNative *native;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;
  TextEditorArea *textEditorArea;
  GtkSourceLanguageManager *languageManager;
  GFile *fileToOpen;
  GtkSourceFileLoader *sourceFileLoader;

  GtkDialogFlags dialogFlags = GTK_DIALOG_DESTROY_WITH_PARENT;
  

  textEditorArea = newTextEditorArea();


  if(textEditorArea == NULL)
  {
     printf("%s Linha: %i, Não foi possivel Alocar\n",__FUNCTION__, __LINE__);
     return;
  }
   ((TextEditor*)input)->tabQnt += 1;
    textEditorArea->textSourceBuffer = gtk_source_buffer_new(NULL); 

    g_object_set_data_full (G_OBJECT(textEditorArea->textSourceBuffer), "language-specs",
                           ((TextEditor*)input)->sourceLanguages,(GDestroyNotify)g_object_unref);

    textEditorArea->textArea = gtk_source_view_new_with_buffer(textEditorArea->textSourceBuffer);
        
    languageManager = g_object_get_data(G_OBJECT(textEditorArea->textSourceBuffer), "language-specs"); 
    ((TextEditor*)input)->syntaxHighlighted = gtk_source_language_manager_get_language(languageManager,"mal");

  if(((TextEditor*)input)->syntaxHighlighted == NULL)
   {   
      gtk_source_buffer_set_highlight_syntax(textEditorArea->textSourceBuffer,FALSE);
   }
  else
   { 
      gtk_source_buffer_set_language(textEditorArea->textSourceBuffer,((TextEditor*)input)->syntaxHighlighted);
      gtk_source_buffer_set_highlight_syntax(textEditorArea->textSourceBuffer,TRUE);
   }

      textEditorArea->scrollTextWindow = gtk_scrolled_window_new(NULL,NULL);
      
      gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(textEditorArea->textArea),TRUE);
      gtk_source_view_set_auto_indent(GTK_SOURCE_VIEW(textEditorArea->textArea),TRUE);
      gtk_source_view_set_indent_on_tab(GTK_SOURCE_VIEW(textEditorArea->textArea),TRUE);
  
      native = gtk_file_chooser_native_new ("Open File",NULL, action ,"_Open", "_Cancel");

      res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(native));
  
  if(res == GTK_RESPONSE_ACCEPT)
  { 
     char *filename;
     String dir;
     GtkFileChooser *chooser = GTK_FILE_CHOOSER(native);
     filename = gtk_file_chooser_get_filename(chooser);
     dir = aceptDirectory(filename);

     fileToOpen = g_file_new_for_path(dir);

     textEditorArea->sourceFile = gtk_source_file_new();

     gtk_source_file_set_location(textEditorArea->sourceFile,fileToOpen);

     sourceFileLoader = gtk_source_file_loader_new(textEditorArea->textSourceBuffer,textEditorArea->sourceFile);

     gtk_source_file_loader_load_async(sourceFileLoader,G_PRIORITY_HIGH,
                                       NULL,NULL,NULL,NULL,NULL,NULL);

     

     textEditorArea->nameFile = gtk_label_new(getNameFromDirectory(dir));

     gtk_notebook_insert_page(GTK_NOTEBOOK(((TextEditor*)input)->tabPage),
                              textEditorArea->scrollTextWindow,
                              textEditorArea->nameFile,((TextEditor*)input)->tabQnt);

     gtk_container_add(GTK_CONTAINER(textEditorArea->scrollTextWindow),textEditorArea->textArea);

    (((TextEditor*)input)->textEdit)->addArray(textEditorArea,&(((TextEditor*)input)->textEdit->list));
     textEditorArea->key = ((TextEditor*)input)->tabQnt; 
    gtk_widget_show_all(((TextEditor*)input)->tabPage);

    g_free(filename);
    g_free(dir);

 }
    g_object_unref(native);

}



static void saveAsFileSelect(GtkMenuItem *menuitem, gpointer input)
{
   unsigned currentPage;
   TextEditorArea *textEditorArea;
   GSList *list;
   GtkWidget *tst;
   GtkSourceFileSaver *sourceFileSaver;
    /*Caixa de dialogo nativa do sistema operacional*/
   GtkFileChooserNative *native;
    /*Interface para manusear o arquivo*/
   GtkFileChooser *fileChooser;
   GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
   gint res;
   GFile *fileToSave;
   String dir;

   currentPage = gtk_notebook_get_current_page(GTK_NOTEBOOK(((TextEditor*)input)->tabPage));
  
   textEditorArea = (((TextEditor*)input)->textEdit)->seach((((TextEditor*)input)->textEdit)->list,
                                                           &currentPage,
                                                           compareTo);

   if(textEditorArea == NULL)
   {
      return;
   }

   if(textEditorArea->sourceFile == NULL)
      textEditorArea->sourceFile = gtk_source_file_new();

     native = gtk_file_chooser_native_new ("Save File", NULL, action, "_Save", "_Cancel");
     fileChooser = GTK_FILE_CHOOSER(native);
     gtk_file_chooser_set_do_overwrite_confirmation(fileChooser, TRUE);

     res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(native));
     
     if(res == GTK_RESPONSE_ACCEPT)
     {
       String filename;
       filename = gtk_file_chooser_get_filename(fileChooser);
       
       dir = aceptDirectory(filename);
       fileToSave = g_file_new_for_path(dir);
       if(fileToSave == NULL)
       {

       }
       
       gtk_source_file_set_location(textEditorArea->sourceFile,fileToSave);
       gtk_label_set_text(GTK_LABEL(textEditorArea->nameFile),getNameFromDirectory(dir));
       gtk_widget_show_all(((TextEditor*)input)->tabPage); 

       g_free(dir);
       g_free (filename);
     }


       g_object_unref(native);

       sourceFileSaver = gtk_source_file_saver_new(textEditorArea->textSourceBuffer,
                                                  textEditorArea->sourceFile);
       gtk_source_file_saver_save_async(sourceFileSaver,G_PRIORITY_HIGH,
                                        NULL,NULL,NULL,NULL,NULL,NULL);
   
}
static void saveAllFileSelect(GtkMenuItem *menuitem, gpointer input)
{
    unsigned qntPages; 
    unsigned i;
    TextEditorArea *textEditorArea;
    GtkSourceFileSaver *sourceFileSaver;
    /*Caixa de dialogo nativa do sistema operacional*/
    GtkFileChooserNative *native;
    /*Interface para manusear o arquivo*/
    GtkFileChooser *fileChooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;
    GFile *fileToSave;
    String dir;
  
    qntPages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(((TextEditor*)input)->tabPage));




    for(i = 1; i < qntPages; i++)
    {
      textEditorArea = (((TextEditor*)input)->textEdit)->seach((((TextEditor*)input)->textEdit)->list,
                                                           &i,
                                                           compareTo);
    if(textEditorArea->sourceFile == NULL)
    {

     native = gtk_file_chooser_native_new ("Save File", NULL, action, "_Save", "_Cancel");
     fileChooser = GTK_FILE_CHOOSER(native);
     gtk_file_chooser_set_do_overwrite_confirmation(fileChooser, TRUE);

     res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(native));
     
     if(res == GTK_RESPONSE_ACCEPT)
     {
       String filename;
       filename = gtk_file_chooser_get_filename(fileChooser);
       
       dir = aceptDirectory(filename);
       fileToSave = g_file_new_for_path(dir);
       if(fileToSave == NULL)
       {

       }
       textEditorArea->sourceFile = gtk_source_file_new();
       gtk_source_file_set_location(textEditorArea->sourceFile,fileToSave);
       gtk_label_set_text(GTK_LABEL(textEditorArea->nameFile),getNameFromDirectory(dir));

       g_free(dir);
       g_free (filename);
     }

       sourceFileSaver = gtk_source_file_saver_new(textEditorArea->textSourceBuffer,
                                                  textEditorArea->sourceFile);
       gtk_source_file_saver_save_async(sourceFileSaver,G_PRIORITY_HIGH,
                                        NULL,NULL,NULL,NULL,NULL,NULL); 
      }
      else
      {
        sourceFileSaver = gtk_source_file_saver_new(textEditorArea->textSourceBuffer,
                                                  textEditorArea->sourceFile);
        gtk_source_file_saver_save_async(sourceFileSaver,G_PRIORITY_HIGH,
                                        NULL,NULL,NULL,NULL,NULL,NULL);     
      } 
   }

}
static void exportFileSelect(GtkMenuItem *menuitem, gpointer input)
{

}
static void printFileSelect(GtkMenuItem *menuitem, gpointer input)
{

}

static void newFileSelect(GtkMenuItem *menuitem, gpointer input)
{
     TextEditorArea *textEditorArea;
     char *pageNumber = (char*)calloc(3,sizeof(char));
     char pageName[] = "prog ";
     GtkSourceLanguageManager *languageManager;
     
          
     textEditorArea = newTextEditorArea();

    

     if(textEditorArea == NULL)
     {
       printf("%s Linha: %i, Não foi possivel Alocar\n",__FUNCTION__, __LINE__);
       return;
     }
      ((TextEditor*)input)->tabQnt += 1;
      textEditorArea->textSourceBuffer = gtk_source_buffer_new(NULL); 
  
       g_object_set_data_full (G_OBJECT(textEditorArea->textSourceBuffer), "language-specs",
                            ((TextEditor*)input)->sourceLanguages,(GDestroyNotify)g_object_unref);


      textEditorArea->textArea = gtk_source_view_new_with_buffer(textEditorArea->textSourceBuffer);
        
      languageManager = g_object_get_data(G_OBJECT(textEditorArea->textSourceBuffer), "language-specs"); 
      ((TextEditor*)input)->syntaxHighlighted = gtk_source_language_manager_get_language(languageManager,"mal");

      if(((TextEditor*)input)->syntaxHighlighted == NULL)
      {
        
        gtk_source_buffer_set_highlight_syntax(textEditorArea->textSourceBuffer,FALSE);
      }
      else
      { 
        gtk_source_buffer_set_language(textEditorArea->textSourceBuffer,((TextEditor*)input)->syntaxHighlighted);
        gtk_source_buffer_set_highlight_syntax(textEditorArea->textSourceBuffer,TRUE);
        

      }

      textEditorArea->scrollTextWindow = gtk_scrolled_window_new(NULL,NULL);
        
      gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(textEditorArea->textArea),TRUE);
      gtk_source_view_set_auto_indent(GTK_SOURCE_VIEW(textEditorArea->textArea),TRUE);
      gtk_source_view_set_indent_on_tab(GTK_SOURCE_VIEW(textEditorArea->textArea),TRUE);

      itoa(((TextEditor*)input)->tabQnt,pageNumber,10);
      strcat(pageName,pageNumber);
     
      textEditorArea->nameFile = gtk_label_new(strcat(pageName,".asm"));
    

      gtk_notebook_insert_page(GTK_NOTEBOOK(((TextEditor*)input)->tabPage),
                              textEditorArea->scrollTextWindow,
                              textEditorArea->nameFile,((TextEditor*)input)->tabQnt);

      gtk_container_add(GTK_CONTAINER(textEditorArea->scrollTextWindow),textEditorArea->textArea);

      gtk_notebook_set_current_page(GTK_NOTEBOOK(((TextEditor*)input)->tabPage),((TextEditor*)input)->tabQnt);

      textEditorArea->key = ((TextEditor*)input)->tabQnt;

      (((TextEditor*)input)->textEdit)->addArray(textEditorArea,&(((TextEditor*)input)->textEdit->list));
       
      textEditorArea->sourceFile = NULL;
      gtk_widget_show_all(((TextEditor*)input)->tabPage);    
      free(pageNumber); 
    
}

static void assembleToolBarClicked(GtkToolButton *toolbutton, gpointer input)
{
  TextEditorArea *textEditorArea;
  unsigned currentPage;
  FeedBackMount* lexicalTokens;
  Status *status;
  GtkTextIter start;
  GtkTextIter end;
  GtkTextTag *tag;


  currentPage = gtk_notebook_get_current_page(GTK_NOTEBOOK((((MainInformation*)input)->_textEditor->tabPage)));

  
  textEditorArea = (((MainInformation*)input)->_textEditor->textEdit)->seach((((MainInformation*)input)->_textEditor->textEdit)->list,
                                                           &currentPage,compareTo);
  if(textEditorArea == NULL)
  {
     return;
  }
  lexicalTokens = mountTokens(textEditorArea->textSourceBuffer,((MainInformation*)input)->messengers);
  
  if(lexicalTokens == NULL)
  {
    return;
  } 
  status = assemblerCode(lexicalTokens->labels, lexicalTokens->instructions);
  
  if(status->response == Success)
  {
     gtk_text_buffer_set_text(((MainInformation*)input)->messengers,"Montado com Sucesso",-1);
     gtk_notebook_set_current_page(GTK_NOTEBOOK((((MainInformation*)input)->mainNote)),1);
     gtk_notebook_set_current_page(GTK_NOTEBOOK((((MainInformation*)input)->noteIter)),1);
  }
  else if(status->response == LoadFailed)
  {
     gtk_text_buffer_set_text(((MainInformation*)input)->messengers,"Falha no carregamento",-1);
  }
  else if(status->response == MissingArguments)
  {
    char text[50] = "Error na Linha ";
    char row[4];

    itoa(status->lexical->line,row,10);
    strcat(text,row);
    strcat(text," Poucos Argumentos");
    gtk_text_buffer_set_text(((MainInformation*)input)->messengers,text,-1);
    gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),&start,status->lexical->line);
    gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),&end,status->lexical->line+1);
    gtk_text_buffer_select_range(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),&start,&end);
     
  }
  else if(status->response == OperatingInvalid)
  {
    char text[50] = "Error na Linha ";
    char row[4];

    itoa(status->lexical->line,row,10);
    strcat(text,row);
    strcat(text," Operandos Inválidos");
    gtk_text_buffer_set_text(((MainInformation*)input)->messengers,text,-1);
    gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),&start,status->lexical->line);
    gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),&end,status->lexical->line+1);
    gtk_text_buffer_select_range(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),&start,&end);


  }
  else if(status->response == IncorrectInstruction)
  {
    char text[50] = "Error na Linha ";
    char row[4];

    itoa(status->lexical->line,row,10);
    strcat(text,row);
    strcat(text," Instrução não suportada");
    gtk_text_buffer_set_text(((MainInformation*)input)->messengers,text,-1);
    gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),&start,status->lexical->line);
    gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),&end,status->lexical->line+1);
    gtk_text_buffer_select_range(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),&start,&end);


  }
  else if(status->response == SymbolNotFoundInTable)
  {
    char text[50] = "Error na Linha ";
    char row[4];

    itoa(status->lexical->line,row,10);
    strcat(text,row);
   // tag = gtk_text_buffer_create_tag (GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer), "blue_foreground","foreground", "blue", NULL); 
    strcat(text," Simbolo não encontrado na tabela");
    gtk_text_buffer_set_text(((MainInformation*)input)->messengers,text,-1);
    gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),&start,status->lexical->line);
    gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),&end,status->lexical->line+1);
    gtk_text_buffer_select_range(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),&start,&end);
  //  gtk_text_buffer_apply_tag(GTK_TEXT_BUFFER(textEditorArea->textSourceBuffer),tag,&start,&end);

  }
   if(lexicalTokens->labels != NULL)
     free(lexicalTokens->labels);
   if(lexicalTokens->instructions != NULL)
     free(lexicalTokens->instructions);
}
static void runToolBarClicked(GtkToolButton *toolbutton, gpointer input)
{
    Processor* mk4Core = NULL;
    RunMode runMode = PlayMode;
    TextEditorArea *textEditorArea;
    unsigned currentPage;


    currentPage = gtk_notebook_get_current_page(GTK_NOTEBOOK((((MainInformation*)input)->_textEditor->tabPage)));

  
    textEditorArea = (((MainInformation*)input)->_textEditor->textEdit)->seach((((MainInformation*)input)->_textEditor->textEdit)->list,
                                                           &currentPage,compareTo);


   
    if(textEditorArea == NULL) return;
    

    mk4Core = newProcessor();

    unitControl(mk4Core, runMode);
  
}


 int main (int argc, char **argv)
 {
 
  FILE* file = NULL;
  FILE* _data = NULL;
  unsigned mode = 0;
  RunMode runMode;
  TextEditor *textEditor = newTextEditor();
  GtkWidget *spinner;
  GtkWidget *seachEntry;
  GtkWidget *statusBar;
  GtkWidget *gridToolBar;
 
  GtkWidget *window;
  GtkWidget *boxMain;
  GtkWidget *boxRegisters;

  GtkWidget *menubar;
  GtkWidget *fileMenu;

  GtkWidget *fileMi;
  GtkWidget *newFileMi;
  GtkWidget *openFileMi;
  GtkWidget *saveMi;
  GtkWidget *saveAsMi;
  GtkWidget *saveAllMi;
  GtkWidget *exportMi;
  GtkWidget *printMi;
  GtkWidget *quitMi;

  GtkWidget *editMenu;
  GtkWidget *editMi;
  GtkWidget *goToMi;
  GtkWidget *findReplaceMi;
  GtkWidget *undoMi;
  GtkWidget *redoMi;
  GtkWidget *undoLastActionMi;
  GtkWidget *redoLastActionMi;
  GtkWidget *cutMi;
  GtkWidget *copyMi;
  GtkWidget *pasteMi;
  GtkWidget *deleteMi;

  GtkWidget *executarMenu;
  GtkWidget *executarMenuMi;
  GtkWidget *assembleMi;
  GtkWidget *goMi;
  GtkWidget *stepMi;
  GtkWidget *backStepMi;
  GtkWidget *pauseMi;
  GtkWidget *stopMi;
  GtkWidget *resetMi;

  GtkWidget *toolBar;
  GtkWidget *imageNewFile;
  GtkWidget *imageOpenFile;
  GtkWidget *imageSave;
  GtkWidget *imageSaveAs;
  GtkWidget *imageSaveAll;
  GtkWidget *imagePrint;

  GtkWidget *imageUndo;
  GtkWidget *imageRedo;
  GtkWidget *imageCut;
  GtkWidget *imageFind;

  GtkWidget *imageAssemble;
  GtkWidget *imageStop;
  GtkWidget *imageRun;
  GtkWidget *imageRunStep;
  GtkWidget *imageHelp;


  GtkToolItem *newFileToolBar;
  GtkToolItem *openFileToolBar;
  GtkToolItem *saveToolBar;
  GtkToolItem *saveAsToolBar;
  GtkToolItem *saveAllToolBar;
  GtkToolItem *printToolBar;

  GtkToolItem *separator1;

  GtkToolItem *undoToolBar;
  GtkToolItem *redoToolBar;
  GtkToolItem *cutToolBar;
  GtkToolItem *findToolBar;

  GtkToolItem *separator2;

  GtkToolItem *assembleToolBar;
  GtkToolItem *stopToolBar;
  GtkToolItem *runToolBar;
  GtkToolItem *runStepToolBar;
  GtkToolItem *helpToolBar;

  GtkWidget *noteBook;
 
  GtkWidget *noteBookInter;
  GtkWidget *imageStartPage;
  GtkWidget *labelStart;
  GtkWidget *labelRegisterPurpose;
  GtkWidget *labelCoprocessor1;
  GtkWidget *labelCoprocessor0;
  GtkWidget *labelMensagens;
  GtkWidget *labelConsole;
  GtkWidget *tableGeneralRegisters;
  GtkWidget *tableCoprocessor1;
  GtkWidget *tableCoprocessor0;
  GtkWidget *mainPanned;
  GtkWidget *secundaryPanned;
  
  GtkWidget *viewInterfaceMensage;
  GtkWidget *viewInterfaceConsole;
  GtkWidget *scrollViewInterfaceMensage;
  GtkWidget *scrollViewInterfaceConsole;
  GtkTextBuffer *bufferTextViewMensage; 
  GtkTextBuffer *bufferTextViewConsole; 
  GtkWidget *label1NotebookMain;
  GtkWidget *label2NotebookMain;
  GtkWidget *seachBar;
  GtkWidget *noteBookMain;
  GtkWidget *gridExecut;
  MainInformation *mainInfomation;
  GtkWidget* panedMemory;
  GtkWidget* scrollTextMemory;


  GtkWidget *colorTable;
  GdkRGBA *rgbColorTable;
  unsigned i;


  gtk_init(&argc, &argv);
  

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);
  gtk_window_set_title(GTK_WINDOW(window), "MIPS Milk Way Earth Version");
  gtk_window_maximize(GTK_WINDOW(window)); 
  mainPanned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
  secundaryPanned = gtk_paned_new(GTK_ORIENTATION_VERTICAL);

  panedMemory = gtk_paned_new(GTK_ORIENTATION_VERTICAL);

  
  boxMain = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  mainInfomation = (MainInformation*)malloc(sizeof(MainInformation));
  
  gtk_container_add(GTK_CONTAINER(window),boxMain);
  gtk_box_set_baseline_position(GTK_BOX(boxMain),GTK_BASELINE_POSITION_CENTER);

  
  gtk_paned_set_wide_handle(GTK_PANED(mainPanned),TRUE); 
  gtk_paned_set_wide_handle(GTK_PANED(secundaryPanned),TRUE); 
  LabelsInit();
 
  menubar = gtk_menu_bar_new();
  fileMenu = gtk_menu_new();
  editMenu = gtk_menu_new();
  executarMenu = gtk_menu_new();

  noteBook = gtk_notebook_new();
  noteBookRegisters = gtk_notebook_new();
  noteBookInter = gtk_notebook_new();
  noteBookMain = gtk_notebook_new();

  mainInfomation->mainNote = noteBookMain;
  mainInfomation->noteIter = noteBookInter;

  gridExecut = gtk_grid_new();
  
  toolBar = gtk_toolbar_new();

  textEditor->tabPage = noteBook;
 
  gtk_toolbar_set_style(GTK_TOOLBAR(toolBar),GTK_TOOLBAR_ICONS);
  
  labelStart = gtk_label_new("Inicio");

  label1NotebookMain = gtk_label_new("Editar");
  label2NotebookMain = gtk_label_new("Executar");

  labelRegisterPurpose = gtk_label_new(NULL);
  labelCoprocessor1 = gtk_label_new(NULL);
  labelCoprocessor0 = gtk_label_new(NULL);



  gtk_label_set_markup(GTK_LABEL(labelRegisterPurpose),"<span style=\"oblique\" font =\"Tahoma\">Registradores</span>");
  gtk_label_set_markup(GTK_LABEL(labelCoprocessor1),   "<span style=\"oblique\" font =\"Tahoma\">Coprocessador 1</span>");
  gtk_label_set_markup(GTK_LABEL(labelCoprocessor0),   "<span style=\"oblique\" font =\"Tahoma\">Coprocessador 0</span>");

  labelMensagens = gtk_label_new("Mensagens");
  labelConsole = gtk_label_new("Entrada/Saída");

  tableGeneralRegisters = gtk_grid_new();
  tableCoprocessor0 = gtk_grid_new();
  tableCoprocessor1 = gtk_grid_new();

  gridToolBar = gtk_grid_new();
  gtk_grid_insert_row(GTK_GRID(gridToolBar),0);
  gtk_grid_insert_column(GTK_GRID(gridToolBar),0);
  gtk_grid_insert_column(GTK_GRID(gridToolBar),1);
  gtk_grid_set_column_spacing(GTK_GRID(gridToolBar),70);


  viewInterfaceMensage = gtk_text_view_new();
  viewInterfaceConsole = gtk_text_view_new();
  bufferTextViewMensage = gtk_text_view_get_buffer(GTK_TEXT_VIEW(viewInterfaceMensage));
  bufferTextViewConsole = gtk_text_view_get_buffer(GTK_TEXT_VIEW(viewInterfaceConsole));
  gtk_text_view_set_editable(GTK_TEXT_VIEW(viewInterfaceMensage),FALSE); 
  gtk_text_view_set_editable(GTK_TEXT_VIEW(viewInterfaceConsole),FALSE); 
  scrollViewInterfaceMensage = gtk_scrolled_window_new(NULL,NULL);
  scrollViewInterfaceConsole = gtk_scrolled_window_new(NULL,NULL);

  textEditor->sourceLanguages = gtk_source_language_manager_new();
  g_object_ref(textEditor->sourceLanguages);


  gtk_grid_set_row_spacing(GTK_GRID(tableGeneralRegisters),1);
  gtk_grid_set_column_spacing(GTK_GRID(tableGeneralRegisters),80);

  gtk_grid_set_row_spacing(GTK_GRID(tableCoprocessor1),1);
  gtk_grid_set_column_spacing(GTK_GRID(tableCoprocessor1),80);

  gtk_grid_set_row_spacing(GTK_GRID(tableCoprocessor0),1);
  gtk_grid_set_column_spacing(GTK_GRID(tableCoprocessor0),80);


  for(i = 0; i < 5; i++) 
     gtk_grid_insert_row(GTK_GRID(tableCoprocessor0),i);

     gtk_grid_insert_column(GTK_GRID(tableCoprocessor0),0);
     gtk_grid_insert_column(GTK_GRID(tableCoprocessor0),1);
     gtk_grid_insert_column(GTK_GRID(tableCoprocessor0),2);
 
  for(i = 0; i < 36; i++) 
     gtk_grid_insert_row(GTK_GRID(tableGeneralRegisters),i);

     gtk_grid_insert_column(GTK_GRID(tableGeneralRegisters),0);
     gtk_grid_insert_column(GTK_GRID(tableGeneralRegisters),1);
     gtk_grid_insert_column(GTK_GRID(tableGeneralRegisters),2);

  for(i = 0; i < 33; i++) 
     gtk_grid_insert_row(GTK_GRID(tableCoprocessor1),i);

     gtk_grid_insert_column(GTK_GRID(tableCoprocessor1),0);
     gtk_grid_insert_column(GTK_GRID(tableCoprocessor1),1);
     gtk_grid_insert_column(GTK_GRID(tableCoprocessor1),2);


  for(i = 0; i < 36; i++) 
     gtk_grid_attach(GTK_GRID(tableGeneralRegisters),regName[i],0,i,1,1);

  for(i = 0; i < 33; i++) 
     gtk_grid_attach(GTK_GRID(tableGeneralRegisters),regNumber[i],1,i,1,1);

  for(i = 0; i < 36; i++) 
     gtk_grid_attach(GTK_GRID(tableGeneralRegisters),regValue[i],2,i,1,1);

  for(i = 0; i < 33; i++) 
     gtk_grid_attach(GTK_GRID(tableCoprocessor1),fregName[i],0,i,1,1);

  for(i = 0; i < 33; i++) 
     gtk_grid_attach(GTK_GRID(tableCoprocessor1),fregValueFloat[i],1,i,1,1);

  for(i = 0; i < 33; i++) 
     gtk_grid_attach(GTK_GRID(tableCoprocessor1),fregValueDouble[i],2,i,1,1);


  for(i = 0; i < 5; i++) 
     gtk_grid_attach(GTK_GRID(tableCoprocessor0),nameC0[i],0,i,1,1);

  for(i = 0; i < 5; i++) 
     gtk_grid_attach(GTK_GRID(tableCoprocessor0),numberC0[i],1,i,1,1);

  for(i = 0; i < 5; i++) 
     gtk_grid_attach(GTK_GRID(tableCoprocessor0),valueC0[i],2,i,1,1);

  imageNewFile = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/newfile.png");
  imageOpenFile = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/openfile.png");
  imageSave = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/save.png");
  imageSaveAs = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/saveas.png");
  imageSaveAll = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/saveall.png");
  imagePrint = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/print.png");

  imageUndo = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/undo.png");
  imageRedo = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/redo.png");
  imageCut = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/cut.png");
  imageFind = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/find.png");

  imageAssemble = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/assemble.png");
  imageStop = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/stop2.png");
  imageRun = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/run2.png");
  imageRunStep = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/runapplet.png");
  imageHelp = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/help.png");

  imageStartPage = gtk_image_new_from_file("C:\\Users/deived/workspace/MipsSimulator/Resources/MIPS-destaque.png"); 

  gtk_notebook_insert_page(GTK_NOTEBOOK(noteBook),imageStartPage,labelStart,0);
  gtk_notebook_insert_page(GTK_NOTEBOOK(noteBookRegisters),tableGeneralRegisters,labelRegisterPurpose,0);
  gtk_notebook_insert_page(GTK_NOTEBOOK(noteBookRegisters),tableCoprocessor1,labelCoprocessor1,1);
  gtk_notebook_insert_page(GTK_NOTEBOOK(noteBookRegisters),tableCoprocessor0,labelCoprocessor0,2);

  gtk_notebook_insert_page(GTK_NOTEBOOK(noteBookInter),scrollViewInterfaceMensage,labelMensagens,0);
  gtk_container_add(GTK_CONTAINER(scrollViewInterfaceMensage),viewInterfaceMensage);

  gtk_notebook_insert_page(GTK_NOTEBOOK(noteBookInter),scrollViewInterfaceConsole,labelConsole,1);
  gtk_container_add(GTK_CONTAINER(scrollViewInterfaceConsole),viewInterfaceConsole);

  
  gtk_notebook_insert_page(GTK_NOTEBOOK(noteBookMain),noteBook,label1NotebookMain,0);
  gtk_notebook_insert_page(GTK_NOTEBOOK(noteBookMain),panedMemory,label2NotebookMain,1);
  scrollTextMemory = gtk_scrolled_window_new(NULL,NULL);
  gtk_paned_add1(GTK_PANED(panedMemory),scrollTextMemory);


  gtk_container_add(GTK_CONTAINER(scrollTextMemory),gridExecut);
  gridMemoryText = gridExecut;

  gtk_widget_set_opacity(tableGeneralRegisters,1);
  gtk_notebook_set_show_tabs(GTK_NOTEBOOK(noteBook),TRUE);
  gtk_notebook_set_show_tabs(GTK_NOTEBOOK(noteBookRegisters),TRUE);
  gtk_notebook_set_show_tabs(GTK_NOTEBOOK(noteBookInter),TRUE);

  spinner = gtk_spinner_new();
  
  statusBar = gtk_statusbar_new();


  newFileToolBar = gtk_tool_button_new(imageNewFile,"Novo");
  openFileToolBar = gtk_tool_button_new(imageOpenFile," ");
  saveToolBar = gtk_tool_button_new(imageSave," ");
  saveAsToolBar = gtk_tool_button_new(imageSaveAs," ");
  saveAllToolBar = gtk_tool_button_new(imageSaveAll," ");
  printToolBar = gtk_tool_button_new(imagePrint," ");

  undoToolBar = gtk_tool_button_new(imageUndo," ");
  redoToolBar = gtk_tool_button_new(imageRedo," ");
  cutToolBar = gtk_tool_button_new(imageCut," ");
  findToolBar = gtk_tool_button_new(imageFind," ");

  assembleToolBar = gtk_tool_button_new(imageAssemble," ");
  stopToolBar = gtk_tool_button_new(imageStop," ");
  runToolBar = gtk_tool_button_new(imageRun," ");
  runStepToolBar = gtk_tool_button_new(imageRunStep," ");
  helpToolBar = gtk_tool_button_new(imageHelp," ");

  separator1 = gtk_separator_tool_item_new();
  separator2 = gtk_separator_tool_item_new();

  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), newFileToolBar, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), openFileToolBar, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), saveToolBar, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), saveAsToolBar, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), saveAllToolBar, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), printToolBar, -1);

  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), separator1, -1);

  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), undoToolBar, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), redoToolBar, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), cutToolBar, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), findToolBar, -1);

  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), separator2, -1);
  
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), assembleToolBar, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), stopToolBar, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), runToolBar, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), runStepToolBar, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), helpToolBar, -1);


  fileMi = gtk_menu_item_new_with_label("Arquivo");

  newFileMi = gtk_menu_item_new_with_label("Novo");
  openFileMi = gtk_menu_item_new_with_label("Abrir...");
  saveMi = gtk_menu_item_new_with_label("Salvar");
  saveAsMi = gtk_menu_item_new_with_label("Salvar Como...");
  saveAllMi= gtk_menu_item_new_with_label("Salvar Tudo");
  exportMi = gtk_menu_item_new_with_label("Exportar");
  printMi = gtk_menu_item_new_with_label("Imprimir");
  quitMi = gtk_menu_item_new_with_label("Sair");

  editMi = gtk_menu_item_new_with_label("Editar");

  goToMi = gtk_menu_item_new_with_label("Ir Para");
  findReplaceMi = gtk_menu_item_new_with_label("Encontrar e Substituir");
  undoMi = gtk_menu_item_new_with_label("Refazer");
  redoMi = gtk_menu_item_new_with_label("Desfazer");
  undoLastActionMi = gtk_menu_item_new_with_label("Desfazer Última Ação");
  redoLastActionMi = gtk_menu_item_new_with_label("Refazer Última Ação");
  cutMi = gtk_menu_item_new_with_label("Recortar");
  copyMi = gtk_menu_item_new_with_label("Copiar");
  pasteMi = gtk_menu_item_new_with_label("Colar");
  deleteMi = gtk_menu_item_new_with_label("Excluir");

  executarMenuMi = gtk_menu_item_new_with_label("Executar");

  assembleMi = gtk_menu_item_new_with_label("Montar");
  goMi = gtk_menu_item_new_with_label("Ir");
  stepMi = gtk_menu_item_new_with_label("Passo");
  backStepMi = gtk_menu_item_new_with_label("Passo para Trás");
  pauseMi = gtk_menu_item_new_with_label("Pausar");
  stopMi = gtk_menu_item_new_with_label("Parar");
  resetMi = gtk_menu_item_new_with_label("Reiniciar");


  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi),fileMenu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(editMi),editMenu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(executarMenuMi),executarMenu);


  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),newFileMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),openFileMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),saveMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),saveAsMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),saveAllMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),exportMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),printMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),quitMi);

  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),goToMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),findReplaceMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),undoMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),redoMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),undoLastActionMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),redoLastActionMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),cutMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),copyMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),pasteMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),deleteMi);

  gtk_menu_shell_append(GTK_MENU_SHELL(executarMenu),assembleMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(executarMenu),goMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(executarMenu),stepMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(executarMenu),backStepMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(executarMenu),pauseMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(executarMenu),stopMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(executarMenu),resetMi);

  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), editMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), executarMenuMi);

  gtk_grid_attach(GTK_GRID(gridToolBar),toolBar,0,0,1,1);

  gtk_box_pack_start(GTK_BOX(boxMain), menubar, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(boxMain), gridToolBar, FALSE, FALSE, 0);

   
  gtk_container_add(GTK_CONTAINER(boxMain),mainPanned);
  gtk_paned_add1(GTK_PANED(mainPanned),secundaryPanned);

  gtk_paned_add1(GTK_PANED(secundaryPanned),noteBookMain);
  gtk_paned_add2(GTK_PANED(secundaryPanned),noteBookInter);
  gtk_paned_add2(GTK_PANED(mainPanned),noteBookRegisters);

    
  gtk_paned_set_position(GTK_PANED(mainPanned),990);

  mainInfomation->_textEditor = textEditor;
  mainInfomation->messengers = bufferTextViewMensage;
   
  g_signal_connect(G_OBJECT(window), "destroy",
  G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(quitMi), "activate",
  G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(newFileMi), "activate",
  G_CALLBACK(newFileSelect),textEditor);

  g_signal_connect(G_OBJECT(openFileMi), "activate",
  G_CALLBACK(openFileSelect),textEditor);

  g_signal_connect(G_OBJECT(saveMi), "activate",
  G_CALLBACK(saveFileSelect),textEditor);

  g_signal_connect(G_OBJECT(saveAsMi), "activate",
  G_CALLBACK(saveAsFileSelect),textEditor);

  g_signal_connect(G_OBJECT(saveAllMi), "activate",
  G_CALLBACK(saveAllFileSelect),textEditor);



  g_signal_connect(G_OBJECT(undoToolBar), "clicked",
  G_CALLBACK(undoToolBarClicked),textEditor);

  g_signal_connect(G_OBJECT(redoToolBar), "clicked",
  G_CALLBACK(redoToolBarClicked),textEditor);

  g_signal_connect(G_OBJECT(newFileToolBar), "clicked",
  G_CALLBACK(newFileToolBarClicked),textEditor);

  g_signal_connect(G_OBJECT(openFileToolBar), "clicked",
  G_CALLBACK(openFileToolBarClicked),textEditor);

  g_signal_connect(G_OBJECT(saveToolBar), "clicked",
  G_CALLBACK(saveToolBarClicked),textEditor);

  g_signal_connect(G_OBJECT(saveAsToolBar), "clicked",
  G_CALLBACK(saveAsToolBarClicked),textEditor);

  g_signal_connect(G_OBJECT(saveAllToolBar), "clicked",
  G_CALLBACK(saveAllToolBarClicked),textEditor);

  g_signal_connect(G_OBJECT(assembleToolBar), "clicked",
  G_CALLBACK(assembleToolBarClicked),mainInfomation);

  g_signal_connect(G_OBJECT(runToolBar), "clicked",
  G_CALLBACK(runToolBarClicked),mainInfomation);

  g_signal_connect(G_OBJECT(noteBookMain), "switch-page",
  G_CALLBACK(switchPage),noteBookMain);


  
  gtk_widget_show_all(window);

  gtk_main();


 return 0;
 }
