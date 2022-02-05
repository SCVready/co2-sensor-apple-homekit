
#include "homekit_device.hpp"
#include <esp_log.h>
#include <esp_wifi.h>

const char HomekitDevice::m_log_tag[] = {"HomekitDevice"};

HomekitDevice::HomekitDevice(HomekitDeviceConfig config, HomeKitRequestCallbacks homekit_callbacks) :
    m_config(config),
    m_homekit_callbacks(homekit_callbacks)
{
    /* Initialize the HAP core */
    hap_init(HAP_TRANSPORT_WIFI);

    /* Create accessory object */
    m_accessory = hap_acc_create(&m_accessory_config);

    /* Add a dummy Product Data */
    hap_acc_add_product_data(m_accessory, m_product_data.data(), m_product_data.size());

    /* Services */
    m_co2_sensor_service = std::make_shared<HomekitService<HomekitServiceType::Co2Sensor>>(m_status_service_callbacks, m_co2_sensor_service_callbacks);
    m_temperature_sensor_service = std::make_shared<HomekitService<HomekitServiceType::TemperatureSensor>>(m_status_service_callbacks, m_temperature_sensor_service_callbacks);
    m_humidity_sensor_service = std::make_shared<HomekitService<HomekitServiceType::HumiditySensor>>(m_status_service_callbacks, m_humidity_sensor_service_callbacks);

    hap_acc_add_serv(m_accessory, m_co2_sensor_service->GetServiceHandler());
    hap_acc_add_serv(m_accessory, m_temperature_sensor_service->GetServiceHandler());
    hap_acc_add_serv(m_accessory, m_humidity_sensor_service->GetServiceHandler());

    /* Add the Accessory to the HomeKit Database */
    hap_add_accessory(m_accessory);

#ifdef CONFIG_EXAMPLE_USE_HARDCODED_SETUP_CODE
    /* Unique Setup code of the format xxx-xx-xxx. Default: 111-22-333 */
    hap_set_setup_code(CONFIG_EXAMPLE_SETUP_CODE);
    /* Unique four character Setup Id. Default: ES32 */
    hap_set_setup_id(CONFIG_EXAMPLE_SETUP_ID);
#ifdef CONFIG_APP_WIFI_USE_WAC_PROVISIONING
    app_hap_setup_payload(CONFIG_EXAMPLE_SETUP_CODE, CONFIG_EXAMPLE_SETUP_ID, true, m_accessory_config.cid);
#else
    app_hap_setup_payload(CONFIG_EXAMPLE_SETUP_CODE, CONFIG_EXAMPLE_SETUP_ID, false, m_accessory_config.cid);
#endif
#endif

    /* Enable Hardware MFi authentication (applicable only for MFi variant of SDK) */
    hap_enable_mfi_auth(HAP_MFI_AUTH_HW);

    /* Initialize Wi-Fi */
    app_wifi_init();

    ESP_LOGI(m_log_tag, "HomekitDevice created");
}

HomekitDevice::~HomekitDevice()
{
    hap_stop();
    hap_acc_delete(m_accessory);
    hap_deinit();
}

int HomekitDevice::sensor_identify(hap_acc_t *accessory)
{
    ESP_LOGI(m_log_tag, "Sensor identified");
    return HAP_SUCCESS;
}

int HomekitDevice::Start()
{
    /* After all the initializations are done, start the HAP core */
    hap_start();

    /* Start Wi-Fi */
    app_wifi_start(portMAX_DELAY);

    return 0;
}

int HomekitDevice::Stop()
{
    hap_stop();

    esp_wifi_stop();
    return 0;
}
