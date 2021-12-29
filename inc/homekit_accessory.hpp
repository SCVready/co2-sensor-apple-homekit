/**
 * @author Alejandro Solozabal
 *
 */

#ifndef HOMEKIT_ACCESSORY_HPP_
#define HOMEKIT_ACCESSORY_HPP_

#include <functional>

using std::function<void(float)> = DataRequestCallback;

struct HomekitAccessoryConfig
{
    int a;
}

class HomekitAccessory
{
    public:
        HomekitAccessory(DataRequestCallback co2_concentration_callback, DataRequestCallback temperature_callback, DataRequestCallback relative_humidity_callback);
        ~HomekitAccessory();
        Start();
        Stop();
        ResetNetwork();
        ResetFactory();

    private:
        RegisterService(HomekitService& service); /* using HomekitService::GetServiceHandler() */ 

        volatile bool m_running = false;
        Co2HomekitConfig m_config;
        static const char m_log_tag[];
        DataRequestCallback m_co2_concentration_callback;
        DataRequestCallback m_temperature_callback;
        DataRequestCallback m_relative_humidity_callback;
        TaskHandle_t m_task_handle;
}

#endif
