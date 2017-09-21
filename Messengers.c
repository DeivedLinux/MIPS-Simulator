#include "Messengers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void sendMessengers(MessengersOutPut* output, const char* messengers)
{
   int lineError;
   char msgLineError[50] = " Error na linha ";
   char lineText[3];

   lineError = gtk_text_iter_get_line(GetStart(output));
   itoa(lineError+1,lineText,10);
   strcat(msgLineError,lineText);
   strcat(msgLineError,messengers);
   gtk_text_buffer_set_text(GetTBuffer(output),msgLineError,-1);
   gtk_text_buffer_select_range(GTK_TEXT_BUFFER(GetSBuffer(output)),GetStart(output),GetEnd(output));
                     
}
MessengersOutPut *newMessengers()
{
	MessengersOutPut *msg;

	msg = (MessengersOutPut*)malloc(sizeof(MessengersOutPut));

    return msg;
}