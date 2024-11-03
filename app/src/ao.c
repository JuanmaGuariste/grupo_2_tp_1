/*
* ao.c
* author: José Luis Krüger
*/

#include "ao.h"
#include "task_button.h"

void active_object_init(active_object_t *obj, event_callback_t process_event, size_t queue_size) {
    obj->event_queue = xQueueCreate(queue_size, sizeof(obj->event_size));
    obj->process_event = process_event;
}

void active_object_send_event(active_object_t *obj, event_data_t event) {
    xQueueSend(obj->event_queue, event, 0);
}

void active_object_task(void *pv_parameters) {
    active_object_t *obj = (active_object_t *) pv_parameters;
    event_data_t event = (event_data_t)pvPortMalloc(obj->event_size);

    for (;;) {
        while (xQueueReceive(obj->event_queue, event, 0) == pdTRUE) {
            obj->process_event(event);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
