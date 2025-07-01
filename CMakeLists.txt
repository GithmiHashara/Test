cmake_minimum_required(VERSION 3.16)
project(IMUHealthMonitor)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable Qt MOC, RCC, and UIC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set source files
set(SOURCES
    main.cpp
    imuhealthmonitor.cpp
    toolbarmanager.cpp
)

set(HEADERS
    imuhealthmonitor.h
    toolbarmanager.h
)

# Create executable
add_executable(IMUHealthMonitor ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(IMUHealthMonitor Qt6::Core Qt6::Widgets)

# Set target properties
set_target_properties(IMUHealthMonitor PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)