#include <driver/adc.h>
#include <math.h>
#include "esp32-lora-board-pins.h"
#include "voltage.h"
#include "sleep-wakeup.h"

#ifdef __cplusplus
extern "C"
{
#endif

  const float FACTOR_6_DB = 2 * 2.2 / 4095;  // Spannungsteiler 1-zu-2, ADC_ATTEN_DB_6
  const float FACTOR_11_DB = 2 * 3.70 / 4095; // Spannungsteiler 1-zu-2, ADC_ATTEN_DB_11

  int number_round = 10;
  int adc_reading_solar = 0;
  int adc_reading_powerpath = 0;
  int adc_reading_vcc2 = 0;

  uint8_t payload[PAYLOAD_LENGTH];
  sensor_values_t sensor_values;

  /*   Polynom bestimmt aus den folgenden ADC- und Spannungs-Werten (3.3 Volt)
      3.665  4.2
      3.296  3.8
      2.916  3.4
      2.543  3.0
      2.072  2.5

      y = 5.814402272·10-2 x4 - 6.614275168·10-1 x3 + 2.785160725 x2 - 4.077128492 x + 3.802638788
      polynom_coeffients_t module1_42Volt = {0.133858560, +1.99756137, -0.41139964, +0.176813638, -0.026239073};
      polynom_coeffients_t module2_33Volt = {-9.72728919, +12.975906, -5.48479196, +1.095972506, -0.0816047888};
 */

  void initVoltage()
  {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(SOLAR_MEASURE_CHANNEL, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(POWER_PATH_MEASURE_CHANNEL, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(VCC2_MEASURE_CHANNEL, ADC_ATTEN_DB_11);
  }

  int readRoundedAdc(adc1_channel_t channel)
  {
    printf("ADC: (Channel=%d)\n", channel);
    int adc;
    int adc_sum = 0;

    for (size_t i = 0; i < number_round; i++)
    {
      adc = adc1_get_raw(channel);

      adc_sum += adc;
      printf("%-4d\t", adc);
      if ((i % 20) == 19)
        printf("\n");
    }

    adc_sum = adc_sum / number_round;
    printf("Rounded ADC value=%-4d\n", adc_sum);

    return adc_sum;
  }

  double calulateVoltageCompensated(double adc, polynom_coeffients_t coeff)
  {
    /*
    google search term: "polynominterpolation online"
    Calulate ploynom with
    http://www.xuru.org/rt/PR.asp
    http://www.jaik.de/js/Interpolation.htm
    https://valdivia.staff.jade-hs.de/interpol.html
  */

    if (adc < 1 || adc > 4095)
      return 0;

    adc = adc / 1000;

    return coeff.c4 * pow(adc, 4) + coeff.c3 * pow(adc, 3) + coeff.c2 * pow(adc, 2) + coeff.c1 * adc + coeff.c0;
  }

  water_level_t getWaterLevel()
  {
    return gpio_get_level(WATER_LEVEL_PIN) == 0 ? LOW : HIGH;
  }

  void decodeToPayload(sensor_values_t values)
  {
    payload[0] = values.waterLevel;

    int16_t val = values.solarVoltage * 100;
    payload[1] = val >> 8;
    payload[2] = val;

    val = values.powerPathVoltage * 100;
    payload[3] = val >> 8;
    payload[4] = val;

    val = values.vcc2Voltage * 100;
    payload[5] = val >> 8;
    payload[6] = val;

    payload[7] = operation_mode;
    payload[8] = values.bootCount >> 8;
    payload[9] = values.bootCount;
    payload[10] = values.execTooLongCount >> 8;
    payload[11] = execTooLongCount;
  }

  float readVoltage(adc1_channel_t channel)
  {
    int adc_reading = readRoundedAdc(channel);
    return (float)adc_reading * FACTOR_11_DB;

    // voltage = calulateVoltageCompensated(adc_reading, module2_33Volt);
  }
  void readSensorValues()
  {
    initVoltage();

    sensor_values.solarVoltage = readVoltage(SOLAR_MEASURE_CHANNEL);
    sensor_values.powerPathVoltage = readVoltage(POWER_PATH_MEASURE_CHANNEL);
    sensor_values.vcc2Voltage = readVoltage(VCC2_MEASURE_CHANNEL);

    printf("Solarpanel-Voltage: %f Volt)\n", sensor_values.solarVoltage);
    printf("PowerPath-Voltage: %f Volt)\n", sensor_values.powerPathVoltage);
    printf("VCC_2-Voltage: %f Volt)\n", sensor_values.vcc2Voltage);

    sensor_values.waterLevel = getWaterLevel();
    printf("Water Level is %s  %i \n", sensor_values.waterLevel == HIGH ? "High" : "LOW", sensor_values.waterLevel);

    decodeToPayload(sensor_values);
  }

#ifdef __cplusplus
}
#endif