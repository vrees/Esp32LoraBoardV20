#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_sleep.h"
#include "esp_mac.h"
#include "TheThingsNetwork.h"

#include "esp32-lora-board-pins.h"
#include "sleep-wakeup.h"
#include "voltage.h"
#include "timer.h"
#include "string.h"
#include "loraconf.h"

extern uint8_t payload[];

static TheThingsNetwork ttn;

// Sensor eingebaut in Quelle Buehlhof

const unsigned TX_INTERVAL = 5;

void initEsp32Resources()
{
    esp_err_t err;
    // Initialize the GPIO ISR handler service
    err = gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    ESP_ERROR_CHECK(err);

    // Initialize the NVS (non-volatile storage) for saving and restoring the keys
    err = nvs_flash_init();
    ESP_ERROR_CHECK(err);

    // Initialize SPI bus
    spi_bus_config_t spi_bus_config;
    memset(&spi_bus_config, 0, sizeof(spi_bus_config_t));
    spi_bus_config.miso_io_num = TTN_PIN_SPI_MISO;
    spi_bus_config.mosi_io_num = TTN_PIN_SPI_MOSI;
    spi_bus_config.sclk_io_num = TTN_PIN_SPI_SCLK;
    spi_bus_config.quadwp_io_num = -1;
    spi_bus_config.quadhd_io_num = -1;
    spi_bus_config.max_transfer_sz = 0;
    err = spi_bus_initialize(TTN_SPI_HOST, &spi_bus_config, TTN_SPI_DMA_CHAN);
    ESP_ERROR_CHECK(err);
}

void printRFSettings(const char *window, const TTNRFSettings &settings)
{
    int bw = (1 << (static_cast<int>(settings.bandwidth) - 1)) * 125;
    int sf = static_cast<int>(settings.spreadingFactor) + 5;

    if (settings.spreadingFactor == kTTNSFNone)
    {
        printf("%s: not used\n", window);
    }
    else if (settings.spreadingFactor == kTTNFSK)
    {
        printf("%s: FSK, BW %dkHz, %lu Hz\n",
               window, bw, settings.frequency);
    }
    else
    {
        printf("%s: SF%d, BW %dkHz, %lu Hz\n",
               window, sf, bw, settings.frequency);
    }
}

void printAllRFSettings()
{
    printRFSettings("TX ", ttn.txSettings());
    printRFSettings("RX1", ttn.rx1Settings());
    printRFSettings("RX2", ttn.rx2Settings());
    printf("RSSI: %d dBm\n", ttn.rssi());
}

void sendMessages(void *pvParameter)
{
    initTimeoutTimer();
    TTNResponseCode res = ttn.transmitMessage(payload, PAYLOAD_LENGTH);
    stopExecutionTimer();

    printf(res == kTTNSuccessfulTransmission ? "Message sent.\n" : "Transmission failed.\n");

    printAllRFSettings();

    vTaskDelay(TX_INTERVAL * 1000 / portTICK_PERIOD_MS);

    // Wait until TTN communication is idle and save state
    ttn.waitForIdle();
    ttn.prepareForDeepSleep();

    // ttn.shutdown();

    powerOffAndSleep(false);
}

void messageReceived(const uint8_t *message, size_t length, port_t port)
{
    printf("*********************************************\n");
    printf("Message of %d bytes received on port %d:", length, port);
    for (int i = 0; i < length; i++)
        printf(" %02x", message[i]);
    printf("\n");
    printf("*********************************************\n");
}

void showMacAddress()
{
    uint8_t mac[6];
    esp_err_t err = esp_efuse_mac_get_default(mac);
    ESP_ERROR_CHECK(err);
    printf("MAC-Adress:   ");
    for (int i = 0; i < 6; i++)
        printf(" %02x", mac[i]);
    printf("\n");
}

extern "C" void app_main(void)
{
    printf("Start app on ESP32LoraBoard\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Take some time to open up the Serial Monitor

    wakeupAndInit();

    initEsp32Resources();

    // Configure the SX127x pins
    ttn.configurePins(TTN_SPI_HOST, TTN_PIN_NSS, TTN_PIN_RXTX, TTN_PIN_RST, TTN_PIN_DIO0, TTN_PIN_DIO1);

    showMacAddress();
    ttn.provisionWithMAC(appEui, appKey);

    // Register callback for received messages
    ttn.onMessage(messageReceived);

    readSensorValues();

    if (ttn.resumeAfterDeepSleep())
    {
        printf("Resumed from deep sleep.\n");
    }
    else
    {
        printf("Joining...\n");
        if (ttn.join())
        {
            printf("Joined.\n");
        }
        else
        {
            printf("Join failed. Goodbye\n");
            return;
        }
    }

    // send data and deep sleep afterwards
    xTaskCreate(sendMessages, "send_messages", 1024 * 4, (void *)0, 3, nullptr);
}