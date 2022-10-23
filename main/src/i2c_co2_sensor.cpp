#include "i2c_co2_sensor.hpp"

#include <iostream>

const char Co2Sensor::m_log_tag[] = {"Co2Sensor"};

static void vTaskCode(void* paramerter)
{
    reinterpret_cast<Co2Sensor*>(paramerter)->ExecutionCycle();
}

Co2Sensor::Co2Sensor(Co2SensorConfig config, std::function<void(float, float, float)> callback) : I2cDevice(SCD30_i2c_address), m_callback(callback)
{
    ESP_LOGI(m_log_tag, "Co2Sensor created");

    SetSelfCalibrationStatus(true);
    SetTemperatureOffset(0.1);
    SetAltitudeCompensation(20);

    bool self_calibration = false;
    GetSelfCalibrationStatus(self_calibration);
    float temperature_offset = 0;
    GetTemperatureOffset(temperature_offset);
    uint16_t altitude_compensation = 0;
    GetAltitudeCompensation(altitude_compensation);

    ESP_LOGI(m_log_tag, "Self calibration %d", self_calibration);
    ESP_LOGI(m_log_tag, "Temperature offset: %.2f°C", temperature_offset);
    ESP_LOGI(m_log_tag, "Altitude compensation: %um", altitude_compensation);
}

Co2Sensor::~Co2Sensor()
{
    Stop();
    ESP_LOGI(m_log_tag, "Co2Sensor destroyed");
}

int Co2Sensor::Start()
{
    if (m_running == false)
    {
        m_running = true;
        xTaskCreate(vTaskCode, "Co2Sensor", TaskStackSize, this, TaskPriority, &m_task_handle);

        ESP_LOGI(m_log_tag, "Co2Sensor started taking meassures");
    }

    return 0;
}

int Co2Sensor::Stop()
{
    if (m_running == true)
    {
        ESP_LOGI(m_log_tag, "Co2Sensor stopped taking meassures");
        m_running = false;
        vTaskDelete(m_task_handle);
    }

    return 0;
}

int Co2Sensor::SetSelfCalibrationStatus(bool enabled)
{
    std::array<uint8_t, 2> command = {0x53, 0x06};
    std::array<uint8_t, 3> data = {0x00, 0x00, 0x81};
    if(enabled)
    {
        data[1] = 0x01;
        data[2] = 0xB0;
    }

    return WriteCommand<2, 3>(command, data);
}

int Co2Sensor::GetSelfCalibrationStatus(bool& enabled)
{
    int ret_val = -1;
    std::array<uint8_t, 2> command = {0x53, 0x06};
    std::array<uint8_t, 3> data = {0};

    enabled = false;

    if (0 == ReadCommand<2, 3>(command, data))
    {
        if(data[1] == 0x01)
        {
            enabled = true;
        }
        ret_val = 0;
    }

    return ret_val;
}

int Co2Sensor::SetTemperatureOffset(float temperature)
{
    uint16_t value16 = static_cast<uint16_t>(temperature * 100);
    std::array<uint8_t, 2> value = {(uint8_t)(value16>8), (uint8_t)(value16)};
    uint8_t crc = ComputeCRC(value);

    std::array<uint8_t, 2> command = {0x54, 0x03};
    std::array<uint8_t, 3> data = {value[0], value[1], crc};

    return WriteCommand<2, 3>(command, data);
}

int Co2Sensor::GetTemperatureOffset(float& temperature)
{
    int ret_val = -1;
    uint16_t temperature_offset = 0;
    std::array<uint8_t, 2> command = {0x54, 0x03};
    std::array<uint8_t, 3> data = {};

    if (0 == ReadCommand<2, 3>(command, data))
    {
        temperature_offset = (data[0] < 8) + data[1];
        temperature = temperature_offset/100.0;
        ret_val = 0;
    }

    return ret_val;
}

int Co2Sensor::SetAltitudeCompensation(uint16_t altitude)
{
    std::array<uint8_t, 2> value = {(uint8_t)(altitude>8), (uint8_t)(altitude)};
    uint8_t crc = ComputeCRC(value);

    std::array<uint8_t, 2> command = {0x51, 0x02};
    std::array<uint8_t, 3> data = {value[0], value[1], crc};

    return WriteCommand<2, 3>(command, data);
}

int Co2Sensor::GetAltitudeCompensation(uint16_t& altitude)
{
    int ret_val = -1;
    std::array<uint8_t, 2> command = {0x51, 0x02};
    std::array<uint8_t, 3> data = {};

    if (0 == ReadCommand<2, 3>(command, data))
    {
        altitude = (data[0] < 8) + data[1];
        ret_val = 0;
    }

    return ret_val;
}

int Co2Sensor::GetReadyStatus(bool& ready)
{
    int ret_val = -1;
    std::array<uint8_t, 2> command = {0x02, 0x02};
    std::array<uint8_t, 3> data = {};

    ready = false;

    if (0 == ReadCommand<2, 3>(command, data))
    {
        if(data[1] == 0x01)
        {
            ready = true;
        }
        ret_val = 0;
    }

    return ret_val;
}

int Co2Sensor::ReadMeasurements(float& co2_concentration, float& temperature, float& relative_humidity)
{
    int ret_val = -1;
    std::array<uint8_t, 2> command = {0x03, 0x00};
    std::array<uint8_t, 18> data = {};

    if (0 == I2cDevice::ReadCommand<2, 18>(command, data))
    {
        std::array<uint8_t, 4> co2_concentration_data = {data[0],data[1],data[3],data[4]};
        std::array<uint8_t, 4> temperature_data       = {data[6],data[7],data[9],data[10]};
        std::array<uint8_t, 4> rel_humidity_data      = {data[12],data[13],data[15],data[16]};

        co2_concentration = ConvertMeasureData(co2_concentration_data);
        temperature       = ConvertMeasureData(temperature_data);
        relative_humidity = ConvertMeasureData(rel_humidity_data);

        ret_val = 0;
    }

    return ret_val;
}

int Co2Sensor::SoftReset()
{
    return 0;
}

float Co2Sensor::ConvertMeasureData(std::array<uint8_t, 4> data)
{
    unsigned int tempU32 = (unsigned int)((((unsigned int)data[0]) << 24) |
                                          (((unsigned int)data[1]) << 16) |
                                          (((unsigned int)data[2]) << 8) |
                                          ((unsigned int)data[3]));

    return *(float*)&tempU32;
}

uint8_t Co2Sensor::ComputeCRC(std::array<uint8_t, 2> data)
{
    const uint8_t polinomial = 0x31;
    uint8_t crc = 0xFF;

    for (uint8_t i = 0; i < data.size(); i++)
    {
        crc ^= (data[i]);
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ polinomial;
            } 
            else
            {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

void Co2Sensor::ExecutionCycle()
{
    bool meassures_ready = false;
    float co2_concentration = 0;
    float temperature = 0;
    float relative_humidity = 0;

    while(m_running)
    {
        GetReadyStatus(meassures_ready);

        if(meassures_ready)
        {
            ESP_LOGI(m_log_tag, "Measures ready");
            ReadMeasurements(co2_concentration, temperature, relative_humidity);
            m_callback(co2_concentration, temperature, relative_humidity);
        }

        vTaskDelay(100 / portTICK_RATE_MS);
    }
}