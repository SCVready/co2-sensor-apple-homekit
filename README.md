# co2-sensor-apple-homekit

[![Build](https://github.com/SCVready/co2-sensor-apple-homekit/actions/workflows/Build.yml/badge.svg)](https://github.com/SCVready/co2-sensor-apple-homekit/actions/workflows/Build.yml)

Apple homekit accessory to monitor true CO2 levels.

Hardware required:
- ESP32 development board
- Sensirion SCD30 Sensor Module

## Wiring
SCD30 VIN <-----> ESP32 3V3

SCD30 GND <-----> ESP32 GND

SCD30 SCL <-----> ESP32 GPIO22 (this can be configured in main/Kconfig.projbuild)

SCD30 SDA <-----> ESP32 GPIO21 (this can be configured in main/Kconfig.projbuild)

## Build and Run using docker
1. Clone the repo

        git clone https://github.com/SCVready/co2-sensor-apple-homekit

2. Init submodules

        cd co2-sensor-apple-homekit
        git submodule update --init --recursive

3. Set the Wifi SSID and the password in the sdkconfig file:

        CONFIG_APP_WIFI_SSID={Your 2Ghz wifi SSID}
        CONFIG_APP_WIFI_PASSWORD={Wifi password}

3. Build and run

        docker run -it --mount type=bind,source="$(pwd)",target=/home/project --device=/dev/ttyUSB0 scvready/esphomekit:latest /home /home/project buildrun

    **If you have more than one USB connected to your PC, replace /dev/ttyUSB0 with the one that represents the ESP32,**

    if you see on the logs lines like the following:

        app_wifi: Connected with IP Address:...
        Co2Sensor: Measures ready
        Co2Accessory: CO2: 775.36 ppm
        Co2Accessory: Temperature: 21.00 °C
        Co2Accessory: Relative humidity: 70.39 %RH

    you have wired correctly the ESP32 and the SCD30, and the ESP32 is connected correctly to your Wifi.

    Congratulations!

    If not, you can open an issue here :) I'll try to help and improve documentation.

    Exit the docker with `docker stop {container}`, ESP32 will keep running. And next time it's booted, it´'ll load the program as well.

## Pairing

If your Iphone is on the same network as the sensor, use the **Add Accessory** option in your Home app.

You can search for your sensor by using the camera and the **QR code** that the build/run log output shows (at the very beginning) or you can just use **More options...**, the sensor should be listed there. Use the password 111-22-333.

If there is any error when trying to pair the sensor, you can **press and hold for 10 seconds the Boot** button on the ESP32, this will remove all the local data that the ESP32 is storing from previous pairings.