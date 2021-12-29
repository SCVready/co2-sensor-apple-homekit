/**
 * @author Alejandro Solozabal
 *
 */

#include "i2c_device.hpp"

I2cDevice::I2cDevice(uint32_t address) : m_address(address), m_i2c_scanner(I2cScanner::GetInstance())
{
}

I2cDevice::~I2cDevice()
{
}
