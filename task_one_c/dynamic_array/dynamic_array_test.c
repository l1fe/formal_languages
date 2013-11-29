#include <stdlib.h>
#include <stdio.h>

#include "dynamic_array.h"

int main() {
	dynamic_array* arr = dynamic_array_create(5);
	
	int i;
	for (i = 0; i < 50; ++i) {
		dynamic_array_push_back(arr, i);
	}
	
	for (i = 0; i < 50; ++i) {
		int success;
		int expected_value = i;
		int value = dynamic_array_get(arr, i, &success);

		if (success < 0 || expected_value != value) {
			fprintf(stderr, "can't get %d value\n", i);
			exit(1);
		} else {
			printf("OK! Value = %d\n", expected_value);
		}
	}

	for (i = 0; i < 50; ++i) {
		dynamic_array_set(arr, i, i * 10);
	}

	for (i = 0; i < 50; ++i) {
		int success;
		int expected_value = i * 10;
		int value = dynamic_array_get(arr, i, &success);
		
		if (success < 0 || expected_value != value) {
			fprintf(stderr, "cant' get %d value\n", i);
			exit(1);
		} else {
			printf("OK! Value = %d\n", expected_value);
		}
	}

	dynamic_array_print(arr);

	dynamic_array_destroy(arr);
	return 0;
}
