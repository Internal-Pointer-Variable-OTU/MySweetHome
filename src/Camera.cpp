#include "Camera.h"
#include "SecuritySystem.h"
#include <sstream>

// Base Camera implementation
Camera::Camera(const std::string &brand, const std::string &model)
    : Device(brand, model), resolution(1080), fps(30), nightVision(false),
      motionDetected(false), securitySystem(NULL)
{
}

Camera::~Camera() {}

void Camera::doPowerOn()
{
    std::cout << "  -> Camera " << name << " starting recording at " << resolution
              << "p, " << fps << " FPS" << std::endl;
}

void Camera::doPowerOff()
{
    std::cout << "  -> Camera " << name << " stopping recording." << std::endl;
}

std::string Camera::getDeviceType() const
{
    return "Camera";
}

std::string Camera::getStatus() const
{
    std::ostringstream oss;
    oss << Device::getStatus() << " | Resolution: " << resolution << "p, FPS: " << fps;
    oss << ", Night Vision: " << (nightVision ? "Yes" : "No");
    oss << ", Motion: " << (motionDetected ? "DETECTED" : "None");
    return oss.str();
}

void Camera::copyConfigurationFrom(const Device *other)
{
    Device::copyConfigurationFrom(other);
    const Camera *otherCam = dynamic_cast<const Camera *>(other);
    if (otherCam)
    {
        this->resolution = otherCam->resolution;
        this->fps = otherCam->fps;
        this->nightVision = otherCam->nightVision;
    }
}

void Camera::setResolution(int res)
{
    resolution = res;
    std::cout << "[INFO] " << name << " resolution set to: " << resolution << "p" << std::endl;
}

void Camera::setFPS(int frames)
{
    fps = frames;
    std::cout << "[INFO] " << name << " FPS set to: " << fps << std::endl;
}

void Camera::setNightVision(bool enabled)
{
    nightVision = enabled;
    std::cout << "[INFO] " << name << " night vision " << (enabled ? "enabled" : "disabled") << std::endl;
}

int Camera::getResolution() const
{
    return resolution;
}

int Camera::getFPS() const
{
    return fps;
}

bool Camera::hasNightVision() const
{
    return nightVision;
}

void Camera::setSecuritySystem(SecuritySystem *system)
{
    securitySystem = system;
}

void Camera::detectMotion()
{
    if (!powerState || !operationMode)
        return;

    motionDetected = true;
    std::cout << "[ALERT] " << name << " detected MOTION!" << std::endl;

    if (securitySystem)
    {
        // Will trigger security sequence
        // This is handled by SecuritySystem
    }
}

bool Camera::isMotionDetected() const
{
    return motionDetected;
}

void Camera::resetMotion()
{
    motionDetected = false;
}

// Samsung Camera
SamsungCamera::SamsungCamera()
    : Camera("Samsung", "SmartCam HD Pro")
{
    resolution = 1080;
    fps = 30;
    nightVision = true;
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
    resolution = 2160;
    fps = 25;
    nightVision = true;
}

XiaomiCamera::~XiaomiCamera() {}

Device *XiaomiCamera::clone() const
{
    XiaomiCamera *newCam = new XiaomiCamera();
    newCam->copyConfigurationFrom(this);
    return newCam;
}
