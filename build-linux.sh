#!/bin/bash

python3 configure.py --release
echo Generating project
python3 generate.py
