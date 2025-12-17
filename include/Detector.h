#ifndef DETECTOR_H
#define DETECTOR_H
	




#include "Device.h"
#include <string>

// Forward declaration
class DetectionSystem;

// Base Detector class - Critical device that cannot be powered off
class Detector : public Device {
protected:
    bool detected;           // Something detected
    int sensitivityLevel;    // 1-10
    DetectionSystem* detectionSystem;

public:
    Detector(const std::string& brand, const std::string& model);
    virtual ~Detector();

    // Override powerOff to prevent turning off critical device
    void powerOff();  // Hide base class version - detectors cannot be powered off

    virtual void doPowerOn();
    virtual void doPowerOff();
    virtual std::string getDeviceType() const = 0;
    virtual std::string getStatus() const;
    virtual Device* clone() const = 0;
    virtual void copyConfigurationFrom(const Device* other);

    // Detector-specific methods
    void setSensitivity(int level);
    int getSensitivity() const;
    bool isDetected() const;
    void resetDetection();
    
    void setDetectionSystem(DetectionSystem* system);
    virtual void detect() = 0;  // Template for different detection types
};

#endif // DETECTOR_H
