#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "dynamic_array.h"
#include "../utils/utils.h"

dynamic_array* dynamic_array_create(int capacity) {
	if (capacity <= 0) {
		return NULL;
	}
	
	dynamic_array* arr = (dynamic_array*)malloc(sizeof(dynamic_array));
	if (arr == NULL) {
		abort_prg("dynamic_array_create:: malloc error");
	}

	arr->size = 0;
	arr->capacity = capacity;
	arr->store = (int*)malloc(capacity * sizeof(int));

	if (arr->store == NULL) {
		abort_prg("dynamic_array_create:: malloc error");
	}

	return arr;
}

bool dynamic_array_realloc(dynamic_array* arr, int new_capacity) {
	if (arr == NULL || arr->store == NULL || new_capacity <= 0 || new_capacity <= arr->capacity) {
		return false;
	}

	arr->store = (int*)realloc(arr->store, new_capacity * sizeof(int));
	if (arr->store == NULL) {
		abort_prg("dynamic_array_realloc:: realloc error");
	}

	arr->capacity = new_capacity;

	return true;
}

bool dynamic_array_push_back(dynamic_array* arr, int value) {
	if (arr == NULL || arr->store == NULL) {
		return false;
	}

	if (arr->size >= arr->capacity / 2) {
		bool realloc_result = dynamic_array_realloc(arr, arr->capacity * 2 + 1);
		if ( !realloc_result ) {
			return false;
		}
	}

	arr->store[arr->size] = value;
	arr->size += 1;
	
	return true;
}

int dynamic_array_get(dynamic_array* arr, int index, int* success_indicator) {
	if (arr == NULL || arr->store == NULL || index < 0 || index >= arr->size) {
		*success_indicator = -1;
		return -1;
	}

	return arr->store[index];
}

bool dynamic_array_set(dynamic_array* arr, int index, int value) {
	if (arr == NULL || arr->store == NULL || index < 0 || index >= arr->size) {
		return false;
	}

	arr->store[index] = value;

	return true;
}

void dynamic_array_print(dynamic_array* arr) {
	if (arr == NULL || arr->store == NULL) {
		return;
	}

	printf("{");
	int i;
	for (i = 0; i < arr->size; ++i) {
		if (i < arr->size - 1) {
			printf("%d, ", arr->store[i]);
		} else {
			printf("%d", arr->store[i]);
		}
	}

	printf("}\n");
}

void dynamic_array_clear(dynamic_array* arr) {
	if (arr == NULL || arr->store == NULL) {
		return;
	}

	int i;
	for (i = 0; i < arr->capacity; ++i) {
		arr->store[i] = 0;
	}

	arr->size = 0;
}

void dynamic_array_destroy(dynamic_array* arr) {
	if (arr == NULL || arr->store == NULL) {
		return;
	}

	free(arr->store);
	free(arr);
}
