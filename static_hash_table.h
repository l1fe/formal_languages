#ifndef STATIC_HASH_TABLE_H_INCLUDED_
#define STATIC_HASH_TABLE_H_INCLUDED_

#define STATIC_HASH_TABLE_SIZE 10

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct bucket_ {
	char* key;
	void* value;
	size_t key_size;
	size_t value_size;
	struct bucket_* next_bucket;
	struct bucket_* prev_bucket;
} bucket;

typedef struct static_hash_table_ {
	bucket* store[STATIC_HASH_TABLE_SIZE];
	unsigned int size;
	unsigned int capacity;	
} static_hash_table;

static_hash_table* static_hash_table_create();
static_hash_table* static_hash_table_put(static_hash_table*, char*, size_t, void*, size_t);
void static_hash_table_clear(static_hash_table*);
void static_hash_table_drop_table(static_hash_table*);
void static_hash_table_print(static_hash_table*);
void* static_hash_table_get(static_hash_table*, char*);
bool static_hash_table_remove(static_hash_table*, char*);

int calculate_hash(char*, size_t, int);

#endif
