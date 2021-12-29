/**
 * @author Alejandro Solozabal
 *
 */

#include "i2c_device.hpp"

template<int CommandSize, int MessageSize> int I2cDevice::WriteCommand(std::array<uint8_t, CommandSize> command, std::array<uint8_t, MessageSize> data)
{
    std::array<uint8_t, CommandSize + MessageSize> message;

    for (int i = 0; i < CommandSize; i++)
    {
        message[i] = command[i];
    }

    for (int i = 0; i < MessageSize; i++)
    {
        message[i + CommandSize] = data[i];
    }

    vTaskDelay(10 / portTICK_RATE_MS);

    return m_i2c_scanner.WriteData<CommandSize + MessageSize>(m_address, message);
}

template<int CommandSize, int MessageSize> int I2cDevice::ReadCommand(std::array<uint8_t, CommandSize> command, std::array<uint8_t, MessageSize>& data)
{
    int ret_val = -1;

    vTaskDelay(10 / portTICK_RATE_MS);

    if (0 == m_i2c_scanner.WriteData<CommandSize>(m_address, command))
    {
        vTaskDelay(10 / portTICK_RATE_MS);

        ret_val = m_i2c_scanner.ReadData<MessageSize>(m_address, data);
    }

    return ret_val;
}
