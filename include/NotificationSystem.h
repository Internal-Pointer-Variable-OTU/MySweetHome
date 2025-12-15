#ifndef NOTIFICATIONSYSTEM_H
#define NOTIFICATIONSYSTEM_H

#include "Device.h" // For IDeviceObserver
#include <string>

// Forward declaration
class NotificationStrategy;

class NotificationSystem : public IDeviceObserver {
private:
    NotificationStrategy* strategy;

public:
    NotificationSystem();
    virtual ~NotificationSystem();
    
    void setStrategy(NotificationStrategy* newStrategy);
    void notify(const std::string& message);
    
    // Observer update method
    virtual void update(Device* device, const std::string& message);
    
    void displayStatus() const;
};

#endif // NOTIFICATIONSYSTEM_H