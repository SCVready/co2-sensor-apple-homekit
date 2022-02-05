
#ifndef I2C_SCANNER_HPP_
#define I2C_SCANNER_HPP_

#include <memory>
#include <esp_log.h>
#include <array>

struct I2cScannerConfig
{
    uint32_t master_num;
    int32_t sclk_gpio_num;
    int32_t sda_gpio_num;
    uint32_t master_freq_hz;
    uint32_t slave_timeout_maxval;
};

class I2cScanner
{
    public:
        static I2cScanner& GetInstance();
        static int Configure(const I2cScannerConfig& config);
        template<int data_size> int WriteData(uint8_t address, std::array<uint8_t, data_size> data);
        template<int data_size> int ReadData(uint8_t address, std::array<uint8_t, data_size>& data);

    private:
        I2cScanner();
        ~I2cScanner();

        static const I2cScannerConfig m_default_config;
        static I2cScannerConfig m_config;
        static const char m_log_tag[];
};

#include "i2c_scanner_impl.hpp"

#endif
