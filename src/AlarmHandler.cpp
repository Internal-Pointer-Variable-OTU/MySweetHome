/**
 * @file AlarmHandler.cpp
 * @brief Implementation of Alarm Handler
 * @version 5.0
 * @date 03/12/2025
 * 
 * @authors
 * - 220201047: Security System - Chain of Responsibility (Handler)
 * 
 * @patterns Chain of Responsibility
 */

#include "AlarmHandler.h"
#include <iostream>

AlarmHandler::AlarmHandler(Alarm* alarm) : alarm(alarm) {}

void AlarmHandler::handleRequest() {
    std::cout << "[SECURITY] Triggering Alarm..." << std::endl;
    if (alarm) {
        alarm->ring();
    }
    
    // Pass to next handler
    SecurityHandler::handleRequest();
}
