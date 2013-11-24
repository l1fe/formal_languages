#include <stdbool.h>
#include <string.h>

#include "static_hash_table.h"
#include "utils.h"

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

unsigned int calculate_hash(char* key, size_t key_size, int h_table_capacity) {
	if (key == NULL || key_size <= 0 || h_table_capacity <= 0) {
		return -1;
	}

	size_t i;
	unsigned int hash = 0;
	for (i = 0; i < key_size; ++i) {
		hash += key[i] * 17 % h_table_capacity;
	}

	return hash % h_table_capacity;
}

static_hash_table* static_hash_table_put(static_hash_table* h_table, char* key, size_t key_size, void* value, size_t value_size) {
	if (h_table == NULL || key == NULL || value == NULL) {
		return NULL;
	}

	int hash = calculate_hash(key, key_size, h_table->capacity);
	if (hash == -1) {
		return NULL;
	}

	bucket* new_bucket = (bucket*)malloc(sizeof(bucket));
	if (new_bucket == NULL) {
		abort_prg("static_hash_table_put:: malloc error");
	}

	new_bucket->key = key;
	new_bucket->key_size = key_size;

	new_bucket->value = value;
	new_bucket->value_size = value_size;

	new_bucket->next_bucket = NULL;
	
	bucket* current_bucket = h_table->store[hash];
	if (current_bucket == NULL) {
		h_table->store[hash] = new_bucket;
		new_bucket->prev_bucket = NULL;
	} else {
		if (strcmp(current_bucket->key, key) == 0) {
			if (current_bucket->key != key) {
				free(key);
			}
			if (current_bucket->value != value) {
				free(current_bucket->value);
			}
			current_bucket->value = value;
			free(new_bucket);
			return h_table;
		}

		while (current_bucket->next_bucket != NULL) {
			if (strcmp(current_bucket->key, key) == 0) {
				if (current_bucket->key != key) {
					free(key);
				}
				if (current_bucket->value != value) {
					free(current_bucket->value);
				}
				current_bucket->value = value;
				free(new_bucket);
				return h_table;
			}
			current_bucket = current_bucket->next_bucket;
		}
		current_bucket->next_bucket = new_bucket;
		new_bucket->prev_bucket = current_bucket;
	}

	return h_table;
}

void* static_hash_table_get(static_hash_table* h_table, char* key) {
	if (key == NULL) {
		return NULL;
	}

	unsigned int i;
	for (i = 0; i < h_table->capacity; ++i) {
		bucket* current_bucket = h_table->store[i];
		while (current_bucket != NULL) {
			if (strcmp(current_bucket->key, key) == 0) {
				return current_bucket->value;
			}
			current_bucket = current_bucket->next_bucket;
		}
	}

	return NULL;
}

bool static_hash_table_remove(static_hash_table* h_table, char* key) {
	if (key == NULL) {
		return false;
	}

	unsigned int i;
	for (i = 0; i < h_table->capacity; ++i) {
		bucket* current_bucket = h_table->store[i];
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
					} else {
						h_table->store[i] = NULL;
					}
				}
				
				free(current_bucket->value);
				if (key != current_bucket->key) {
					free(current_bucket->key);
				}
	
				free(current_bucket);
				return true;
			}

			current_bucket = current_bucket->next_bucket; 
		}
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
			printf("(%s,%d)", current_bucket->key, *(int*)current_bucket->value);
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
			free(bucket_to_delete->value);
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
