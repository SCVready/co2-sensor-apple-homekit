
#ifndef HOMEKIT_SERVICE_HPP_
#define HOMEKIT_SERVICE_HPP_

#include <functional>
#include <memory>

#include <hap.h>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>

enum class HomekitServiceType
{
    Co2Sensor,
    TemperatureSensor,
    HumiditySensor
};

struct StatusRequestCallbacks
{
    std::function<void(bool&)> StatusActiveCallback;
    std::function<void(uint32_t&)> StatusFaultCallback;
    std::function<void(uint32_t&)> StatusTamperedCallback;
    std::function<void(uint32_t&)> StatusLowBatteryCallback;
};

template<HomekitServiceType type>
struct DataRequestCallbacks;

template<> struct DataRequestCallbacks<HomekitServiceType::Co2Sensor>
{
    std::function<void(uint32_t&)> CarbonDioxideDetectedCallback;
    std::function<void(float&)> CarbonDioxideLevelCallback;
    std::function<void(float&)> CarbonDioxidePeakLevelCallback;
};

template<> struct DataRequestCallbacks<HomekitServiceType::TemperatureSensor>
{
    std::function<void(float&)> TemperatureCallback;
};

template<> struct DataRequestCallbacks<HomekitServiceType::HumiditySensor>
{
    std::function<void(float&)> RelativeHumidityCallback;
};

template<HomekitServiceType type>
class HomekitService
{
    public:
        HomekitService(StatusRequestCallbacks status_callbacks, DataRequestCallbacks<type> data_callbacks);
        ~HomekitService();
        hap_serv_t* GetServiceHandler();
        int ServiceValuesCallback(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv);
    private:
        static const char m_log_tag[];
        const char* m_service_name = "Carbon Dioxide sensor";
        hap_serv_t *m_service_handler = nullptr;
        StatusRequestCallbacks m_status_callbacks;
        DataRequestCallbacks<type> m_data_callbacks;
};

template<HomekitServiceType type>
hap_serv_t* HomekitService<type>::GetServiceHandler()
{
    return m_service_handler;
}

#endif
