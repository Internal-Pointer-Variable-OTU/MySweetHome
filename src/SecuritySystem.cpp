#include "SecuritySystem.h"
#include "Alarm.h"
#include "Light.h"
#include <iostream>

// SecurityHandler Implementation
SecurityHandler::SecurityHandler(const std::string &name, int duration)
    : nextHandler(NULL), handlerName(name), durationSeconds(duration)
{
}

SecurityHandler::~SecurityHandler() {}

void SecurityHandler::setNext(SecurityHandler *handler)
{
    nextHandler = handler;
}

void SecurityHandler::executeChain()
{
    handle();
    if (nextHandler)
    {
        nextHandler->executeChain();
    }
}

std::string SecurityHandler::getName() const
{
    return handlerName;
}

int SecurityHandler::getDuration() const
{
    return durationSeconds;
}

// AlarmHandler Implementation
AlarmHandler::AlarmHandler(Alarm *alarmDevice, int duration)
    : SecurityHandler("Alarm", duration), alarm(alarmDevice)
{
}

void AlarmHandler::handle()
{
    std::cout << ">>> SECURITY: Triggering ALARM for " << durationSeconds << " seconds..." << std::endl;
    if (alarm)
    {
        alarm->ring();
    }
    // Simulate duration
    std::cout << ">>> ALARM active for " << durationSeconds << " seconds" << std::endl;
}

// LightHandler Implementation
LightHandler::LightHandler(std::vector<Light *> *lightDevices, int duration)
    : SecurityHandler("Light", duration), lights(lightDevices)
{
}

void LightHandler::handle()
{
    std::cout << ">>> SECURITY: Turning ON all lights for " << durationSeconds << " seconds..." << std::endl;
    if (lights)
    {
        for (size_t i = 0; i < lights->size(); ++i)
        {
            (*lights)[i]->powerOn();
            (*lights)[i]->setBrightness(100);
        }
    }
    std::cout << ">>> LIGHTS ON for " << durationSeconds << " seconds" << std::endl;
}

// PoliceCallHandler Implementation
PoliceCallHandler::PoliceCallHandler(int duration)
    : SecurityHandler("Police Call", duration)
{
}

void PoliceCallHandler::handle()
{
    std::cout << ">>> SECURITY: CALLING POLICE..." << std::endl;
    std::cout << ">>> [SIMULATED] Police call initiated!" << std::endl;
    std::cout << ">>> Emergency services have been notified." << std::endl;
}

// SecuritySystem Implementation
SecuritySystem::SecuritySystem(Alarm *alarmDevice, std::vector<Light *> *lightDevices)
    : isActive(false), alarm(alarmDevice), lights(lightDevices)
{

    // Create chain of responsibility
    alarmHandler = new AlarmHandler(alarm, 3);
    lightHandler = new LightHandler(lights, 2);
    policeHandler = new PoliceCallHandler(1);

    // Setup chain: Alarm -> Light -> Police
    alarmHandler->setNext(lightHandler);
    lightHandler->setNext(policeHandler);
}

SecuritySystem::~SecuritySystem()
{
    delete alarmHandler;
    delete lightHandler;
    delete policeHandler;
}

void SecuritySystem::activate()
{
    isActive = true;
    std::cout << "[SECURITY] Security system ACTIVATED." << std::endl;
}

void SecuritySystem::deactivate()
{
    isActive = false;
    if (alarm)
    {
        alarm->stop();
    }
    std::cout << "[SECURITY] Security system DEACTIVATED." << std::endl;
}

bool SecuritySystem::isActivated() const
{
    return isActive;
}

void SecuritySystem::triggerSecuritySequence()
{
    if (!isActive)
    {
        std::cout << "[SECURITY] Security system is not active. Sequence not triggered." << std::endl;
        return;
    }

    std::cout << std::endl;
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "!!!     SECURITY BREACH DETECTED        !!!" << std::endl;
    std::cout << "!!!   INITIATING SECURITY SEQUENCE      !!!" << std::endl;
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << std::endl;

    // Execute chain of responsibility
    alarmHandler->executeChain();

    std::cout << std::endl;
    std::cout << "[SECURITY] Security sequence completed." << std::endl;
}

void SecuritySystem::handleMotionDetection()
{
    std::cout << "[SECURITY] Motion detected by camera!" << std::endl;
    triggerSecuritySequence();
}

void SecuritySystem::displayStatus() const
{
    std::cout << "=== Security System Status ===" << std::endl;
    std::cout << "  Status: " << (isActive ? "ACTIVE" : "INACTIVE") << std::endl;
    std::cout << "  Alarm: " << (alarm && alarm->isAlarmRinging() ? "RINGING" : "Silent") << std::endl;
}
