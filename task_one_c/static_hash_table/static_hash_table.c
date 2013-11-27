#include <stdbool.h>
#include <string.h>

#include "static_hash_table.h"
#include "../utils/utils.h"

static_hash_table* static_hash_table_create() {
	static_hash_table* h_table = (static_hash_table*)malloc(sizeof(static_hash_table));
	if (h_table == NULL) {
		abort_prg("static_hash_table_create:: malloc error");
	}
	
	int i;
	for (i = 0; i < STATIC_HASH_TABLE_SIZE; ++i) {
		h_table->store[i] = NULL;
	}

	h_table->size = 0;
	h_table->capacity = STATIC_HASH_TABLE_SIZE;

	return h_table;
}

int calculate_hash(char* key, int h_table_capacity) {
	if (key == NULL || h_table_capacity <= 0) {
		return -1;
	}

	size_t i;
	unsigned int hash = -1;
	
	for (i = 0; i < strlen(key); ++i) {
		hash += key[i] * 19 % h_table_capacity;
	}

	return hash % h_table_capacity;
}

static_hash_table* static_hash_table_put(static_hash_table* h_table, char* key, int value) {
	if (h_table == NULL || h_table->store == NULL || key == NULL) {
		return NULL;
	}

	char* key_to_put = strdup(key);

	int hash = calculate_hash(key_to_put, h_table->capacity);
	if (hash == -1) {
		return NULL;
	}

	bucket* current_bucket = h_table->store[hash];
	bucket* prev_bucket;

	if (current_bucket == NULL) {
		h_table->store[hash] = (bucket*)malloc(sizeof(bucket));
		if (h_table->store[hash] == NULL) {
			abort_prg("static_hash_table_put:: malloc error");
		}
	
		h_table->store[hash]->key = key_to_put;
		h_table->store[hash]->value = value;
		h_table->store[hash]->next_bucket = NULL;
		h_table->store[hash]->prev_bucket = NULL;
		return h_table;
	} 

	while (current_bucket != NULL) {
		if ( strcmp(current_bucket->key, key_to_put) == 0 ) {
			current_bucket->value = value;
			return h_table;
		}

		prev_bucket = current_bucket;
		current_bucket = current_bucket->next_bucket;
	}

	bucket* new_bucket = (bucket*)malloc(sizeof(bucket));
	if (new_bucket == NULL) {
		abort_prg("static_hash_table_put:: malloc error");
	}

	new_bucket->key = key_to_put;
	new_bucket->value = value;
	new_bucket->next_bucket = NULL;
	new_bucket->prev_bucket = prev_bucket;

	prev_bucket->next_bucket = new_bucket;
	
	return h_table;
}

int static_hash_table_get(static_hash_table* h_table, char* key, int* success_indicator) {
	if (key == NULL) {
		*success_indicator = -1;
		return -1;
	}

	int hash = calculate_hash(key, h_table->capacity);
	if (hash == -1) {
		*success_indicator = -1;
		return -1;
	}
	
	bucket* current_bucket = h_table->store[hash];
	while (current_bucket != NULL) {
		if (strcmp(current_bucket->key, key) == 0) {
			*success_indicator = 0;
			return current_bucket->value;
		}
	
		current_bucket = current_bucket->next_bucket;
	}

	*success_indicator = -1;
	return -1;
}

bool static_hash_table_remove(static_hash_table* h_table, char* key) {
	if (h_table == NULL || h_table->store == NULL || key == NULL) {
		return false;
	}

	int hash = calculate_hash(key, h_table->capacity);
	if (hash < 0) {
		return false;
	}

	bucket* current_bucket = h_table->store[hash];
	while (current_bucket != NULL) {
		if (strcmp(current_bucket->key, key) == 0) {
			if (current_bucket->prev_bucket != NULL) {
				current_bucket->prev_bucket->next_bucket = current_bucket->next_bucket;
				if (current_bucket->next_bucket != NULL) {
					current_bucket->next_bucket->prev_bucket = current_bucket->prev_bucket;
				}
			} else {
				if (current_bucket->next_bucket != NULL) {
					current_bucket->next_bucket->prev_bucket = NULL;
					h_table->store[hash] = current_bucket->next_bucket;
				} else {
					h_table->store[hash] = NULL;
				}
			}
				
			if (key != current_bucket->key) {
				free(current_bucket->key);
			}

			free(current_bucket);
			return true;
		}

		current_bucket = current_bucket->next_bucket; 
	}

	return false;
}

void static_hash_table_print(static_hash_table* h_table) {
	if (h_table == NULL) {
		return;
	}

	unsigned int i;
	for (i = 0; i < h_table->capacity; ++i) {
		printf("%d: ", i);
		bucket* current_bucket = h_table->store[i];
		if (current_bucket == NULL) {
			printf("empty\n");
			continue;
		}
		do {
			printf("(%s,%d)", current_bucket->key, current_bucket->value);
		} while ((current_bucket = current_bucket->next_bucket) != NULL);

		printf("\n");
	} 
}

void static_hash_table_clear(static_hash_table* h_table) {
	if (h_table == NULL) {
		return;
	}
	
	unsigned int i;
	for (i = 0; i < h_table->capacity; ++i) {
		bucket* current_bucket = h_table->store[i];
		while (current_bucket != NULL) {
			bucket* bucket_to_delete = current_bucket;
			current_bucket = current_bucket->next_bucket;
			free(bucket_to_delete->key);
			free(bucket_to_delete);
		}
		
		h_table->store[i] = NULL;
	}
}

void static_hash_table_drop_table(static_hash_table* h_table) {
	if (h_table == NULL) {
		return;
	}

	static_hash_table_clear(h_table);
	free(h_table);
}
