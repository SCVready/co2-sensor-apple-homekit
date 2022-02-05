
#include "co2_accessory.hpp"

const char Co2Accessory::m_log_tag[] = {"Co2Accessory"};

static void vTaskCode(void* paramerter)
{
    reinterpret_cast<Co2Accessory*>(paramerter)->ExecutionCycle();
}

Co2Accessory::Co2Accessory()
{
    m_homekit_device = std::make_shared<HomekitDevice>(m_homekit_device_config, m_homekit_callbacks);

    auto co2_sensor_callback = std::bind(&Co2Accessory::SensorMeasuresCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    m_co2_sensor = std::make_shared<Co2Sensor>(m_co2_sensor_config, co2_sensor_callback);
}

Co2Accessory::~Co2Accessory()
{
}

int Co2Accessory::Start()
{
    if (m_running == false)
    {
        m_running = true;
        m_homekit_device->Start();
        m_co2_sensor->Start();
        xTaskCreate(vTaskCode, "Co2Accessory", TaskStackSize, this, TaskPriority, &m_task_handle);
    }
    return 0;
}

int Co2Accessory::Stop()
{
    if (m_running == true)
    {
        m_running = false;
        m_homekit_device->Stop();
        m_co2_sensor->Stop();
        vTaskDelete(m_task_handle);
    }
    return 0;
}

void Co2Accessory::SensorMeasuresCallback(float co2_concentration, float temperature, float relative_humidity)
{
    ESP_LOGI(m_log_tag, "CO2: %.2f ppm", co2_concentration);
    ESP_LOGI(m_log_tag, "Temperature: %.2f °C", temperature);
    ESP_LOGI(m_log_tag, "Relative humidity: %.2f %%RH", relative_humidity);

    m_carbon_dioxide_level = co2_concentration;
    m_temperature = temperature;
    m_relative_humidity = relative_humidity;
}

void Co2Accessory::StatusActiveCallback(bool& active)
{
    active = m_active;
    ESP_LOGI(m_log_tag, "StatusActiveCallback");
}

void Co2Accessory::StatusFaultCallback(uint32_t& fault)
{
    fault = m_fault;
    ESP_LOGI(m_log_tag, "StatusFaultCallback");
}

void Co2Accessory::StatusTamperedCallback(uint32_t& tampered)
{
    tampered = m_tampered;
    ESP_LOGI(m_log_tag, "StatusTamperedCallback");
}

void Co2Accessory::StatusLowBatteryCallback(uint32_t& low_batery)
{
    low_batery = m_low_batery;
    ESP_LOGI(m_log_tag, "StatusLowBatteryCallback");
}

void Co2Accessory::CarbonDioxideDetectedCallback(uint32_t& carbon_dioxide_detected)
{
    carbon_dioxide_detected = m_carbon_dioxide_detected;
    ESP_LOGI(m_log_tag, "CarbonDioxideDetectedCallback");
}

void Co2Accessory::CarbonDioxideLevelCallback(float& carbon_dioxide_level)
{
    carbon_dioxide_level = m_carbon_dioxide_level;
    ESP_LOGI(m_log_tag, "CarbonDioxideLevelCallback");
}

void Co2Accessory::CarbonDioxidePeakLevelCallback(float& carbon_dioxide_peak_level)
{
    carbon_dioxide_peak_level = m_carbon_dioxide_peak_level;
    ESP_LOGI(m_log_tag, "CarbonDioxidePeakLevelCallback");
}

void Co2Accessory::TemperatureCallback(float& temperature)
{
    temperature = m_temperature;
    ESP_LOGI(m_log_tag, "TemperatureCallback");
}

void Co2Accessory::RelativeHumidityCallback(float& relative_humidity)
{
    relative_humidity = m_relative_humidity;
    ESP_LOGI(m_log_tag, "RelativeHumidityCallback");
}

void Co2Accessory::ExecutionCycle()
{
    TickType_t now = xTaskGetTickCount();
    TickType_t last_minute_tick = now;
    TickType_t last_hour_tick = now;

    while(m_running)
    {
        now = xTaskGetTickCount();

        if((pdTICKS_TO_MS(now - last_minute_tick)/1000) > 60)
        {
            ESP_LOGI(m_log_tag, "New minute");
            last_minute_tick = now;
            m_last_hour_values.Insert(0);
        }

        if((pdTICKS_TO_MS(now - last_hour_tick)/1000) > 3600)
        {
            ESP_LOGI(m_log_tag, "New Hour");
            last_hour_tick = now;
            m_last_day_values.Insert(0);
        }

        m_last_minute_values.Insert(m_carbon_dioxide_level);

        auto last_minute_max = m_last_minute_values.Max();

        if(last_minute_max > m_last_hour_values.Peek())
        {
            m_last_hour_values.Modify(last_minute_max);
        }

        auto last_hour_max = m_last_hour_values.Max();

        if(last_hour_max > m_last_day_values.Peek())
        {
            m_last_day_values.Modify(last_hour_max);
        }

        auto last_day_max = m_last_day_values.Max();

        m_carbon_dioxide_peak_level = last_day_max;
    }
}