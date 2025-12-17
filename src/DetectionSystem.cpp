#include "DetectionSystem.h"
#include "Alarm.h"
#include "Light.h"
#include <iostream>

// DetectionHandler Implementation
DetectionHandler::DetectionHandler(const std::string& name, int duration)
    : nextHandler(NULL), handlerName(name), durationSeconds(duration), interrupted(false) {
}

DetectionHandler::~DetectionHandler() {}

void DetectionHandler::setNext(DetectionHandler* handler) {
    nextHandler = handler;
}

bool DetectionHandler::executeChain() {
    if (interrupted) {
        std::cout << "[DETECTION] Sequence interrupted by user." << std::endl;
        return false;
    }
    
    if (!handle()) {
        return false;  // Handler was interrupted
    }
    
    if (nextHandler && !interrupted) {
        return nextHandler->executeChain();
    }
    
    return true;
}

void DetectionHandler::interrupt() {
    interrupted = true;
    if (nextHandler) {
        nextHandler->interrupt();
    }
}

void DetectionHandler::reset() {
    interrupted = false;
    if (nextHandler) {
        nextHandler->reset();
    }
}

std::string DetectionHandler::getName() const {
    return handlerName;
}

// DetectionAlarmHandler Implementation
DetectionAlarmHandler::DetectionAlarmHandler(Alarm* alarmDevice, int duration)
    : DetectionHandler("Detection Alarm", duration), alarm(alarmDevice) {
}

bool DetectionAlarmHandler::handle() {
    if (interrupted) return false;
    
    std::cout << ">>> DETECTION: Triggering ALARM for " << durationSeconds << " seconds..." << std::endl;
    std::cout << ">>> Press any key to acknowledge and stop the alarm..." << std::endl;
    
    if (alarm) {
        alarm->ring();
    }
    
    // Simulate waiting for acknowledgment
    std::cout << ">>> ALARM sounding for " << durationSeconds << " seconds..." << std::endl;
    
    if (interrupted) {
        std::cout << ">>> Alarm acknowledged by user." << std::endl;
        if (alarm) alarm->stop();
        return false;
    }
    
    return true;
}

// DetectionBlinkHandler Implementation
DetectionBlinkHandler::DetectionBlinkHandler(std::vector<Light*>* lightDevices, int duration, int blinks)
    : DetectionHandler("Light Blinking", duration), lights(lightDevices), blinkCount(blinks) {
}

bool DetectionBlinkHandler::handle() {
    if (interrupted) return false;
    
    std::cout << ">>> DETECTION: Blinking lights ON/OFF with 1 second interval..." << std::endl;
    std::cout << ">>> Press any key to acknowledge and stop..." << std::endl;
    
    if (lights) {
        for (int i = 0; i < blinkCount && !interrupted; ++i) {
            std::cout << ">>> BLINK " << (i + 1) << "/" << blinkCount << std::endl;
            for (size_t j = 0; j < lights->size(); ++j) {
                (*lights)[j]->blinkLight();
            }
        }
    }
    
    if (interrupted) {
        std::cout << ">>> Blinking stopped by user." << std::endl;
        return false;
    }
    
    return true;
}

// FireStationCallHandler Implementation
FireStationCallHandler::FireStationCallHandler(int duration)
    : DetectionHandler("Fire Station Call", duration) {
}

bool FireStationCallHandler::handle() {
    if (interrupted) return false;
    
    std::cout << ">>> DETECTION: CALLING FIRE STATION..." << std::endl;
    std::cout << ">>> [SIMULATED] Fire station call initiated!" << std::endl;
    std::cout << ">>> Emergency services have been notified." << std::endl;
    
    return true;
}

// DetectionSystem Implementation
DetectionSystem::DetectionSystem(Alarm* alarmDevice, std::vector<Light*>* lightDevices)
    : isActive(true), sequenceRunning(false), alarm(alarmDevice), lights(lightDevices) {
    
    // Create chain of responsibility
    alarmHandler = new DetectionAlarmHandler(alarm, 3);
    blinkHandler = new DetectionBlinkHandler(lights, 5, 5);
    fireHandler = new FireStationCallHandler(1);
    
    // Setup chain: Alarm -> Blink -> Fire Station
    alarmHandler->setNext(blinkHandler);
    blinkHandler->setNext(fireHandler);
}

DetectionSystem::~DetectionSystem() {
    delete alarmHandler;
    delete blinkHandler;
    delete fireHandler;
}

void DetectionSystem::activate() {
    isActive = true;
    std::cout << "[DETECTION] Detection system ACTIVATED." << std::endl;
}

void DetectionSystem::deactivate() {
    isActive = false;
    if (alarm) {
        alarm->stop();
    }
    std::cout << "[DETECTION] Detection system DEACTIVATED." << std::endl;
}

bool DetectionSystem::isActivated() const {
    return isActive;
}

void DetectionSystem::triggerDetectionSequence() {
    if (!isActive) {
        std::cout << "[DETECTION] Detection system is not active. Sequence not triggered." << std::endl;
        return;
    }
    
    std::cout << std::endl;
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "!!!     HAZARD DETECTED                 !!!" << std::endl;
    std::cout << "!!!   INITIATING DETECTION SEQUENCE     !!!" << std::endl;
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << std::endl;
    
    sequenceRunning = true;
    
    // Reset any previous interrupts
    alarmHandler->reset();
    
    // Execute chain of responsibility
    bool completed = alarmHandler->executeChain();
    
    if (completed) {
        std::cout << std::endl;
        std::cout << "[DETECTION] Full detection sequence completed." << std::endl;
    } else {
        std::cout << std::endl;
        std::cout << "[DETECTION] Sequence was acknowledged/interrupted by user." << std::endl;
    }
    
    sequenceRunning = false;
}

void DetectionSystem::acknowledgeAlarm() {
    if (sequenceRunning) {
        std::cout << "[DETECTION] User acknowledged - interrupting sequence..." << std::endl;
        alarmHandler->interrupt();
        if (alarm) {
            alarm->stop();
        }
    } else {
        std::cout << "[DETECTION] No active sequence to acknowledge." << std::endl;
    }
}

void DetectionSystem::handleSmokeDetection() {
    std::cout << "[DETECTION] SMOKE detected by sensors!" << std::endl;
    triggerDetectionSequence();
}

void DetectionSystem::handleGasDetection() {
    std::cout << "[DETECTION] GAS detected by sensors!" << std::endl;
    triggerDetectionSequence();
}

void DetectionSystem::displayStatus() const {
    std::cout << "=== Detection System Status ===" << std::endl;
    std::cout << "  Status: " << (isActive ? "ACTIVE" : "INACTIVE") << std::endl;
    std::cout << "  Sequence Running: " << (sequenceRunning ? "YES" : "NO") << std::endl;
}
