/**
 * @file SmokeDetector.cpp
 * @brief Implementation of Smoke Detector
 * @version 5.0
 * @date 03/12/2025
 * 
 * @authors
 * - 220208041: Detectors & Alarm - Smoke Detector implementation
 * 
 * @patterns Factory Method (Product)
 */

#include "SmokeDetector.h"
#include <sstream>

SmokeDetector::SmokeDetector(const std::string& brand, const std::string& model)
    : Detector(brand, model), smokeLevel(0) {
}

SmokeDetector::~SmokeDetector() {}

std::string SmokeDetector::getDeviceType() const {
    return "Smoke Detector";
}

std::string SmokeDetector::getStatus() const {
    std::ostringstream oss;
    oss << Detector::getStatus() << " | Smoke Level: " << smokeLevel << "%";
    return oss.str();
}

void SmokeDetector::detect() {
    if (!powerState || !operationMode) return;
    
    if (smokeLevel > (10 - sensitivityLevel) * 10) {  // Higher sensitivity = lower threshold
        detected = true;
        std::cout << "[ALERT] " << name << " detected SMOKE! Level: " << smokeLevel << "%" << std::endl;
    }
}

void SmokeDetector::trigger() {
    detect();
}

int SmokeDetector::getSmokeLevel() const {
    return smokeLevel;
}

void SmokeDetector::setSmokeLevel(int level) {
    if (level < 0) level = 0;
    if (level > 100) level = 100;
    smokeLevel = level;
    detect();  // Auto-check after level change
}

// Nest Smoke Detector
NestSmokeDetector::NestSmokeDetector()
    : SmokeDetector("Nest", "Protect 2nd Gen") {
    sensitivityLevel = 7;
}

NestSmokeDetector::~NestSmokeDetector() {}

Device* NestSmokeDetector::clone() const {
    NestSmokeDetector* newDet = new NestSmokeDetector();
    newDet->copyConfigurationFrom(this);
    return newDet;
}

// First Alert Smoke Detector
FirstAlertSmokeDetector::FirstAlertSmokeDetector()
    : SmokeDetector("First Alert", "Onelink Safe & Sound") {
    sensitivityLevel = 6;
}

FirstAlertSmokeDetector::~FirstAlertSmokeDetector() {}

Device* FirstAlertSmokeDetector::clone() const {
    FirstAlertSmokeDetector* newDet = new FirstAlertSmokeDetector();
    newDet->copyConfigurationFrom(this);
    return newDet;
}
