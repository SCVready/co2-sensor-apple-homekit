/**
 * @author Alejandro Solozabal
 *
 */

#include "driver/i2c.h"

#include "i2c_scanner.hpp"

template<int data_size> int I2cScanner::WriteData(uint8_t address, std::array<uint8_t, data_size> data)
{
    esp_err_t retval = ESP_FAIL;
    esp_err_t esp_retval = ESP_FAIL;
    i2c_cmd_handle_t command_handle = NULL;

    if (NULL == (command_handle = i2c_cmd_link_create()))
    {
        ESP_LOGE(m_log_tag, "%s(). ERROR: i2c_cmd_link_create() failed", __FUNCTION__);
    }
    else if(ESP_OK != (esp_retval = i2c_master_start(command_handle)))
    {
        ESP_LOGE(m_log_tag, "%s(). ERROR: i2c_master_start(). Error %i (%s)", __FUNCTION__, esp_retval, esp_err_to_name(esp_retval));
    }
    else if(ESP_OK != (esp_retval = i2c_master_write_byte(command_handle, (address << 1)  | I2C_MASTER_WRITE, true)))
    {
        ESP_LOGE(m_log_tag, "%s(). ERROR: i2c_master_write_byte(). Error %i (%s)", __FUNCTION__, esp_retval, esp_err_to_name(esp_retval));
    }
    else if(ESP_OK != (esp_retval = i2c_master_write(command_handle, data.data(), data_size, true)))
    {
        ESP_LOGE(m_log_tag, "%s(). ERROR: i2c_master_write(). Error %i (%s)", __FUNCTION__, esp_retval, esp_err_to_name(esp_retval));
    }
    else if(ESP_OK != (esp_retval = i2c_master_stop(command_handle)))
    {
        ESP_LOGE(m_log_tag, "%s(). ERROR: i2c_master_stop(). Error %i (%s)", __FUNCTION__, esp_retval, esp_err_to_name(esp_retval));
    }
    else if(ESP_OK != (esp_retval = i2c_master_cmd_begin(m_config.master_num, command_handle, 1000/portTICK_PERIOD_MS)))
    {
        ESP_LOGE(m_log_tag, "%s(). ERROR: i2c_master_cmd_begin(). Error %i (%s)", __FUNCTION__, esp_retval, esp_err_to_name(esp_retval));
    }
    else
    {
        retval = ESP_OK;
    }

    i2c_cmd_link_delete(command_handle);

    return retval;
}

template<int data_size> int I2cScanner::ReadData(uint8_t address, std::array<uint8_t, data_size>& data)
{
    esp_err_t retval = ESP_FAIL;
    esp_err_t esp_retval = ESP_FAIL;
    i2c_cmd_handle_t command_handle = NULL;

    if (NULL == (command_handle = i2c_cmd_link_create()))
    {
        ESP_LOGE(m_log_tag, "%s(). ERROR: i2c_cmd_link_create() failed", __FUNCTION__);
    }
    else if(ESP_OK != (esp_retval = i2c_master_start(command_handle)))
    {
        ESP_LOGE(m_log_tag, "%s(). ERROR: i2c_master_start(). Error %i (%s)", __FUNCTION__, esp_retval, esp_err_to_name(esp_retval));
    }
    else if(ESP_OK != (esp_retval = i2c_master_write_byte(command_handle, (address << 1)  | I2C_MASTER_READ, true)))
    {
        ESP_LOGE(m_log_tag, "%s(). ERROR: i2c_master_write_byte(). Error %i (%s)", __FUNCTION__, esp_retval, esp_err_to_name(esp_retval));
    }
    else if(ESP_OK != (esp_retval = i2c_master_read(command_handle, data.data(), data_size, I2C_MASTER_ACK)))
    {
        ESP_LOGE(m_log_tag, "%s(). ERROR: i2c_master_read(). Error %i (%s)", __FUNCTION__, esp_retval, esp_err_to_name(esp_retval));
    }
    else if(ESP_OK != (esp_retval = i2c_master_stop(command_handle)))
    {
        ESP_LOGE(m_log_tag, "%s(). ERROR: i2c_master_stop(). Error %i (%s)", __FUNCTION__, esp_retval, esp_err_to_name(esp_retval));
    }
    else if(ESP_OK != (esp_retval = i2c_master_cmd_begin(m_config.master_num, command_handle, 1000/portTICK_PERIOD_MS)))
    {
        //ESP_LOGE(m_log_tag, "%s(). ERROR: i2c_master_cmd_begin(). Error %i (%s)", __FUNCTION__, esp_retval, esp_err_to_name(esp_retval));
        retval = ESP_OK;
    }
    else
    {
        retval = ESP_OK;
    }

    i2c_cmd_link_delete(command_handle);

    return retval;
}
