#include <stdio.h>
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gptimer.h"
#include "sleep-wakeup.h"
#include "esp_log.h"


/*
 deprecated timer API. Use new gptimer sintead.
 see https://github.com/espressif/esp-idf/tree/v5.1/examples/peripherals/timer_group/gptimer


#define TIMER_DIVIDER 16                             //  Hardware timer clock divider
#define TIMER_SCALE (TIMER_BASE_CLK / TIMER_DIVIDER) // convert counter value to seconds
#define TIMER_TRANMISSION_TIMEOUT_SEC (20.0)                   // sample test interval for the first timer

typedef struct
{
    int type; // the type of timer's event
    int timer_group;
    int timer_idx;
    uint64_t timer_counter_value;
} timer_event_t;

xQueueHandle timer_queue;

static void inline print_timer_counter(uint64_t counter_value)
{
    printf("Counter: 0x%08x%08x\n", (uint32_t)(counter_value >> 32),
           (uint32_t)(counter_value));
    printf("Time   : %.8f s\n", (double)counter_value / TIMER_SCALE);
}

void IRAM_ATTR timer_group0_isr(void *para)
{
    timer_spinlock_take(TIMER_GROUP_0);
    int timer_idx = (int)para;

    uint32_t timer_intr = timer_group_get_intr_status_in_isr(TIMER_GROUP_0);
    uint64_t timer_counter_value = timer_group_get_counter_value_in_isr(TIMER_GROUP_0, timer_idx);

    timer_event_t evt;
    evt.timer_group = 0;
    evt.timer_idx = timer_idx;
    evt.timer_counter_value = timer_counter_value;

    if (timer_intr & TIMER_INTR_T0)
    {
        timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);
        timer_counter_value += (uint64_t)(TIMER_TRANMISSION_TIMEOUT_SEC * TIMER_SCALE);
        timer_group_set_alarm_value_in_isr(TIMER_GROUP_0, timer_idx, timer_counter_value);
    }
    else
    {
        evt.type = -1; // not supported even type
    }

    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, timer_idx);

    xQueueSendFromISR(timer_queue, &evt, NULL);
    timer_spinlock_give(TIMER_GROUP_0);
}

static void tg0_timer_init(int timer_idx, double timer_interval_sec)
{
    timer_config_t config = {
        .divider = TIMER_DIVIDER,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = TIMER_AUTORELOAD_DIS,
    }; // default clock source is APB
    timer_init(TIMER_GROUP_0, timer_idx, &config);


    timer_set_alarm_value(TIMER_GROUP_0, timer_idx, timer_interval_sec * TIMER_SCALE);
    timer_enable_intr(TIMER_GROUP_0, timer_idx);
    timer_isr_register(TIMER_GROUP_0, timer_idx, timer_group0_isr,
                       (void *)timer_idx, ESP_INTR_FLAG_IRAM, NULL);

    timer_start(TIMER_GROUP_0, timer_idx);
}

*/

static void powerOffAfterTimeout(void *arg)
{
/*     while (1)
    {
        timer_event_t evt;
        xQueueReceive(timer_queue, &evt, portMAX_DELAY);
        ++execTooLongCount;
        ++bootCount;
        powerOffAndSleep(true);
    } */
}

void initTimeoutTimer()
{
/*     timer_queue = xQueueCreate(10, sizeof(timer_event_t));
    tg0_timer_init(TIMER_0, TIMER_TRANMISSION_TIMEOUT_SEC);
    xTaskCreate(powerOffAfterTimeout, "timer_evt_task", 2048, NULL, 5, NULL);
 */
}

void stopExecutionTimer()
{
    // t imer_disable_intr(TIMER_GROUP_0, TIMER_0);
}
