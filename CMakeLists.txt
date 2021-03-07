project(Zia)
cmake_minimum_required(VERSION 3.14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")
set(CMAKE_BUILD_TYPE Debug)

#Documentation
option(BUILD_DOC "Build documentation" ON)

find_package(Doxygen)

if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/build/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    #add_custom_target(doc_doxygen ALL
    #        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    #        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    #        COMMENT "Generating API documentation with Doxygen"
    #        VERBATIM)
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

#Project build
if (UNIX)
    set(CMAKE_CXX_STANDARD 20)
    add_definitions("-fPIC")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -ansi -ldl -W -Wall -Wextra -g -Wno-unused") # FIXME remove flags for delivery.
endif()
if (WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG "/DWIN32 /MD /Z7 /D_WINDOWS /EHsc /WX /wd4355 /wd4251 /wd4250 /wd4996" CACHE STRING "Debug compiler flags" FORCE)
    add_compile_options("/std:c++latest")
    add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
    if(CMAKE_COMPILER_IS_GNUCXX)
        message(STATUS "GCC detected, adding compile flags")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ansi -W -Wall -Wextra -g") # FIXME -g
    endif(CMAKE_COMPILER_IS_GNUCXX)
endif()

include(./build/conanbuildinfo.cmake)

conan_basic_setup()

set(SOURCES
        server/main.cpp

        server/Server.cpp
        modules/HTTP/network/tcp/TcpProtocol.cpp
        modules/HTTP/network/tcp/TcpSocket.cpp

        server/client/Client.cpp

        server/ConfigurationHandler.cpp

        server/core/RequestHandler/RequestHandler.cpp
        server/core/RequestParser/RequestParser.cpp
        server/Server.hpp

        modules/HTTP/network/tcp/TcpProtocol.hpp
        modules/HTTP/network/tcp/TcpSocket.hpp
        modules/HTTP/network/tcp/ReceiveData.hpp

        server/client/Client.hpp

        server/tools/Utils.hpp
        server/tools/Log.hpp
        server/tools/Logger.hpp

        server/include/rapidjson/document.h
        server/include/rapidjson/filereadstream.h
        server/ConfigurationHandler.hpp

        server/loader/ADLLoader.hpp
        server/loader/DLLoader.hpp
        server/loader/DynamicLibManager.hpp

        server/modules/IModule.hpp
        server/core/RequestHandler/RequestHandler.hpp
        server/core/Request/IRequest.hpp
        server/core/Request/GetRequest.hpp
        server/core/Request/PostRequest.hpp
        server/core/Request/OptionsRequest.hpp
        server/core/Request/HeadRequest.hpp
        server/core/Request/PutRequest.hpp
        server/core/Request/DeleteRequest.hpp
        server/core/Request/TraceRequest.hpp
        server/core/Request/ConnectRequest.hpp
        server/core/RequestParser/RequestParser.hpp
        server/core/ExceptionCore/ExceptionCore.hpp
        server/exception/ZiaError.hpp
        server/loader/ExceptionsDLLoader.hpp
        server/modules/AModule.cpp
        server/modules/AModule.hpp
        server/modules/ModuleHandler.cpp
        server/modules/ModuleHandler.hpp
        server/modules/ModuleException.hpp
        server/ServerException.hpp
        server/core/ExceptionCore/ZiaHTTPError.hpp
        server/routing/Router/Router.cpp
        server/routing/Router/Router.hpp
        server/core/Response/Response.cpp
        server/core/Response/Response.hpp)

set(SOURCES_HTTP_MODULE
        modules/HTTP/HttpModule.cpp
        modules/HTTP/HttpModule.hpp
        server/modules/IModule.hpp
        server/modules/AModule.cpp
        server/modules/AModule.hpp
        server/ConfigurationHandler.cpp
        server/ConfigurationHandler.hpp
        modules/HTTP/network/tcp/TcpProtocol.hpp
        modules/HTTP/network/tcp/TcpProtocol.cpp
        modules/HTTP/network/tcp/TcpSocket.hpp
        modules/HTTP/network/tcp/TcpSocket.cpp
        modules/HTTP/network/tcp/ReceiveData.hpp
        server/core/RequestParser/RequestParser.hpp
        server/core/RequestParser/RequestParser.cpp)

set(SOURCES_HTTPS_MODULE
        modules/HTTPS/HttpsModule.cpp
        modules/HTTPS/HttpsModule.hpp
        server/modules/IModule.hpp
        server/modules/AModule.cpp
        server/modules/AModule.hpp
        server/ConfigurationHandler.cpp
        server/ConfigurationHandler.hpp
        modules/HTTPS/network/tcp/TcpProtocol.hpp
        modules/HTTPS/network/tcp/TcpProtocol.cpp
        modules/HTTPS/network/tcp/TcpSocket.hpp
        modules/HTTPS/network/tcp/TcpSocket.cpp
        modules/HTTPS/network/tcp/ReceiveData.hpp
        modules/HTTPS/cryption/Cryption.cpp
        modules/HTTPS/cryption/Cryption.hpp
        server/core/RequestParser/RequestParser.hpp
        server/core/RequestParser/RequestParser.cpp)

set(SOURCES_PHP_MODULE
        modules/PHP/PHP_CGI.cpp
        modules/PHP/PHP_CGI.hpp
        server/modules/IModule.hpp
        server/modules/AModule.cpp
        server/modules/AModule.hpp
        server/core/Response/Response.hpp
        server/core/Response/Response.cpp
        server/core/RequestParser/RequestParser.hpp
        server/core/RequestParser/RequestParser.cpp
        server/routing/Router/Router.cpp
        server/routing/Router/Router.hpp
        )

set(RESOURCES_PHP_MODULE
        modules/PHP/php_rsc)

add_executable(server ${SOURCES})

include(GNUInstallDirs)

target_include_directories(server PRIVATE
        server
        server/tools
        server/modules
        modules/HTTP/network
        modules/HTTP/network/tcp
        modules/PHP
        server/client
        server/include/rapidjson
        server/exception
        server/core/RequestHandler
        server/core/Request
        server/core/RequestParser
        server/core/ExceptionCore
        server/routing/Router
        server/loader
        server/core/Response
        )

add_library(httpModule SHARED
        ${SOURCES_HTTP_MODULE})

add_library(httpsModule SHARED
        ${SOURCES_HTTPS_MODULE})

add_library(phpModule SHARED
        ${SOURCES_PHP_MODULE})

target_include_directories(httpModule PRIVATE
        server/modules
        server
        server/include/rapidjson
        server/tools
        modules/HTTP/network/tcp
        server/core/RequestParser
        server/exception
        server/core/ExceptionCore)

target_include_directories(httpsModule PRIVATE
        server/modules
        server
        server/include/rapidjson
        server/tools
        modules/HTTPS/network/tcp
        server/core/RequestParser
        server/exception
        server/core/ExceptionCore)

target_include_directories(phpModule PRIVATE
        server/modules
        server/tools
        server/core/RequestParser
        server/exception
        server/core/Response
        server/routing/Router
        server/core/ExceptionCore)

message("===========================================")
message(${CONAN_LIBS})
message("===========================================")

target_link_libraries(server ${CONAN_LIBS})
target_link_libraries(httpsModule ${CONAN_LIBS})
target_link_libraries(phpModule ${CONAN_LIBS})
if (WIN32)
    set_target_properties(httpsModule PROPERTIES LINK_FLAGS "/ignore:4099")
    set_target_properties(server PROPERTIES LINK_FLAGS "/ignore:4099")
endif()

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

if (UNIX)
    # copy lib dir to bin dir
    message("Copying lib ${CMAKE_BINARY_DIR}/lib to ${CMAKE_BINARY_DIR}/bin")
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    file(COPY ${CMAKE_BINARY_DIR}/lib DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()
# copy php module sources to bin dir
message("Copying php resources ${RESOURCES_PHP_MODULE} to ${CMAKE_BINARY_DIR}/bin")
file(COPY ${RESOURCES_PHP_MODULE} DESTINATION ${CMAKE_BINARY_DIR}/bin)