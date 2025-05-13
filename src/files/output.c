#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "output.h"

// Функция для вписывания данных в файл
int write_to_file(const char* filename, const char* format, ...) {
    int result = 0; 			// Результат, дающий знать о успешной записи
    FILE *file = fopen(filename, "a");  // Открываем файл по названию (a - Дописывает информацию к концу файла)

    if (file) {				// Если файл найден, начинаем запись
        va_list args;
        va_start(args, format);
        vfprintf(file, format, args);
        va_end(args);
    
        fclose(file);
	result = 1;			// Ставим результат как 1 (true)
    }

    return result;			// Возращаем полученный результат
}
