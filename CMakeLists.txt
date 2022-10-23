
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS esp-homekit-sdk/components esp-homekit-sdk/components/homekit esp-homekit-sdk/examples/common)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(co2-sensor-apple-homekit)
