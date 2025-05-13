#include <stdio.h>
#include <stdlib.h>
#include "input.h"

// Функция для чтения файла
char *read_file(const char *filename) {
    FILE *f = fopen(filename, "rb");	// Открываем файл для чтения (rb - Двоичный вид)
    char *res = NULL;			// Результат

    if (f) {				// Файл найден, начинаем считывание данных
        fseek(f, 0, SEEK_END);		// Ставим указатель на начало
        size_t size = ftell(f);		// Читаем размер файла
        rewind(f);			// Указатель на начало потока
        res = (char *)malloc(size + 1); // Выделение памяти для результата строки
        if (res) {			// Если выделение успешно, начинаем считывать текст файл
            fread(res, 1, size, f);
            res[size] = '\0';
        }   
        fclose(f);			// Закрываем файл
    }

    return res;				// Возращаем результат
}

