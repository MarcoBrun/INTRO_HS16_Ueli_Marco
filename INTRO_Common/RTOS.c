/**
 * \file
 * \brief Real Time Operating System (RTOS) main program.
 * \author Erich Styger, erich.styger@hslu.ch
 */

#include "Platform.h"
#if PL_CONFIG_HAS_RTOS
#include "RTOS.h"
#include "FRTOS1.h"
#include "LED.h"
#include "Event.h"
#include "Keys.h"
#include "Application.h"
#include "KeyDebounce.h"

#if 0
static void LedTask(void* param) {
  const int *whichLED = (int*)param;

  (void)param; /* avoid compiler warning */
  for(;;) {
    if (*whichLED==1) {
      LED1_Neg();
    } else if (*whichLED==2) {
      LED2_Neg();
    }
    /* \todo handle your application code here */
    FRTOS1_vTaskDelay(pdMS_TO_TICKS(500));
  }
}
#endif

static void AppTask(void *param) {
  for(;;) {
    #if PL_CONFIG_HAS_KEYS
      #if PL_CONFIG_HAS_DEBOUNCE
        KEYDBNC_Process();
      #else
        KEY_Scan();
      #endif
    #endif
    #if PL_CONFIG_HAS_EVENTS
      EVNT_HandleEvent(APP_EventHandler, TRUE);
    #endif
    FRTOS1_vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void RTOS_Init(void) {
#if 0
  static const int led1 = 1;
  static const int led2 = 2;
#endif
  EVNT_SetEvent(EVNT_STARTUP); /* set startup event */
#if 0
  /*! \todo Create tasks here */
  if (FRTOS1_xTaskCreate(LedTask, (uint8_t *)"Led1", configMINIMAL_STACK_SIZE, (void*)&led1, tskIDLE_PRIORITY, NULL) != pdPASS) {
    for(;;){} /* error case only, stay here! */
  }
  if (FRTOS1_xTaskCreate(LedTask, (uint8_t *)"Led2", configMINIMAL_STACK_SIZE, (void*)&led2, tskIDLE_PRIORITY, NULL) != pdPASS) {
    for(;;){} /* error case only, stay here! */
  }
#endif
  if (FRTOS1_xTaskCreate(AppTask, (uint8_t *)"App", configMINIMAL_STACK_SIZE+200, NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
    for(;;){} /* error case only, stay here! */
  }
}

void RTOS_Deinit(void) {
  /* nothing needed for now */
}

#endif /* PL_CONFIG_HAS_RTOS */
