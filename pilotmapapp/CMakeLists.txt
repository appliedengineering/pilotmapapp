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
# set(QT_INSTALL_PATH "O:\\Qt\\5.15.2\\msvc2019_64")
#set(QT_INSTALL_PATH "/opt/Qt/5.15.2/")
set(CMAKE_PREFIX_PATH "${QT_INSTALL_PATH}")  # <--- MAKE SURE HOMEBREW INSTALLATION FOR QT IS x86_64 (arm is not supported for dependencies)

## Set CMAKE cmake script dir

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR})

## Set CMAKE Arch to x86_64

set(CMAKE_GENERATOR_PLATFORM x86_64)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_OSX_ARCHITECTURES x86_64)

message("Binary DIR = ${CMAKE_BINARY_DIR}")
message("Source DIR = ${CMAKE_SOURCE_DIR}")

###

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
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
