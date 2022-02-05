
#ifndef I2C_DEVICE_HPP_
#define I2C_DEVICE_HPP_

#include <array>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include "i2c_scanner.hpp"

class I2cDevice
{
    public:
        I2cDevice(uint32_t address);
        ~I2cDevice();
        template<int CommandSize, int MessageSize> int WriteCommand(std::array<uint8_t, CommandSize> command, std::array<uint8_t, MessageSize> data);
        template<int CommandSize, int MessageSize> int ReadCommand(std::array<uint8_t, CommandSize> command, std::array<uint8_t, MessageSize>& data);

    private:
        uint32_t m_address;
        I2cScanner& m_i2c_scanner;
};

#include "i2c_device_impl.hpp"

#endif
