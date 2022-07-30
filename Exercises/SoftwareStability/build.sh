#!/bin/bash

declare -x PATH=/opt/toolchains/stbgcc-4.5.4-2.6/bin:$PATH

#First clean everything
make clean

make