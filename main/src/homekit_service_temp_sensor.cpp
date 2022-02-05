
#include "homekit_service.hpp"
#include <esp_log.h>
#include <string.h>

static int ServiceValuesCallbackStatic(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv);

template<> const char HomekitService<HomekitServiceType::TemperatureSensor>::m_log_tag[] = {"HomekitService Temperature sensor"};

template<> int HomekitService<HomekitServiceType::TemperatureSensor>::ServiceValuesCallback(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv)
{
    hap_val_t new_value;
    if (!strcmp(hap_char_get_type_uuid(hc), HAP_CHAR_UUID_NAME))
    {
        new_value.s = const_cast<char*>(m_service_name);
        hap_char_update_val(hc, &new_value);
    }
    else if (!strcmp(hap_char_get_type_uuid(hc), HAP_CHAR_UUID_STATUS_ACTIVE))
    {
        m_status_callbacks.StatusActiveCallback(new_value.b);
        hap_char_update_val(hc, &new_value);
    }
    else if (!strcmp(hap_char_get_type_uuid(hc), HAP_CHAR_UUID_STATUS_FAULT))
    {
        m_status_callbacks.StatusFaultCallback(new_value.u);
        hap_char_update_val(hc, &new_value);
    }
    else if (!strcmp(hap_char_get_type_uuid(hc), HAP_CHAR_UUID_STATUS_TAMPERED))
    {
        m_status_callbacks.StatusTamperedCallback(new_value.u);
        hap_char_update_val(hc, &new_value);
    }
    else if (!strcmp(hap_char_get_type_uuid(hc), HAP_CHAR_UUID_STATUS_LOW_BATTERY))
    {
        m_status_callbacks.StatusLowBatteryCallback(new_value.u);
        hap_char_update_val(hc, &new_value);
    }
    else if (!strcmp(hap_char_get_type_uuid(hc), HAP_CHAR_UUID_CURRENT_TEMPERATURE))
    {
        m_data_callbacks.TemperatureCallback(new_value.f);
        hap_char_update_val(hc, &new_value);
    }
    else
    {
        ESP_LOGI(m_log_tag, "Received Read for param %s", hap_char_get_type_uuid(hc));
        *status_code = HAP_STATUS_RES_ABSENT;
        return HAP_FAIL;
    }

    *status_code = HAP_STATUS_SUCCESS;
    return HAP_SUCCESS;
}

template<> HomekitService<HomekitServiceType::TemperatureSensor>::HomekitService(StatusRequestCallbacks status_callbacks, DataRequestCallbacks<HomekitServiceType::TemperatureSensor> data_callbacks) :
    m_status_callbacks(status_callbacks),
    m_data_callbacks(data_callbacks)

{
    m_service_handler = hap_serv_temperature_sensor_create(0);

    hap_serv_add_char(m_service_handler, hap_char_name_create((char*)"Temperature sensor"));
    hap_serv_set_read_cb(m_service_handler, ServiceValuesCallbackStatic);
    hap_serv_set_priv(m_service_handler, this);

    ESP_LOGI(m_log_tag, "Service created");
}

template<> HomekitService<HomekitServiceType::TemperatureSensor>::~HomekitService()
{
    ESP_LOGI(m_log_tag, "Service deleted");
}

int ServiceValuesCallbackStatic(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv)
{
    auto homekit_service_co2 = reinterpret_cast<HomekitService<HomekitServiceType::TemperatureSensor>*> (serv_priv);
    return homekit_service_co2->ServiceValuesCallback(hc, status_code, serv_priv, read_priv);
}