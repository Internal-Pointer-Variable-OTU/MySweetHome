#include "GasDetector.h"
#include "DetectionSystem.h"
#include <sstream>

GasDetector::GasDetector(const std::string& brand, const std::string& model)
    : Detector(brand, model), gasLevel(0), gasType("CO/Natural Gas") {
}

GasDetector::~GasDetector() {}

std::string GasDetector::getDeviceType() const {
    return "Gas Detector";
}

std::string GasDetector::getStatus() const {
    std::ostringstream oss;
    oss << Detector::getStatus() << " | Gas Level: " << gasLevel << "% (" << gasType << ")";
    return oss.str();
}

void GasDetector::detect() {
    if (!powerState || !operationMode) return;
    
    if (gasLevel > (10 - sensitivityLevel) * 10) {
        detected = true;
        std::cout << "[ALERT] " << name << " detected GAS! Level: " << gasLevel 
                  << "% (" << gasType << ")" << std::endl;
        
        if (detectionSystem) {
            // Will trigger detection sequence - handled by DetectionSystem
        }
    }
}

int GasDetector::getGasLevel() const {
    return gasLevel;
}

void GasDetector::setGasLevel(int level) {
    if (level < 0) level = 0;
    if (level > 100) level = 100;
    gasLevel = level;
    detect();  // Auto-check after level change
}

std::string GasDetector::getGasType() const {
    return gasType;
}

// Nest Gas Detector
NestGasDetector::NestGasDetector()
    : GasDetector("Nest", "Protect CO") {
    sensitivityLevel = 8;
    gasType = "Carbon Monoxide";
}

NestGasDetector::~NestGasDetector() {}

Device* NestGasDetector::clone() const {
    NestGasDetector* newDet = new NestGasDetector();
    newDet->copyConfigurationFrom(this);
    return newDet;
}

// Kidde Gas Detector
KiddeGasDetector::KiddeGasDetector()
    : GasDetector("Kidde", "Nighthawk") {
    sensitivityLevel = 7;
    gasType = "Natural Gas/Propane";
}

KiddeGasDetector::~KiddeGasDetector() {}

Device* KiddeGasDetector::clone() const {
    KiddeGasDetector* newDet = new KiddeGasDetector();
    newDet->copyConfigurationFrom(this);
    return newDet;
}
