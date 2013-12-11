#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "static_hash_table.h"

int main() {
	printf("%d\n", (int)'1');
	static_hash_table* table = static_hash_table_create();
	if (table == NULL) {
		fprintf(stderr, "error: can't create static_hash_table\n");
		exit(1);
	}

	char* key = (char*)malloc((strlen("key") + 3) * sizeof(char));
	if (key == NULL) {
		fprintf(stderr, "error: can't allocate memory for key\n");
		exit(1);
	}
	
	key[0] = 'k';
	key[1] = 'e';
	key[2] = 'y';
	key[5] = '\0';

	unsigned int i;
	for (i = 0; i < 10; ++i) {
		key[3] = i + 48;
		unsigned int j;
		for (j = 0; j < 10; ++j) {
			key[4] = j + 48;
			printf("put: (%s, %d)\n", key, i * 10 + j);
			table = static_hash_table_put(table, key, i * 10 + j);
			if (table == NULL) {
				fprintf(stderr, "error: table put error\n");
				exit(1);
			}
		}
	}

	for (i = 0; i < 10; ++i) {
		key[3] = i + 48;
		unsigned int j;
		for (j = 0; j < 10; ++j) {
			key[4] = j + 48;
			int expected_value = i * 10 + j;
			int success;
			int value = static_hash_table_get(table, key, &success);
		
			if (success == 0 && value == expected_value) {
				printf("OK! value = %d\n", value);
			} else {
				fprintf(stderr, "error: cant' get expected value\n");
				exit(1);
			}
		}
	}
	
	static_hash_table_print(table);
	
	for (i = 0; i < 10; ++i) {
		key[3] = i + 48;
		unsigned int j;
		for (j = 0; j < 10; ++j) {
			key[4] = j + 48;
			bool remove_result = static_hash_table_remove(table, key);
			if ( !remove_result ) {
				fprintf(stderr, "error: can't remove value\n");
				exit(1);
			} else {
				printf("Remove OK! (removed key = %s)\n", key);
			}
		}

	}

	free(key);

	static_hash_table_drop_table(table);
	return 0;
}
