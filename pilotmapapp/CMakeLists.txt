cmake_minimum_required(VERSION 3.5)

project(pilotmapapp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH "bin")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform Options

## QT Options
set(QT_INSTALL_PATH "/usr/local/Homebrew/Cellar/qt@5/5.15.2_2")
#set(QT_INSTALL_PATH "O:/Qt/5.15.2/msvc2019_64")
#set(QT_INSTALL_PATH "/opt/Qt/5.15.2/")
set(CMAKE_PREFIX_PATH "${QT_INSTALL_PATH}")  # <--- MAKE SURE HOMEBREW INSTALLATION FOR QT IS x86_64 (arm is not supported for dependencies)

## ArcGIS Options
set(ArcGISRuntime_INSTALL_DIR "/Users/richy/ArcGIS_SDKs/Qt100.7") # <--- Location of ArcGIS SDK ([location]/ArcGIS_SDKs/Qt100.13/)
#set(ArcGISRuntime_INSTALL_DIR "/opt/arcgis")
#set(ArcGISRuntime_INSTALL_DIR "O:/ArcGIS_Runtime/SDKS/qt100.7")
set(ArcGISRuntime_DIR "${ArcGISRuntime_INSTALL_DIR}/sdk/ideintegration")

set(ArcGIS_PLATFORM_NAME "macOS") # options are: android, iOS, macOS
#set(ArcGIS_PLATFORM_NAME "windows")
include_directories("${ArcGISRuntime_INSTALL_DIR}/sdk/include") # ArcGIS SDK include files

## ZMQ Options
set(ZMQ_VERSION "4.3.4")
set(ZMQ_INSTALL_PATH "/usr/local/Homebrew/Cellar/zeromq/${ZMQ_VERSION}/")
#set(ZMQ_INSTALL_PATH "O:/ZeroMQ/${ZMQ_VERSION}/")
include_directories("${ZMQ_INSTALL_PATH}/include")
link_directories("${ZMQ_INSTALL_PATH}/lib")

## Boost Options
set(BOOST_INSTALL_PATH "/usr/local/Homebrew/Cellar/boost/1.78.0_1") # before the "boost" (mind the lowercase) folder
#set(BOOST_INSTALL_PATH "O:/Boost/boost_1_79_0")
include_directories("${BOOST_INSTALL_PATH}/include")
set(Boost_INCLUDE_DIR "${BOOST_INSTALL_PATH}/include")

## MSGPACK Options (Windows only)
#set(MSGPACK_INSTALL_PATH "O:/MSGPACK/")
#include_directories("${MSGPACK_INSTALL_PATH}/include")
#set(RUNTIME_CMAKE_PATH "${MSGPACK_INSTALL_DIR}/lib/cmake/msgpackc-cxx/msgpack-config.cmake")

## Set CMAKE cmake script dir

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR})

## Set CMAKE Arch to x86_64 or x64

#set(CMAKE_GENERATOR_PLATFORM x64)
#set(CMAKE_SYSTEM_PROCESSOR x64)

set(CMAKE_GENERATOR_PLATFORM x86_64)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_OSX_ARCHITECTURES x86_64)

message("Binary DIR = ${CMAKE_BINARY_DIR}")
message("Source DIR = ${CMAKE_SOURCE_DIR}")

###

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Multimedia REQUIRED)

find_package(ArcGISRuntime 100.7 COMPONENTS Cpp REQUIRED)

find_package(msgpack REQUIRED)

set(PROJECT_SOURCES
        resources.qrc
        main.cpp
        mainwindow.cpp
        mainwindow.h
        Widgets/mapWidget.cpp
        Widgets/mapWidget.h
        Backend/Utilities/utilities.cpp
        Backend/Utilities/utilities.h
        Backend/Communication/communicationManager.cpp
        Backend/Communication/communicationManager.h
        Backend/Communication/communicationThread.cpp
        Backend/Communication/communicationThread.h
        Backend/Communication/dataManager.cpp
        Backend/Communication/dataManager.h
        Backend/boatKernel.cpp
        Backend/boatKernel.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pilotmapapp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET pilotmapapp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(pilotmapapp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(pilotmapapp
            ${PROJECT_SOURCES}
        )
    endif()

# Copy required ArcGIS dynamic libraries to the build folder as a post-build step.
message("Copy ArcGIS Runtime libs")
add_custom_command(TARGET pilotmapapp POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${ArcGISRuntime_LIBRARIES}
  "${PROJECT_BINARY_DIR}/bin")


# ArcGISRuntime::Cpp
target_link_libraries(pilotmapapp PRIVATE 
  Qt${QT_VERSION_MAJOR}::Widgets 
  Qt${QT_VERSION_MAJOR}::Multimedia
  ArcGISRuntime::Cpp
  zmq
  msgpackc-cxx
  )
endif()

target_link_libraries(pilotmapapp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(pilotmapapp PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pilotmapapp)
endif()

