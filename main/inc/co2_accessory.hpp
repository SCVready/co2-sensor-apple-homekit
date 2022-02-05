
#ifndef CO2_ACCESSORY_HPP_
#define CO2_ACCESSORY_HPP_

#include <memory>

#include "homekit_device.hpp"
#include "co2_sensor.hpp"
#include "circular_buffer.hpp"

class Co2Accessory
{
    public:
        Co2Accessory();
        ~Co2Accessory();
        int Start();
        int Stop();
        void ExecutionCycle();
    private:
        void StatusActiveCallback(bool& active);
        void StatusFaultCallback(uint32_t& fault);
        void StatusTamperedCallback(uint32_t& tampered);
        void StatusLowBatteryCallback(uint32_t& low_batery);
        void CarbonDioxideDetectedCallback(uint32_t& carbon_dioxide_detected);
        void CarbonDioxideLevelCallback(float& carbon_dioxide_level);
        void CarbonDioxidePeakLevelCallback(float& carbon_dioxide_peak_level);
        void TemperatureCallback(float& temperature);
        void RelativeHumidityCallback(float& relative_humidity);

        void SensorMeasuresCallback(float co2_concentration, float temperature, float relative_humidity);

        uint32_t m_carbon_dioxide_detected = 0;
        float m_carbon_dioxide_level = 0;
        float m_carbon_dioxide_peak_level = 0;
        float m_temperature = 0;
        float m_relative_humidity = 0;

        bool m_active = 0;
        uint32_t m_fault = 0;
        uint32_t m_tampered = 0;
        uint32_t m_low_batery = 0;

        CircularBuffer<uint32_t, 30> m_last_minute_values;
        CircularBuffer<uint32_t, 60> m_last_hour_values;
        CircularBuffer<uint32_t, 60> m_last_day_values;

        HomekitDeviceConfig m_homekit_device_config;
        HomeKitRequestCallbacks m_homekit_callbacks = {
            std::bind(&Co2Accessory::StatusActiveCallback, this, std::placeholders::_1),
            std::bind(&Co2Accessory::StatusFaultCallback, this, std::placeholders::_1),
            std::bind(&Co2Accessory::StatusTamperedCallback, this, std::placeholders::_1),
            std::bind(&Co2Accessory::StatusLowBatteryCallback, this, std::placeholders::_1),
            std::bind(&Co2Accessory::CarbonDioxideDetectedCallback, this, std::placeholders::_1),
            std::bind(&Co2Accessory::CarbonDioxideLevelCallback, this, std::placeholders::_1),
            std::bind(&Co2Accessory::CarbonDioxidePeakLevelCallback, this, std::placeholders::_1),
            std::bind(&Co2Accessory::TemperatureCallback, this, std::placeholders::_1),
            std::bind(&Co2Accessory::RelativeHumidityCallback, this, std::placeholders::_1)
        };
        std::shared_ptr<HomekitDevice> m_homekit_device;

        Co2SensorConfig m_co2_sensor_config;
        std::shared_ptr<Co2Sensor> m_co2_sensor;

        static const char m_log_tag[];

        TaskHandle_t m_task_handle;
        volatile bool m_running = false;
};

#endif
