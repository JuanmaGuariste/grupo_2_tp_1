/*
* ao.c
* author: José Luis Krüger
*/

#include "ao.h"
#include "task_button.h"
#include "logger.h"
// constantes definidas para facilitar el debugging del programa
static char *QUEUE_ID_1 = "Queue_id_1";
static char *QUEUE_ID_2 = "Queue_id_2";
static char *QUEUE_ID_3 = "Queue_id_3";
static char *QUEUE_ID_4 = "Queue_id_4";
static char *QUEUE_ID_DEF = "Queue_id_default";

 char *get_queue_name( uint8_t id) {
     char *buffer[4] = { 0 };
     if (id > 4) return QUEUE_ID_DEF;

     buffer[0] = QUEUE_ID_1;
     buffer[1] = QUEUE_ID_2;
     buffer[2] = QUEUE_ID_3;
     buffer[3] = QUEUE_ID_4;

    return buffer[id - 1];
}

void active_object_init(active_object_t *obj, event_callback_t process_event, size_t queue_size) {
    get_queue_name(obj->obj_id);
    obj->event_queue = xQueueCreate(queue_size, obj->event_size);
    configASSERT(NULL != obj->event_queue);
    vQueueAddToRegistry(obj->event_queue, get_queue_name(obj->obj_id));
    obj->process_event = process_event;
//    LOGGER_INFO("Se inicializa el objeto activo id: %d, Tamaño del evento: %d",obj->obj_id, obj->event_size);
}

void active_object_send_event(active_object_t *obj, event_data_t event) {
    xQueueSend(obj->event_queue, event, 0);
//    LOGGER_INFO("Se envía un evento, objeto activo id: %d"
//                ", Tamaño del evento: %d",
//                obj->obj_id,
//                obj->event_size);
}

void active_object_task(void *pv_parameters) {
    active_object_t *obj = (active_object_t *) pv_parameters;
    event_data_t event = (event_data_t)pvPortMalloc(obj->event_size);

    for (;;) {
        while (xQueueReceive(obj->event_queue, event, 0) == pdTRUE) {
//                LOGGER_INFO("Se recibe un evento, objeto activo id: %d"
//                ", Tamaño del evento: %d --> se despacha al handler correspondiente",
//                obj->obj_id,
//                obj->event_size);
            obj->process_event(event);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
