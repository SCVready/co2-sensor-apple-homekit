#!/bin/bash

IDF_PATH=/home/esp-idf . /home/esp-idf/export.sh
cd /home/project
idf.py build
idf.py flash
idf.py monitor