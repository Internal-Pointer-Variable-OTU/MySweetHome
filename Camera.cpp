/**
 * @file Camera.cpp
 * @brief Implementation of Camera device
 * @version 3.0
 * @date 03/12/2025
 *
 * @authors
 * - 220201047: Security System - Camera motion detection logic
 *
 * @patterns Factory Method (Product)
 */

#include "Camera.h"
#include <iostream>
#include <sstream>

Camera::Camera(const std::string &brand, const std::string &model)
    : Device(brand, model), resolution(1080), isRecording(false)
{
}

Camera::~Camera() {}

std::string Camera::getDeviceType() const
{
    return "Camera";
}

std::string Camera::getStatus() const
{
    std::ostringstream oss;
    oss << Device::getStatus() << " | Res: " << resolution << "p";
    if (powerState && isRecording)
        oss << " [REC]";
    return oss.str();
}

void Camera::doPowerOn()
{
    isRecording = true;
    std::cout << "  -> Camera started recording." << std::endl;
}

void Camera::doPowerOff()
{
    isRecording = false;
    std::cout << "  -> Camera stopped recording." << std::endl;
}

void Camera::detectMotion()
{
    if (!powerState)
        return;

    std::cout << "[INFO] Camera " << name << " detected motion." << std::endl;
    // Security System trigger removed in V3.0
}

void Camera::setResolution(int res)
{
    if (res != 720 && res != 1080 && res != 2160)
    {
        std::cout << "[ERROR] Invalid resolution. Keeping " << resolution << "p" << std::endl;
        return;
    }
    resolution = res;
    std::cout << "[INFO] Camera resolution set to " << resolution << "p" << std::endl;
}

int Camera::getResolution() const
{
    return resolution;
}

// Samsung Camera
SamsungCamera::SamsungCamera()
    : Camera("Samsung", "SmartCam HD Pro")
{
}

SamsungCamera::~SamsungCamera() {}

Device *SamsungCamera::clone() const
{
    SamsungCamera *newCam = new SamsungCamera();
    newCam->copyConfigurationFrom(this);
    return newCam;
}

// Xiaomi Camera
XiaomiCamera::XiaomiCamera()
    : Camera("Xiaomi", "Mi Home Security 360")
{
}

XiaomiCamera::~XiaomiCamera() {}

Device *XiaomiCamera::clone() const
{
    XiaomiCamera *newCam = new XiaomiCamera();
    newCam->copyConfigurationFrom(this);
    return newCam;
}