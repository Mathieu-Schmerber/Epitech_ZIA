project(Zia)
cmake_minimum_required(VERSION 3.14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")
set(CMAKE_BUILD_TYPE Debug)

if (UNIX)
    set(CMAKE_CXX_STANDARD 17)
    add_definitions("-fPIC")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -ansi -W -Wall -Wextra -g") # FIXME remove flags for delivery.
endif()
if(WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG "/DWIN32 /D_WINDOWS /EHsc /WX /wd4355 /wd4251 /wd4250 /wd4996 -D_WIN32_WINNT=0x0601" CACHE STRING "Debug compiler flags" FORCE)
    add_compile_options("/std:c++latest")
    add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
    if(CMAKE_COMPILER_IS_GNUCXX)
        message(STATUS "GCC detected, adding compile flags")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ansi -W -Wall -Wextra -g")
    endif(CMAKE_COMPILER_IS_GNUCXX)
endif()

include(./build/conanbuildinfo.cmake)

conan_basic_setup()

set(SOURCES
        server/main.cpp

        server/Server.cpp
        server/network/tcp/TcpProtocol.cpp
        server/network/tcp/TcpSocket.cpp

        server/client/Client.cpp

        server/ConfigurationHandler.cpp

        server/modules/IModule.cpp
        server/modules/PHP_CGI.cpp
        server/core/RequestHandler/RequestHandler.cpp)

set(INCLUDES
        server/Server.hpp

        server/network/tcp/TcpProtocol.hpp
        server/network/tcp/TcpSocket.hpp
        server/network/tcp/ReceiveData.hpp

        server/client/Client.hpp

        server/tools/Utils.hpp
        server/tools/Log.hpp

        server/include/rapidjson/document.h
        server/include/rapidjson/filereadstream.h
        server/ConfigurationHandler.hpp

        server/loader/ADLLoader.hpp
        server/loader/DLLoader.hpp
        server/loader/DynamicLibManager.hpp

        server/modules/IModule.hpp
        server/modules/PHP_CGI.hpp
        server/core/RequestHandler/RequestHandler.hpp server/core/Request/IRequest.hpp server/core/Request/GetRequest.hpp server/core/Request/PostRequest.hpp server/core/Request/OptionsRequest.hpp server/core/Request/HeadRequest.hpp server/core/Request/PutRequest.hpp server/core/Request/DeleteRequest.hpp server/core/Request/TraceRequest.hpp server/core/Request/ConnectRequest.hpp server/core/Response/IResponse.hpp server/core/RequestParser/RequestParser.cpp server/core/RequestParser/RequestParser.hpp server/core/ExceptionCore/ExceptionCore.hpp server/exception/ZiaError.hpp)

add_executable(server ${SOURCES} ${INCLUDES})

include(GNUInstallDirs)

target_include_directories(server PRIVATE
        server
        server/tools
        server/network
        server/network/tcp
        server/client
        server/include/rapidjson
        server/exception
        server/core/RequestHandler
        server/core/Request
        server/core/RequestParser
        )

message("===========================================")
message(${CONAN_LIBS})
message("===========================================")

target_link_libraries(server ${CONAN_LIBS})

if(MSVC)
    set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
    set(Boost_USE_DEBUG_LIBS         OFF) # ignore debug libs and
    set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
    set(Boost_USE_MULTITHREADED      ON)
    set(Boost_USE_STATIC_RUNTIME     OFF)
    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
        target_link_libraries(server ${Boost_LIBRARIES})
    endif()
endif()