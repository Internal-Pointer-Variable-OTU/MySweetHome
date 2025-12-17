#ifndef DETECTIONSYSTEM_H
#define DETECTIONSYSTEM_H

#include <string>
#include <vector>

// Forward declarations
class Alarm;
class Light;
class Detector;

// Chain of Responsibility Pattern for Detection - Can be interrupted
class DetectionHandler {
protected:
    DetectionHandler* nextHandler;
    std::string handlerName;
    int durationSeconds;
    bool interrupted;

public:
    DetectionHandler(const std::string& name, int duration);
    virtual ~DetectionHandler();
    
    void setNext(DetectionHandler* handler);
    virtual bool handle() = 0;  // Returns true if should continue, false if interrupted
    bool executeChain();
    void interrupt();
    void reset();
    
    std::string getName() const;
};

// Concrete Detection Handlers
class DetectionAlarmHandler : public DetectionHandler {
private:
    Alarm* alarm;
public:
    DetectionAlarmHandler(Alarm* alarmDevice, int duration = 3);
    virtual bool handle();
};

class DetectionBlinkHandler : public DetectionHandler {
private:
    std::vector<Light*>* lights;
    int blinkCount;
public:
    DetectionBlinkHandler(std::vector<Light*>* lightDevices, int duration = 5, int blinks = 5);
    virtual bool handle();
};

class FireStationCallHandler : public DetectionHandler {
public:
    FireStationCallHandler(int duration = 1);
    virtual bool handle();
};

// Detection System - Facade for smoke/gas detection operations
class DetectionSystem {
private:
    bool isActive;
    bool sequenceRunning;
    Alarm* alarm;
    std::vector<Light*>* lights;
    
    // Chain of handlers
    DetectionAlarmHandler* alarmHandler;
    DetectionBlinkHandler* blinkHandler;
    FireStationCallHandler* fireHandler;

public:
    DetectionSystem(Alarm* alarmDevice, std::vector<Light*>* lightDevices);
    ~DetectionSystem();
    
    void activate();
    void deactivate();
    bool isActivated() const;
    
    void triggerDetectionSequence();
    void acknowledgeAlarm();  // User acknowledgment to stop sequence
    
    void handleSmokeDetection();
    void handleGasDetection();
    
    void displayStatus() const;
};

#endif // DETECTIONSYSTEM_H