
#include "freertos/FreeRTOS.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp32-lora-board-pins.h"
#include "power.h"
#include "sleep-wakeup.h"
#include "voltage.h"

#ifdef __cplusplus
}
#endif

#define MICROSEC_TO_SEC_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */

// #define SLEEP_TIME_NORMAL_SEC 2 * 24 * 60 *60  //  Tage
#define SLEEP_TIME_NORMAL_SEC 30 *60  // 30 Minuten
#define SLEEP_TIME_LOW_LEVEL_SEC 15 * 60  // alle 15 Minuten
// #define SLEEP_TIME_LOW_LEVEL_SEC 60  // jede Minute
#define SLEEP_TIME_IMMEDIATE_WAKEUP 5  // reboot if tranmission is not successful in between 20 seconds

operation_mode_t operation_mode = TIMER_WAKEUP;
RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR int execTooLongCount = 0;

/*
Method to print the reason by which ESP32
*/
void printWakeupReason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    printf("Wakeup caused by external signal using RTC_IO\n");
    operation_mode = LOW_LEVEL_WAKEUP;
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    printf("Wakeup caused by timer\n");
    operation_mode = TIMER_WAKEUP;
    break;
    /*   
  case ESP_SLEEP_WAKEUP_EXT1:
    printf("Wakeup caused by external signal using RTC_CNTL\n");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    printf("Wakeup caused by touchpad\n");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    printf("Wakeup caused by ULP program\n");
    break;
  case ESP_SLEEP_WAKEUP_GPIO:
    printf("Wakeup was not caused GPIO\n");
    break;
  case ESP_SLEEP_WAKEUP_UART:
    printf("Wakeup was not caused by UART\n");
    break;
 */
  default:
    printf("Wakeup was not caused by deep sleep. Reason=%i\n", wakeup_reason);
    operation_mode = DEBUG_WAKEUP;
    break;
  }
}

void wakeupAndInit()
{
  //Increment boot number and print it every reboot
  ++bootCount;
  sensor_values.bootCount = bootCount;
  sensor_values.execTooLongCount = execTooLongCount;
  printf("wakeup(). Boot number: %d\n", bootCount);

  //Print the wakeup reason for ESP32
  printWakeupReason();

  // define output level before port config to ensure unwanted glitch
  enablePeripheralPower();
  initIoPorts();
}

void powerOffAndSleep(bool rebootImmediately)
{
  printf("Preparing for deep sleep\n");

  disablePeripheralPower();
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  int sleepTimeInSeconds;

  if (getWaterLevelStatus() == NORMAL)
  {
    // if sensor detects low water level wakeup process is started
    esp_sleep_enable_ext0_wakeup(WATER_LEVEL_PIN, 0);
    sleepTimeInSeconds = SLEEP_TIME_NORMAL_SEC;
  }
  else
  {
    esp_sleep_enable_ext0_wakeup(WATER_LEVEL_PIN, 1);
    sleepTimeInSeconds = SLEEP_TIME_LOW_LEVEL_SEC;
  }

  if (rebootImmediately) {
    sleepTimeInSeconds = SLEEP_TIME_IMMEDIATE_WAKEUP;
  }

  esp_sleep_enable_timer_wakeup((uint64_t) sleepTimeInSeconds * MICROSEC_TO_SEC_FACTOR);
  printf("Setup ESP32 to sleep next %i in seconds\n", sleepTimeInSeconds);

  printf("Going to sleep now\n");
  fflush(stdout);

  esp_deep_sleep_start();
}

#ifdef __cplusplus
}
#endif