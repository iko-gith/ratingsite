#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "text.h"

// Функция для замены определенной части строки другой строкой
char *str_replace(char *origin, 
		  const char *replacement, 
		  const char *with) {
    char *result;	// Результат работы функции в виде готовой строки
    char *ins;    	// Точка вставки replacement
    char *tmp;    	// Меняющийся указатель     (Для посчёта количество замен, после для работы с результатом)
    int len_rep;  	// Длина строки replacement (Заменить)
    int len_with;	// Длина строки with	    (На)
    int len_front; 	// Расстояние между точкой вставки ins на данный момент 
			// и следующей точки вставки replacement
    int count;   	// Количество замен
    
    // При отсутствии наличий важных аргументов, вернуть NULL
    if (!origin || !replacement)
        return NULL;
    
    // Если длина замены равна 0 (Отсутствует), вернуть NULL
    len_rep = strlen(replacement);
    if (!len_rep)
        return NULL;
    
    // Если отсутствует аргумент На, заменить его пустой строкой, чтобы не было ошибки
    if (!with)
        with = "";
    len_with = strlen(with);
    
    // Проходим через цикл, подсчитываем количество замен
    ins = origin;
    for (count = 0; (tmp = strstr(ins, replacement)); ++count)
        ins = tmp + len_rep;
    
    // Выделяем Место для результата
    result = malloc(strlen(origin) + (len_with - len_rep) * count + 1);
    if (!result)
        return NULL;

    tmp = result;

    // Используя полученное количество замен, проходим через цикл замены по аргументу replacement
    // с аргументом with
    while (count--) {
        ins = strstr(origin, replacement);		   // Определяем точку замены
        len_front = ins - origin;			   // Определяем расстояние (Между заданной точкой замены и конца предыдущей точки замены)
        tmp = strncpy(tmp, origin, len_front) + len_front; // tmp = Точка вставки, до момента замены
        tmp = strcpy(tmp, with) + len_with;		   // tmp = Точка вставки, после замены
        origin += len_front + len_rep; 			   // Следующая точка проверки
    }

    strcpy(tmp, origin);				   // Оставшуюся часть строки origin доставляем в конец tmp
    return result;
}
