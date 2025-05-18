#ifndef MONGOOSE_REPSONSES_H
#define MONGOOSE_REPSONSES_H

/**
* @brief Ошибки, которые могу возникнуть в процессе
*/

enum {
    ERR_OK                   = 0,
    ERR_FAILED_INITIALIZE    = 1,
    ERR_MISSING_USERNAME     = 2, 
    ERR_FAILED_SAVE_REVIEW   = 3
};

/**
* @brief Структура результата
*/

typedef struct {
    int status_code;
    int error_code;
    char* response;
    char* content_type;
} PostResult;

/**
* @brief Иницилизирует результат
* @return Возращает заготовленный результат (Default Error)
*/

PostResult post_result_init();

/**
* @brief Получаем страницу о том, что запись была совершенны успешно
* @param result Заготовленный результат, в который будем вписывать данные
* @param hm Структура mongoose, из которой будем брать данные
*/

void get_response_success(PostResult *result, struct mg_http_message *hm);

/**
* @brief Получаем стили css
* @param result Заготовленный результат, в который будем вписывать данные
*/

void get_styles(PostResult *result);

/**
* @brief Получаем обычную страницу
* @param result Заготовленный результат, в который будем вписывать данные
*/

void get_main_page(PostResult *result);

#endif  // MONGOOSE_REPSONSES_H
