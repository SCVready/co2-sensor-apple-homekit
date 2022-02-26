
FROM ubuntu:18.04

RUN apt update
RUN apt install -y git wget flex bison gperf python3 python3-pip python3-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

RUN mkdir -p ~/esp
WORKDIR /root/esp
RUN git clone -b v4.4 --recurse-submodules -j8 https://github.com/espressif/esp-idf.git
RUN git clone -b master --recurse-submodules -j8 https://github.com/SCVready/esp-homekit-sdk

WORKDIR /root/esp/esp-idf
RUN ./install.sh esp32

WORKDIR /root