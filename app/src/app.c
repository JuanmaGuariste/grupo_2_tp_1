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

#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"

#include "task_button.h"
#include "task_led.h"
#include "task_ui.h"

#include "ao.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/
  static active_object_t red_led_obj, green_led_obj, blue_led_obj, ui_interface;
  static all_obt_t all_obj = {
    .blue_led = &blue_led_obj,
    .green_led = &green_led_obj,
    .red_led = &red_led_obj,
    .ui_obj = &ui_interface
  };
/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration *****************************/

/********************** external functions definition ************************/
void app_init(void)
{
  // Inicialización para log
  red_led_obj.obj_id = 1;
  green_led_obj.obj_id = 2;
  blue_led_obj.obj_id = 3;
  ui_interface.obj_id = 4;
  
  init_led_active_object(&red_led_obj, handle_red_led_event, 1);
  init_led_active_object(&green_led_obj, handle_green_led_event, 1);
  init_led_active_object(&blue_led_obj, handle_blue_led_event, 1);
  init_ui_active_object(&ui_interface, ui_process_event, 2);

  BaseType_t status;

  status = xTaskCreate(task_button, "Button_Task", configMINIMAL_STACK_SIZE, &all_obj, 2, NULL);
  while (pdPASS != status)
  {
    // error
  }

  LOGGER_INFO("app init");

  cycle_counter_init();
}

/********************** end of file ******************************************/
