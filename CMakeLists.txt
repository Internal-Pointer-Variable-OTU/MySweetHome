cmake_minimum_required(VERSION 3.10)
project(MySweetHome VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/Menu.cpp
    src/Storage.cpp
    src/Device.cpp
    src/Light.cpp
    src/Camera.cpp
    src/Television.cpp
    src/Detector.cpp
    src/SmokeDetector.cpp
    src/GasDetector.cpp
    src/Alarm.cpp
    src/SoundSystem.cpp
    src/DeviceFactory.cpp
    src/ModeManager.cpp
    src/StateManager.cpp
    src/SecuritySystem.cpp
    src/DetectionSystem.cpp
    src/NotificationSystem.cpp
    src/HomeController.cpp
    src/LightHandler.cpp
    src/PoliceHandler.cpp
    src/SecurityHandler.cpp
    src/AlarmHandler.cpp
)

# Create library
add_library(msh_lib ${SOURCES})

# Create executable
add_executable(msh src/main.cpp)
target_link_libraries(msh msh_lib)

# Output directory
set_target_properties(msh PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Testing
enable_testing()
add_subdirectory(tests)
