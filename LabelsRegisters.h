/*
 * Processador.h
 *
 *  Created on: 10/05/2017
 *      Author: Deived William Jal Windows
 */

#ifndef LABELSREGISTERS_H_
#define LABELSREGISTERS_H_

#include <gtk/gtk.h>


extern GtkWidget *regName[36];
extern GtkWidget *regNumber[33];
extern GtkWidget *regValue[36];


extern GtkWidget *fregName[33];
extern GtkWidget *fregValueFloat[33];
extern GtkWidget *fregValueDouble[33];

extern GtkWidget *nameC0[5];
extern GtkWidget *numberC0[5];
extern GtkWidget *valueC0[5];
extern GtkWidget *noteBookRegisters;
extern GtkWidget *gridMemoryText;


void LabelsInit(void);

#endif