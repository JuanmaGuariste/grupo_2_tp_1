/*
 * Copyright (c) 2023 Sebastian Bedin <sebabedin@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @author : Sebastian Bedin <sebabedin@gmail.com>
 */

/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"
#include "task_button.h"
#include "task_ui.h"
#include "task_led.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/
ao_interface_t queue_interface_handler;
/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

extern SemaphoreHandle_t hsem_button;
extern SemaphoreHandle_t hsem_led;

/********************** internal functions definition ************************/

/********************** external functions definition ************************/
void ao_ui_init(void)
{
  queue_interface_handler.user_interface_queue_event = xQueueCreate(1, sizeof(button_type_t));
  queue_interface_handler.user_interface_queue_action = xQueueCreate(1, sizeof(led_color_t));

  xTaskCreate(task_ui,
              "Button Task",
              128, NULL,
              tskIDLE_PRIORITY + 1,
              NULL);
}

void ao_ui_send_button_event(button_type_t button_event) {
  xQueueSend(queue_interface_handler.user_interface_queue_event,(void *)&button_event, portMAX_DELAY);
}

void ao_ui_receive_led_action(led_color_t *color) {
  xQueueReceive(queue_interface_handler.user_interface_queue_action,(void *)color, portMAX_DELAY);
}

void task_ui(void *argument)
{
  button_type_t button_type;
  while (true)
  {
    if(pdTRUE == xQueueReceive(queue_interface_handler.user_interface_queue_event,&button_type, portMAX_DELAY))
    {
      LOGGER_INFO("ui led activate: %d", button_type);
      led_color_t color_to_send = LED_COLOR_NONE;
      switch (button_type)
      {
      case BUTTON_TYPE_PULSE:
        color_to_send = LED_COLOR_RED;
        break;
      case BUTTON_TYPE_SHORT:
        color_to_send = LED_COLOR_GREEN;
        break;
      case BUTTON_TYPE_LONG:
        color_to_send = LED_COLOR_BLUE;
        break;
      default:
        break;
      }
      if (color_to_send != LED_COLOR_NONE)
        xQueueSend(queue_interface_handler.user_interface_queue_action,(void *)&color_to_send, portMAX_DELAY);
    }
  }
}

/********************** end of file ******************************************/
