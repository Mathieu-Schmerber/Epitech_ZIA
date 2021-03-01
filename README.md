# Zia Project

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](http://forthebadge.com)

### Prerequisite

- Install conan and cmake, then add the following repository to your conan profile:
    - conan-center: https://bintray.com/conan/conan-center
    - epitech: https://bintray.com/epitech/public-conan
    - bincrafters: https://bintray.com/bincrafters/public-conan

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

><b>How to quit the server</b><br>
<br>
quit the server<br>
``exit``

><b>How to use the modules</b><br>
<br>
load the module "httpModule" (httpModule.so on Linux and httpModule.dll on Windows)<br>
``loadmodule httpModule``<br>
start the module "httpModule"<br>
``startmodule httpModule``<br>
stop the module "httpModule"<br>
``stopmodule httpModule``<br>
reload the module "httpModule"<br>
``reloadmodule httpModule``<br>
reload all the modules<br>
``reloadmodules``


#### Conan remotes
conan remote add <REMOTE> 
- https://api.bintray.com/conan/conan-community/conan 
- ... FIXME

## Made with

* [RapidJson](https://rapidjson.org/) - JSON parser library

## Auteurs

* **Alexandre Malaquin** _alias_ [@Monfred156](https://github.com/Monfred156)
* **Cyprien Ricque** _alias_ [@Cyprien-Ricque](https://github.com/Cyprien-Ricque)
* **Emilien Delevoye** _alias_ [@Emilien-Delevoye](https://github.com/Emilien-Delevoye)
* **Mathieu Schmerber** _alias_ [@Mathieu-Schmerber](https://github.com/Mathieu-Schmerber)
* **William Petitprez** _alias_ [@Willilafrite](https://github.com/Willilafrite)