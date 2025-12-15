/**
 * @file Camera.h
 * @brief Concrete Camera device implementation
 * @version 3.0
 * @date 03/12/2025
 *
 * @authors
 * - 220201047: Security System - Camera motion detection logic
 *
 * @patterns Factory Method (Product)
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "Device.h"

class Camera : public Device
{
protected:
    int resolution; // 720, 1080, 2160 (4K)
    bool isRecording;

public:
    Camera(const std::string &brand, const std::string &model);
    virtual ~Camera();

    virtual std::string getDeviceType() const;
    virtual std::string getStatus() const;

    // Override template methods
    virtual void doPowerOn();
    virtual void doPowerOff();

    virtual Device *clone() const = 0;

    void detectMotion(); // Used to trigger security, now independent
    void setResolution(int res);
    int getResolution() const;
};

// Concrete Camera - Samsung
class SamsungCamera : public Camera
{
public:
    SamsungCamera();
    virtual ~SamsungCamera();
    virtual Device *clone() const;
};

// Concrete Camera - Xiaomi
class XiaomiCamera : public Camera
{
public:
    XiaomiCamera();
    virtual ~XiaomiCamera();
    virtual Device *clone() const;
};

#endif // CAMERA_H