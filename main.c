#include "./src/mongoose/mongoose.h"     
#include "./src/files/input.h"    
#include "./src/files/output.h"
#include "./src/text/text.h"
#include "./constants.h"   
#include "./src/mongoose/mongoose_responses.h"
#include <stdlib.h>                  
#include <stdbool.h>

// Функция обработки входящих HTTP-запросов
static int process_request(struct mg_connection *c, 
                           struct mg_http_message *hm) {
    PostResult result = post_result_init();
    
    // Если получен POST запрос
    if (!mg_strcmp(    hm->uri,    mg_str("/postrating")) &&
        !mg_strcasecmp(hm->method, mg_str("POST")) 
       ) {
	    get_response_success(&result, hm);
    }

    // Если запрашивается файл стилей
    else if (!mg_strcmp(hm->uri, mg_str("/styles.css"))) {
	get_styles(&result);
    } 

    // Если запрашивается что-либо другое — возвращаем главную страницу rating.html
    else {
        get_main_page(&result);
    }

    
    if (result.error_code == ERR_OK)			      // Если не было ошибок — отправляем ответ клиенту
        mg_http_reply(c, result.status_code, result.content_type, "%s", result.response); 
    else						      // В случае ошибки — пустой ответ с кодом 500
        mg_http_reply(c, 500, "", ""); 

    free(result.response); 			// Освобождение выделенной памяти
    return result.error_code; 			// Возврат кода ошибки
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
