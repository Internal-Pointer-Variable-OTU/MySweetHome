#ifndef DETECTION_SYSTEM_H
#define DETECTION_SYSTEM_H

#include <string>

// Interface for DetectionSystem
class DetectionSystem {
public:
    virtual ~DetectionSystem() {}
    virtual void handleDetection(const std::string& detectorName, const std::string& detectionType, const std::string& message) = 0;
};

#endif // DETECTION_SYSTEM_H
