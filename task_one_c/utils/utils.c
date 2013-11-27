#include <stdlib.h>
#include <stdio.h>

#include "../utils/utils.h"

void abort_prg(const char* error_message) {
	fprintf(stderr, "%s\n", error_message);
	exit(1);
}
