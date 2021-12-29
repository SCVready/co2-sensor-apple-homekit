/**
 * @author Alejandro Solozabal
 *
 */

#ifndef HOMEKIT_SERVICE_HPP_
#define HOMEKIT_SERVICE_HPP_

#include <functional>

enum class HomekitServiceType
{
    Co2Sensor,
    TemperatureSensor,
    HumiditySensor
};

using std::function<void(float)> = DataRequestCallback;

template<HomekitServiceType type>
class HomekitService
{
    public:
        HomekitService(DataRequestCallback callback);
        ~HomekitService();
        hap_serv_t* GetServiceHandler();
    private:
        CreateService();/*Template expecialization*/
        RegisterService();/* hap_serv_set_read_cb( c callback ) && hap_serv_set_priv( HomekitService object pointer ) */

        hap_serv_t *service == nullptr;
}

#endif
