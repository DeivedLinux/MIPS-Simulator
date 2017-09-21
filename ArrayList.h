/*
 * ArrayList.h
 *
 *  Created on: 23/06/2017
 *      Author: Deived William Jal Windows
 */

#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

#include <stdbool.h>


typedef struct
{
	void* information;
   	void* next;
   	void* previous;
}__attribute__((aligned(16)))List;

typedef struct
{
	List *list;
	bool(*isEmpty)(List* );
	bool(*remove)(void*,List*, bool(*isEqual)(void* , void*));
	void(*addArray)(void*, List** );
	void(*addOrdered)(void*, List*);
	void*(*seach)(List*, void*, bool(*isEqual)(void* elem, void* info));
	void(*destroyList)(List**);
	void**(*toArray)(List*);
	unsigned(*size)(List*);
}__attribute__((packed))ArrayList;

ArrayList* newArrayList();

#endif /* ARRAYLIST_H_ */
