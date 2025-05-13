#ifndef OUTPUT_H
#define OUTPUT_H

/**
* @brief Функция позволяет записать какой либо текст в файл по названию
* @param filename Название файла, в который необходимо записать текст
* @param format Текст, который необходимо записать в файл
* @param ... Дополнительные аргументы
* @return Возращает булеан, который дает знать о успешной записи
*/

int write_to_file(const char *filename, const char *format, ...);

#endif  // OUTPUT_H
