#!/bin/bash

IDF_PATH=$1/esp-idf . $1/esp-idf/export.sh
cd $2

if [[ "$3" == "buildrun" ]]; then
    idf.py build
    idf.py flash
    idf.py monitor
elif [[ "$3" == "run" ]]; then
    idf.py flash
    idf.py monitor
else
    idf.py build
fi
