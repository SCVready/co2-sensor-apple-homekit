
#ifndef HOMEKIT_DEVICE_HPP_
#define HOMEKIT_DEVICE_HPP_

#include <functional>
#include <array>

extern "C" {
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <hap.h>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include <iot_button.h>
#include <app_wifi.h>
#include <app_hap_setup_payload.h>
}

#include "homekit_service.hpp"
#include "co2_sensor.hpp"

struct HomeKitRequestCallbacks
{
    std::function<void(bool&)> StatusActiveCallback;
    std::function<void(uint32_t&)> StatusFaultCallback;
    std::function<void(uint32_t&)> StatusTamperedCallback;
    std::function<void(uint32_t&)> StatusLowBatteryCallback;
    std::function<void(uint32_t&)> CarbonDioxideDetectedCallback;
    std::function<void(float&)> CarbonDioxideLevelCallback;
    std::function<void(float&)> CarbonDioxidePeakLevelCallback;
    std::function<void(float&)> TemperatureCallback;
    std::function<void(float&)> RelativeHumidityCallback;
};

struct HomekitDeviceConfig
{
    int a;
};

class HomekitDevice
{
    public:
        HomekitDevice(HomekitDeviceConfig config, HomeKitRequestCallbacks homekit_callbacks);
        ~HomekitDevice();
        int Start();
        int Stop();
        int ResetNetwork();
        int ResetFactory();
        void ExecutionCycle();

        static int sensor_identify(hap_acc_t *accessory);

    private:
        hap_acc_t* m_accessory = nullptr;
        hap_acc_cfg_t m_accessory_config = {
            .name = "Carbon dioxide sensor",
            .model = "CO2Sensor",
            .manufacturer = "SCVready",
            .serial_num = "001122334455",
            .fw_rev = "0.1.0",
            .hw_rev = "0.1.0",
            .pv = "1.1.0",
            .cid = HAP_CID_SENSOR,
            .identify_routine = HomekitDevice::sensor_identify,
        };

        std::array<uint8_t, 8> m_product_data = {'E','S','P','3','2','H','A','P'};

        volatile bool m_running = false;
        HomekitDeviceConfig m_config;
        static const char m_log_tag[];
        TaskHandle_t m_task_handle;

        HomeKitRequestCallbacks m_homekit_callbacks;

        std::shared_ptr<HomekitService<HomekitServiceType::Co2Sensor>> m_co2_sensor_service;
        std::shared_ptr<HomekitService<HomekitServiceType::TemperatureSensor>> m_temperature_sensor_service;
        std::shared_ptr<HomekitService<HomekitServiceType::HumiditySensor>> m_humidity_sensor_service;

        StatusRequestCallbacks m_status_service_callbacks = {
            m_homekit_callbacks.StatusActiveCallback,
            m_homekit_callbacks.StatusFaultCallback,
            m_homekit_callbacks.StatusTamperedCallback,
            m_homekit_callbacks.StatusLowBatteryCallback,
        };

        DataRequestCallbacks<HomekitServiceType::Co2Sensor> m_co2_sensor_service_callbacks = {
            m_homekit_callbacks.CarbonDioxideDetectedCallback,
            m_homekit_callbacks.CarbonDioxideLevelCallback,
            m_homekit_callbacks.CarbonDioxidePeakLevelCallback,
        };

        DataRequestCallbacks<HomekitServiceType::TemperatureSensor> m_temperature_sensor_service_callbacks = {
            m_homekit_callbacks.TemperatureCallback,
        };

        DataRequestCallbacks<HomekitServiceType::HumiditySensor> m_humidity_sensor_service_callbacks = {
            m_homekit_callbacks.RelativeHumidityCallback,
        };
};

#endif
