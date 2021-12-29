/**
 * @author Alejandro Solozabal
 *
 */

#include "driver/i2c.h"

#include "i2c_scanner.hpp"

const char I2cScanner::m_log_tag[] = {"I2cScanner"};

const I2cScannerConfig I2cScanner::m_default_config = {
    .master_num            = CONFIG_MY_SCANNER_I2C_MASTER_NUM,
    .sclk_gpio_num         = CONFIG_MY_SCANNER_I2C_SCLK_GPIO_NUM,
    .sda_gpio_num          = CONFIG_MY_SCANNER_I2C_SDA_GPIO_NUM,
    .master_freq_hz        = 50 * 1000,
    .slave_timeout_maxval  = 1048575
};

I2cScannerConfig I2cScanner::m_config;

I2cScanner& I2cScanner::GetInstance()
{
    static I2cScanner i2c_bus;

    return i2c_bus;
}

I2cScanner::I2cScanner()
{
    ESP_LOGI(m_log_tag, "I2cScanner created");
    if (0 != Configure(m_default_config))
    {
        abort();
    }
}

I2cScanner::~I2cScanner()
{
    ESP_LOGI(m_log_tag, "I2cScanner destroyed");
}

int I2cScanner::Configure(const I2cScannerConfig& config)
{
    int ret_val = -1;
    esp_err_t f_retval = ESP_FAIL;

    ESP_LOGI(m_log_tag, "Configuring I2C Scanner...");
    ESP_LOGI(m_log_tag, "I2C Scanner num:           %u", config.master_num);
    ESP_LOGI(m_log_tag, "I2C Scanner SCLK GPio num: %u", config.sclk_gpio_num);
    ESP_LOGI(m_log_tag, "I2C Scanner SDA GPio num:  %u", config.sda_gpio_num);
    ESP_LOGI(m_log_tag, "I2C Scanner Freq Hz:       %u", config.master_freq_hz);
    ESP_LOGI(m_log_tag, "I2C Scanner Slave Timeout: %u", config.slave_timeout_maxval);

    m_config = config;

    i2c_config_t i2c_configuration = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = config.sda_gpio_num,
        .scl_io_num = config.sclk_gpio_num,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
            .clk_speed = config.master_freq_hz
        },
        .clk_flags = {}
    };

    if (ESP_OK != (f_retval = i2c_param_config(config.master_num, &i2c_configuration)))
    {
        ESP_LOGE(m_log_tag, "%s(). ABORT. i2c_param_config() | err %i (%s)", __FUNCTION__, f_retval, esp_err_to_name(f_retval));
    }
    else if (ESP_OK != (f_retval = i2c_set_timeout(config.master_num, config.slave_timeout_maxval)))
    {
        ESP_LOGE(m_log_tag, "%s(). ABORT. i2c_set_timeout() | err %i (%s)", __FUNCTION__, f_retval, esp_err_to_name(f_retval));
    }
    else if (ESP_OK != (f_retval = i2c_driver_install(config.master_num, i2c_configuration.mode, 0, 0, 0)))
    {
        ESP_LOGE(m_log_tag, "%s(). ABORT. i2c_driver_install() | err %i (%s)", __FUNCTION__, f_retval, esp_err_to_name(f_retval));
    }
    else
    {
        ESP_LOGI(m_log_tag, "I2C Scanner configured");
        ret_val = 0;
    }

    return ret_val;
}
