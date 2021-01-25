#!/usr/bin/sh

[ ! -d "./build" ] && mkdir build
cd build && conan install .. --build=missing && cmake -G "Unix Makefiles" .. && cmake --build . && make