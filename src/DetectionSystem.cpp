/**
 * @file DetectionSystem.cpp
 * @brief Implementation of Detection System
 * @version 5.0
 * @date 03/12/2025
 * 
 * @authors
 * - 210201856: Detection System - Chain of Responsibility, Observer (Smoke/Gas)
 * 
 * @patterns Chain of Responsibility, Observer
 */

#include "DetectionSystem.h"
#include <iostream>

DetectionSystem::DetectionSystem(Alarm* alarm, std::vector<Light*>* lights) 
    : alarm(alarm), lights(lights) {
}

DetectionSystem::~DetectionSystem() {
}

void DetectionSystem::activate() {
    std::cout << "[DetectionSystem] System Activated. Monitoring sensors..." << std::endl;
}

void DetectionSystem::deactivate() {
    std::cout << "[DetectionSystem] System Deactivated." << std::endl;
}

void DetectionSystem::handleSmokeDetection() {
    std::cout << "[DetectionSystem] WARNING: Smoke Detected!" << std::endl;
    alarm->trigger();
    // Logic for blinking lights would go here (Chain of Responsibility)
}

void DetectionSystem::handleGasDetection() {
    std::cout << "[DetectionSystem] WARNING: Gas Detected!" << std::endl;
    alarm->trigger();
    // Logic for blinking lights would go here (Chain of Responsibility)
}

void DetectionSystem::displayStatus() const {
    std::cout << "--- DETECTION SYSTEM ---" << std::endl;
    std::cout << "  Status: Active" << std::endl;
    std::cout << "  Sensors: Monitoring" << std::endl;
}
