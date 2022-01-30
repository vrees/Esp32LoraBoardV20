#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp32-lora-board-pins.h"
#include <sys/time.h>
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define HIGH 1
#define LOW 0

    const int64_t TIME_OUT = 100000; // us

    /**
     * returns true in case of timeout
     */
    bool waitForEchoState(int state)
    {
        int64_t startMicros = esp_timer_get_time();
        int64_t stopMicros, delta = 0;
        int level = 0;

        do
        {
            stopMicros = esp_timer_get_time();
            delta = stopMicros - startMicros;
            level = gpio_get_level(ECHO);
        } while ((level != state) && delta <= TIME_OUT);

        return delta >= TIME_OUT;
    }

    int64_t jsnsr04tTiming()
    {
        // TRIG pulse for 10ms
        gpio_set_level(TRIGGER, 1);
        ets_delay_us(30);
        gpio_set_level(TRIGGER, 0);

        int64_t startMicros = esp_timer_get_time();

        // wait for the echo pin HIGH or timeout
        if (waitForEchoState(HIGH))
        {
            printf("JSN-SR04T TimeOut High\n");
            return 0;
        }

        // wait for the echo pin LOW or timeout
        if (waitForEchoState(LOW))
        {
            printf("JSN-SR04T TimeOut LOW\n");
            return 0;
        }

        return esp_timer_get_time() - startMicros;
    }

    float measureDistance()
    {
        int64_t duration = jsnsr04tTiming();

        // Berechnung der Entfernung
        // Da der Weg doppel ist: Hinweg - Rückweg, muss der Wert durch 2 geteilt werden
        float distance = duration * 0.034 / 2;

        printf("JSN-SR04T Distance=%f\n", distance);

        return distance;
    }

#ifdef __cplusplus
}
#endif