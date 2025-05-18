#ifndef STRING_H
#define STRING_H

/**
* @brief Возврат строки, с замененными её частями
* @param origin Строка, которую необхотимо изменить
* @param replacement Часть строки, которую надо заменить
* @param with Строка, на которую произведется заменя
* @return Изменённая строка с замененными частями
*/

char *str_replace(char *origin, const char *replacement, const char *with);

#endif  // STRING_H
