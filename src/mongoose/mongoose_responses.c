#include "mongoose.h"     
#include "../files/input.h"    
#include "../files/output.h"
#include "../text/text.h"
#include "../../constants.h"
#include "mongoose_responses.h"
#include <stdlib.h>                  
#include <stdbool.h>

// Функция, иницилизирующая результат
PostResult post_result_init() {
    PostResult result;

    result.error_code = ERR_FAILED_INITIALIZE;
    result.status_code = 500;
    result.content_type = "";
    result.response = NULL;

    return result;
}

// Функция для записи значений от POST-запроса
void get_response_success(PostResult *result,
			 struct mg_http_message *hm) {

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
    if ( strlen(username)<=0 || strlen(rating)<=0 ) {				// Если не получили нужные данные, даем знать что они отсутсвуют
        result->error_code = ERR_MISSING_USERNAME;
	result->response = read_file(PATH_RATING_ERROR_HTML);
    } else {			   						// При наличии всех нужных пунктов, записываем в файл
	int writing_result = write_to_file(FILE_REVIEWS,
			"<p><span class=\"highlight\">%s</span>" 	 // Оценка пользователя
			"<span class=\"review-text\"> by <b>%s</b> <br>" // Имя Пользователя
			"%s <br> </span> </p>\n", 			 // Отзыв пользователя
			rating, username, review);			 // Используемые аргументы
	
        if ( !writing_result ) {						// При ошибке записи, выдать окно ошибки
	    result->error_code = ERR_FAILED_SAVE_REVIEW;
	    result->response = read_file(PATH_RATING_ERROR_HTML);
    } else {									// При успешной записи, выдать экран подтверждения
	result->response = read_file(PATH_RATING_SUCCESS_HTML);
        }
    }

    if (result->response) {
        result->status_code = 200;
	result->content_type = CONTENT_TYPE_HTML;
	result->error_code = ERR_OK;
    }
}

// Функция для получения стилей
void get_styles(PostResult *result) {
    result->response = read_file(PATH_CSS_STYLES);

    if (result->response) {
        result->status_code = 200;
	result->content_type = CONTENT_TYPE_CSS;
	result->error_code = ERR_OK;
    }
}

// Функция для получения обычной страницы
void get_main_page(PostResult *result) {
    result->response = read_file(PATH_RATING_HTML);
    if (result->response) {
	char *reviews = read_file(FILE_REVIEWS);				// Получаем отзывы
	if (!reviews) {	
	    result->response = str_replace(result->response, 			// Если их нет, даем пользователю знать
			                   "{{CONTENT}}", 
			                   "<p><span class=\"review-text\">"
					   "No reviews yet.</span><p>");
        } else {
	    result->response = str_replace(result->response, 	  		// Если они найдены, вставляем их в страницу
			                   "{{CONTENT}}", 
			                   reviews);
	    free(reviews);
        }
	
	if (result->response) {
	    result->status_code = 200;
	    result->content_type = CONTENT_TYPE_HTML;
	    result->error_code = ERR_OK;
	}
    }
}
