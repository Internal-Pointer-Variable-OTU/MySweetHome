#include <iostream>
#include <vector>
#include <string>
#include "../include/SecuritySystem.h"
#include "../include/DetectionSystem.h"
#include "../include/Alarm.h"
#include "../include/Light.h"

// Mock Light for testing
class MockLight : public Light {
public:
    MockLight() : Light("TestBrand", "TestModel") {}
    virtual std::string getBrand() const { return "Mock"; }
    virtual Device* clone() const { return new MockLight(*this); }
};

int main() {
    std::cout << "=== SEQUENCE VERIFICATION TEST ===" << std::endl;

    // Setup Components
    Alarm* alarm = Alarm::getInstance();
    std::vector<Light*> lights;
    lights.push_back(new MockLight());

    // 1. Test Security System Sequence
    std::cout << "\n[TEST] Testing Security System Sequence..." << std::endl;
    SecuritySystem securitySys(alarm, &lights);
    
    securitySys.activate();
    // Trigger sequence manually or via motion
    securitySys.handleMotionDetection();

    // 2. Test Detection System Sequence
    std::cout << "\n[TEST] Testing Detection System Sequence..." << std::endl;
    DetectionSystem detectionSys(alarm, &lights);
    
    detectionSys.activate();
    detectionSys.handleSmokeDetection();

    // Cleanup
    for(size_t i=0; i<lights.size(); ++i) delete lights[i];
    
    std::cout << "\n=== TEST COMPLETE ===" << std::endl;
    return 0;
}
