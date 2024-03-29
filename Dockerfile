
FROM ubuntu:18.04

RUN apt update
RUN apt install -y git wget flex bison gperf python3 python3-pip python3-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0


WORKDIR /home/
RUN git clone -b v4.4 --recurse-submodules -j8 https://github.com/espressif/esp-idf.git

RUN ./esp-idf/install.sh

COPY docker_entry.sh ./docker_entry.sh

ENTRYPOINT ["./docker_entry.sh"]
