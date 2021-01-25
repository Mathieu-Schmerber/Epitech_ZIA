ECHO Start installation for Windows.
if exist "./build" ECHO Folder build already exist.
if not exist "./build" mkdir build
cd build
conan install .. -s build_type=Release --build=missing
cmake ..
cmake --build .
cd ..