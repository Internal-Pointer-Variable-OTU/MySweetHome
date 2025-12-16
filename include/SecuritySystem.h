#ifndef SECURITYSYSTEM_H
#define SECURITYSYSTEM_H

#include <string>
#include <vector>

// Forward declarations
class Alarm;
class Light;

// Chain of Responsibility Pattern - Security handlers
class SecurityHandler
{
protected:
    SecurityHandler *nextHandler;
    std::string handlerName;
    int durationSeconds;

public:
    SecurityHandler(const std::string &name, int duration);
    virtual ~SecurityHandler();

    void setNext(SecurityHandler *handler);
    virtual void handle() = 0;
    void executeChain();

    std::string getName() const;
    int getDuration() const;
};

// Concrete Handlers
class AlarmHandler : public SecurityHandler
{
private:
    Alarm *alarm;

public:
    AlarmHandler(Alarm *alarmDevice, int duration = 3);
    virtual void handle();
};

class LightHandler : public SecurityHandler
{
private:
    std::vector<Light *> *lights;

public:
    LightHandler(std::vector<Light *> *lightDevices, int duration = 2);
    virtual void handle();
};

class PoliceCallHandler : public SecurityHandler
{
public:
    PoliceCallHandler(int duration = 1);
    virtual void handle();
};

// Security System - Facade for security operations
class SecuritySystem
{
private:
    bool isActive;
    Alarm *alarm;
    std::vector<Light *> *lights;

    // Chain of handlers
    AlarmHandler *alarmHandler;
    LightHandler *lightHandler;
    PoliceCallHandler *policeHandler;

public:
    SecuritySystem(Alarm *alarmDevice, std::vector<Light *> *lightDevices);
    ~SecuritySystem();

    void activate();
    void deactivate();
    bool isActivated() const;

    void triggerSecuritySequence();
    void handleMotionDetection();

    void displayStatus() const;
};

#endif // SECURITYSYSTEM_H
