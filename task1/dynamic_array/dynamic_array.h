#ifndef DYNAMIC_ARRAY_H_INCLUDED_
#define DYNAMIC_ARRAY_H_INCLUDED_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct dynamic_array_ {
	int* store;
	int size;
	int capacity;
} dynamic_array;

dynamic_array* dynamic_array_create(int);
bool dynamic_array_realloc(dynamic_array*, int);
bool dynamic_array_push_back(dynamic_array*, int);
void dynamic_array_print(dynamic_array*);
void dynamic_array_clear(dynamic_array*);
int dynamic_array_get(dynamic_array*, int, int*);
bool dynamic_array_set(dynamic_array*, int, int);
void dynamic_array_destroy(dynamic_array*);

#endif
