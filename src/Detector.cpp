#include "Detector.h"
#include <sstream>

Detector::Detector(const std::string& brand, const std::string& model)
    : Device(brand, model), detected(false), sensitivityLevel(5), detectionSystem(NULL) {
    // Detectors start powered on by default - critical devices
    powerState = true;
}

Detector::~Detector() {}

void Detector::powerOff() {
    std::cout << "[WARNING] " << name << " is a CRITICAL device and cannot be powered off!" << std::endl;
    // Do not change powerState - keep it on
}

void Detector::doPowerOn() {
    std::cout << "  -> Detector " << name << " monitoring activated." << std::endl;
}

void Detector::doPowerOff() {
    // This should never be called for detectors
    std::cout << "  -> Detector " << name << " cannot be turned off (critical device)." << std::endl;
}

std::string Detector::getStatus() const {
    std::ostringstream oss;
    oss << Device::getStatus() << " | Sensitivity: " << sensitivityLevel << "/10";
    oss << ", Detection: " << (detected ? "ALERT!" : "Clear");
    return oss.str();
}

void Detector::copyConfigurationFrom(const Device* other) {
    Device::copyConfigurationFrom(other);
    const Detector* otherDet = dynamic_cast<const Detector*>(other);
    if (otherDet) {
        this->sensitivityLevel = otherDet->sensitivityLevel;
    }
}

void Detector::setSensitivity(int level) {
    if (level < 1) level = 1;
    if (level > 10) level = 10;
    sensitivityLevel = level;
    std::cout << "[INFO] " << name << " sensitivity set to: " << sensitivityLevel << "/10" << std::endl;
}

int Detector::getSensitivity() const {
    return sensitivityLevel;
}

bool Detector::isDetected() const {
    return detected;
}

void Detector::resetDetection() {
    detected = false;
    std::cout << "[INFO] " << name << " detection reset." << std::endl;
}

void Detector::setDetectionSystem(DetectionSystem* system) {
    detectionSystem = system;
}
