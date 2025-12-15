#include "NotificationSystem.h"
#include <iostream>

// Default strategy placeholder (LogStrategy typically)
class NotificationStrategy {
public:
    virtual ~NotificationStrategy() {}
    virtual void send(const std::string& msg) = 0;
};

class LogStrategy : public NotificationStrategy {
public:
    void send(const std::string& msg) {
        std::cout << "[LOG] Notification: " << msg << std::endl;
    }
};

NotificationSystem::NotificationSystem() : strategy(new LogStrategy()) {
}

NotificationSystem::~NotificationSystem() {
    delete strategy;
}

void NotificationSystem::setStrategy(NotificationStrategy* newStrategy) {
    if (strategy) delete strategy;
    strategy = newStrategy;
}

void NotificationSystem::notify(const std::string& message) {
    if (strategy) {
        strategy->send(message);
    }
}

void NotificationSystem::update(Device* device, const std::string& message) {
    std::string fullMsg = "Device " + device->getName() + ": " + message;
    notify(fullMsg);
}

void NotificationSystem::displayStatus() const {
    std::cout << "--- NOTIFICATION SYSTEM ---" << std::endl;
    std::cout << "  Status: Active" << std::endl;
    std::cout << "  Strategy: Log (Default)" << std::endl;
}