#ifndef CAMERA_H
#define CAMERA_H

#include "Device.h"
#include <string>

// Forward declaration
class SecuritySystem;

// Base Camera class
class Camera : public Device
{
protected:
    int resolution; // e.g., 1080, 4K = 2160
    int fps;        // frames per second
    bool nightVision;
    bool motionDetected;
    SecuritySystem *securitySystem;

public:
    Camera(const std::string &brand, const std::string &model);
    virtual ~Camera();

    virtual void doPowerOn();
    virtual void doPowerOff();
    virtual std::string getDeviceType() const;
    virtual std::string getStatus() const;
    virtual Device *clone() const = 0;
    virtual void copyConfigurationFrom(const Device *other);

    // Camera-specific methods
    void setResolution(int res);
    void setFPS(int frames);
    void setNightVision(bool enabled);
    int getResolution() const;
    int getFPS() const;
    bool hasNightVision() const;

    // Motion detection
    void setSecuritySystem(SecuritySystem *system);
    void detectMotion();
    bool isMotionDetected() const;
    void resetMotion();
};

// Concrete Camera classes - Samsung
class SamsungCamera : public Camera
{
public:
    SamsungCamera();
    virtual ~SamsungCamera();
    virtual Device *clone() const;
};

// Concrete Camera classes - Xiaomi
class XiaomiCamera : public Camera
{
public:
    XiaomiCamera();
    virtual ~XiaomiCamera();
    virtual Device *clone() const;
};

#endif // CAMERA_H
