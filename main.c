#include "./mongoose/mongoose.h"     
#include "./input/input.h"    
#include "./output/output.h"
#include "./constants/constants.h"   
#include <stdlib.h>                  
#include <stdbool.h>
#include <string.h>

enum {
    ERR_OK                   = 0, 
    ERR_MISSING_USERNAME     = 1, 
    ERR_FAILED_SAVE_REVIEW   = 2
};

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
    for (count = 0; (tmp = strstr(ins, replacement)); ++count) {
        ins = tmp + len_rep;
    }
    
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

// Функция обработки входящих HTTP-запросов
static int process_request(struct mg_connection *c, 
                           struct mg_http_message *hm) {
    int status_code   = 500;     
    const char *ctype = "";    
    char *response    = NULL;    
    int error_code    = ERR_MISSING_USERNAME; 

    // Проверка: если пришёл POST-запрос на /postrating
    if (!mg_strcmp(    hm->uri,    mg_str("/postrating")) &&
        !mg_strcasecmp(hm->method, mg_str("POST")) 
       ) {
        
	// Необходимые нам аргументы
        char username[100];
	char review[100];
	char rating[10];

        // Извлечение значений из тела POST-запроса
        mg_http_get_var(&hm->body, 
                            "username", username, sizeof(username));
	mg_http_get_var(&hm->body,
			"userreview", review, sizeof(review));
	mg_http_get_var(&hm->body,
			"userrating", rating, sizeof(rating));

	if ( strlen(username)<=0 || strlen(rating)<=0 ) {			// Если не получили нужные данные, даем знать что они отсутсвуют
            error_code = ERR_MISSING_USERNAME;
	    response = read_file(PATH_RATING_ERROR_HTML);
	} else {			   					// При наличии всех нужных пунктов, записываем в файл
	    int writing_result = write_to_file(FILE_REVIEWS, "%s,%s,%s\n", rating, username, review);
	    if ( !writing_result ) {						// При ошибки записи, выдать окно ошибки
	        error_code = ERR_FAILED_SAVE_REVIEW;
		response = read_file(PATH_RATING_ERROR_HTML);
	    } else {								// При успешной записи, выдать экран подтверждения
	        response = read_file(PATH_RATING_SUCCESS_HTML);
	    }
	}

        if (response) {								// При наличии response, сказать серверу что все ОК
            status_code = 200;
            ctype = CONTENT_TYPE_HTML;
            error_code = ERR_OK;
	}

    } 

    // Если запрашивается файл стилей
    else if (!mg_strcmp(hm->uri, mg_str("/styles.css"))) {
        response = read_file(PATH_CSS_STYLES); // Читаем файл CSS
        if (response) {
            status_code = 200;
            ctype = CONTENT_TYPE_CSS;
            error_code = ERR_OK;
        }

    } 

    // Если запрашивается что-либо другое — возвращаем главную страницу rating.html
    else {
        char *temp_response = read_file(PATH_RATING_HTML);
	char *reviews = read_file("reviews.txt");
	if (temp_response) {
	    if (!reviews) {				  // При отсутствии "reviews.txt", сказать что отзывов нету
		response = str_replace(temp_response, 
				       "{{CONTENT}}", 
				       "No reviews yet.");
	    } else {   					  // При наличии, осуществить его вставку в страницу сайта
	        response = str_replace(temp_response, 	  
				       "{{CONTENT}}", 
				       reviews);
		free(reviews);
	    }

	    free(temp_response);
	}

        if (response) {
            status_code = 200;
            ctype = CONTENT_TYPE_HTML;
            error_code = ERR_OK;
        }
    }

    
    if (error_code == ERR_OK)				      // Если не было ошибок — отправляем ответ клиенту
        mg_http_reply(c, status_code, ctype, "%s", response); 
    else						      // В случае ошибки — пустой ответ с кодом 500
        mg_http_reply(c, 500, "", ""); 

    free(response); 			// Освобождение выделенной памяти
    return error_code; 			// Возврат кода ошибки
}

// Основная функция-обработчик событий сервера
static void main_fun(struct mg_connection *c, 
                     int ev, 
                     void *ev_data) {
						 
    if (ev == MG_EV_HTTP_MSG) { 					 // Если произошло событие HTTP-запроса
        struct mg_http_message *hm = (struct mg_http_message *) ev_data; // Преобразуем данные события к нужному типу
        process_request(c, hm); 					 // Передаём запрос в обработчик

        // Здесь можно добавить логирование результата обработки
	// printf("Результат обработки: %d\n", result);
    }
}

// Точка входа в программу
int main(void) {
    const char *server_address = "http://localhost:8081"; 
    
    struct mg_mgr mgr; 
    mg_mgr_init(&mgr); 
    mg_http_listen(&mgr, server_address, main_fun, NULL); 

    printf("Сервер запущен на %s\n", server_address); 

    for (;;) mg_mgr_poll(&mgr, 1000); 

    mg_mgr_free(&mgr); 

    return 0;
}
