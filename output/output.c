#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "output.h"

int write_to_file(const char* filename, const char* format, ...) {
    int result = 0; 
    FILE *file = fopen(filename, "a");

    if (file) {
        va_list args;
        va_start(args, format); // Initialization of va_list
        vfprintf(file, format, args); // Print function
        va_end(args);
    
        fclose(file);
	result = 1;
    }

    return result;
}
