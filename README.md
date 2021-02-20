# Zia Project


## Build

### Cross Platform

><b>make</b><br>
compile the server

> <b>make run</b><br>
compile the server and run the binary

><b>make clean</b><br>
remove build directory

><b>make fclean</b><br>
run clean rule and remove conan packages

### Linux

> <b>Compile by using the script</b><br>
``./install_unix.sh``

> <b>Compile by yourself</b><br>
`` mkdir build && cd build && conan install .. --build=missing && cmake .. -G “Unix Makefiles” && cmake --build .``

### Windows

> <b>Compile by using the script</b><br>
``.\install_windows.bat``

> <b>Compile by yourself</b><br>
``mkdir build && cd build && conan install .. --build=missing && cmake .. -G "Visual Studio 16 2019" && cmake --build .``

> <b>Install Make on windows</b><br>
``choco install make``

### How to use the server

><b>How to use the modules</b><br>
load the module "httpModule" (httpModule.so on Linux and httpModule.dll on Windows)<br>
``loadmodule httpModule``<br>
start the module "httpModule"<br>
``startmodule httpModule``<br>
stop the module "httpModule"<br>
``stopmodule httpModule``<br>
