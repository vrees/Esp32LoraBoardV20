#ifndef _ESP32_LORA_BOARD_PINS_H_
#define _ESP32_LORA_BOARD_PINS_H_

// Pins for LORA chip SPI interface, reset line and interrupt lines (compatible TTGO LoRa32 V2.1.6)
// #define LORA_SCK  (5) 
// #define LORA_CS   (18)
// #define LORA_MISO (19)
// #define LORA_MOSI (27)
// #define LORA_RST  (23)
// #define LORA_IRQ  (26)
// #define LORA_IO1  (33)
// #define LORA_IO2  (32)

// Pins and other resources
#define TTN_SPI_HOST      HSPI_HOST
#define TTN_SPI_DMA_CHAN  1
#define TTN_PIN_SPI_SCLK  5
#define TTN_PIN_SPI_MOSI  27
#define TTN_PIN_SPI_MISO  19
#define TTN_PIN_NSS       18 // also called CS
#define TTN_PIN_RXTX      TTN_NOT_CONNECTED
#define TTN_PIN_RST       23
#define TTN_PIN_DIO0      26
#define TTN_PIN_DIO1      25
#define TTN_PIN_DIO2      32


// JTAG ESP-Programmer
#define JTAG_TDI        (12)
#define JTAG_TCK        (13)
#define JTAG_TMS        (14)
#define JTAG_TDO        (15)

// JSN-SR04T
#define TRIGGER         (17)
#define ECHO            (4)

#define RESERVED        (2)

// Control Pins
#define POWER_ENABLE                (16)
#define SOLAR_MEASURE               (35)   // ADC1_CHANNEL_7
#define POWER_PATH_MEASURE          (34)   // ADC1_CHANNEL_6
#define VCC_2_MEASURE               (33)   // ADC1_CHANNEL_5
#define SOLAR_MEASURE_CHANNEL       ADC1_CHANNEL_7
#define POWER_PATH_MEASURE_CHANNEL  ADC1_CHANNEL_6
#define VCC2_MEASURE_CHANNEL        ADC1_CHANNEL_5
#define TEMPERATURE                 (39) 
#define WATER_LEVEL_PIN             GPIO_NUM_36   

// I2C Bus
#define I2C_SDA         (21)
#define I2C_SCL         (22)

// Serial Connection
#define TXDO            (1)
#define RXDO            (3)

#endif