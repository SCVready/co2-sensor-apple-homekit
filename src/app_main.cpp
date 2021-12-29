/**
 * @author Alejandro Solozabal
 *
 */

extern "C" {
#include <esp_log.h>
}

#include "co2_sensor.hpp"

extern "C" {
void app_main();
}

void MeasuresCallback(float co2_concentration, float temperature, float relative_humidity)
{
    ESP_LOGI(TAG_I2C, "CO2: %.2f ppm", co2_concentration);
    ESP_LOGI(TAG_I2C, "Temperature: %.2f °C", temperature);
    ESP_LOGI(TAG_I2C, "Relative humidity: %.2f %%RH", relative_humidity);
}

void app_main()
{
    Co2SensorConfig config;
    Co2Sensor co2_sensor(config, MeasuresCallback);
    co2_sensor.Start();

    while(1)sleep(1);
}
