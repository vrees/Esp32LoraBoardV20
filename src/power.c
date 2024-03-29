#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp32-lora-board-pins.h"

#define GPIO_OUTPUT_PIN_SEL ((1ULL << POWER_ENABLE) | (1ULL << TRIGGER))
#define GPIO_INPUT_PIN_SEL ((1ULL << SOLAR_MEASURE) | (1ULL << POWER_PATH_MEASURE) | (1ULL << VCC_2_MEASURE) | (1ULL << TEMPERATURE) | (1ULL << WATER_LEVEL_PIN) | (1ULL << ECHO))

#ifdef __cplusplus
extern "C"
{
#endif

    void initIoPorts()
    {
        printf("Init IO ports\n");
        // Configure all output pins
        gpio_config_t io_conf;
        // disable interrupt
        io_conf.intr_type = GPIO_INTR_DISABLE;
        // set as output mode
        io_conf.mode = GPIO_MODE_OUTPUT;
        // bit mask of the pins that you want to set
        io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
        // disable pull-down mode
        io_conf.pull_down_en = 0;
        // disable pull-up mode
        io_conf.pull_up_en = 0;
        // configure GPIO with the given settings
        gpio_config(&io_conf);

        // Configure all input pins
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
        io_conf.pull_down_en = 0;
        io_conf.pull_up_en = 0;
        gpio_config(&io_conf);
    }

    void enablePeripheralPower()
    {
        printf("Enabling Peripheral Power\n");
        gpio_set_level((gpio_num_t)POWER_ENABLE, 0);
    }

    void disablePeripheralPower()
    {
        printf("Disabling Peripheral Power\n");
        gpio_set_level((gpio_num_t)POWER_ENABLE, 1);
    }

#ifdef __cplusplus
}
#endif