/**
 * @author Alejandro Solozabal
 *
 */

#ifndef CO2_SENSOR_HPP_
#define CO2_SENSOR_HPP_

#include <functional>

#include "i2c_device.hpp"


static const int SCD30_i2c_address = 0x61;

static const int TaskPriority = 1;
static const int TaskStackSize = 4 * 1024;

struct Co2SensorConfig
{
    int a;
};

class Co2Sensor : public I2cDevice 
{
    public:
        Co2Sensor(Co2SensorConfig config, std::function<void(float, float, float)> callback);
        ~Co2Sensor();
        int Start();
        int Stop();
        void ExecutionCycle();
    private:
        int SetSelfCalibrationStatus(bool enabled);
        int GetSelfCalibrationStatus(bool& enabled);
        int SetTemperatureOffset(float temperature);
        int GetTemperatureOffset(float& temperature);
        int SetAltitudeCompensation(uint16_t altitude);
        int GetAltitudeCompensation(uint16_t& altitude);
        int GetReadyStatus(bool& ready);
        int ReadMeasurements(float& co2_concentration, float& temperature, float& relative_humidity);
        int SoftReset();
        float ConvertMeasureData(std::array<uint8_t, 4> data);
        uint8_t ComputeCRC(std::array<uint8_t, 2> data);

        volatile bool m_running = false;
        Co2SensorConfig m_config;
        static const char m_log_tag[];
        std::function<void(float, float, float)> m_callback;
        TaskHandle_t m_task_handle;
};

#endif
