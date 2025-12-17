#include "HomeController.h"
#include "Device.h"
#include "Light.h"
#include "Camera.h"
#include "Television.h"
#include "SmokeDetector.h"
#include "GasDetector.h"
#include "SoundSystem.h"
#include "Alarm.h"
#include "Menu.h"
#include "Storage.h"
#include "ModeManager.h"
#include "StateManager.h"
#include "SecuritySystem.h"
#include "DetectionSystem.h"
#include "NotificationSystem.h"
#include "DeviceFactory.h"
#include <iostream>
#include <sstream>

HomeController::HomeController() : isRunning(false) {
    // Initialize singletons
    alarm = Alarm::getInstance();
    storage = Storage::getInstance();
    
    // Initialize managers
    menu = new Menu();
    modeManager = new ModeManager();
    stateManager = new StateManager();
    
    // Initialize notification system
    notificationSystem = new NotificationSystem();
    
    // Set alarm observer
    alarm->setObserver(notificationSystem);
    
    // Initialize default devices
    initializeDefaultDevices();
    
    // Update light pointers for systems
    updateLightPtrs();
    
    // Initialize security and detection systems
    securitySystem = new SecuritySystem(alarm, &lightPtrs);
    detectionSystem = new DetectionSystem(alarm, &lightPtrs);
}

HomeController::~HomeController() {
    // Clean up devices
    for (size_t i = 0; i < allDevices.size(); ++i) {
        delete allDevices[i];
    }
    allDevices.clear();
    lights.clear();
    cameras.clear();
    televisions.clear();
    smokeDetectors.clear();
    gasDetectors.clear();
    soundSystems.clear();
    
    // Clean up managers and systems
    delete menu;
    delete modeManager;
    delete stateManager;
    delete securitySystem;
    delete detectionSystem;
    delete notificationSystem;
    
    // Note: Alarm and Storage are singletons, not deleted here
}

void HomeController::initializeDefaultDevices() {
    // Add one of each device type by default
    addLight(1);  // Philips Hue
    addCamera(1); // Samsung
    addTV(1);     // Samsung
    addDetectorPair(1);  // Nest
    addSoundSystem(1);   // Sonos
}

void HomeController::registerDevice(Device* device) {
    if (device) {
        device->setObserver(notificationSystem);
        allDevices.push_back(device);
    }
}

void HomeController::unregisterDevice(Device* device) {
    for (std::vector<Device*>::iterator it = allDevices.begin(); it != allDevices.end(); ++it) {
        if (*it == device) {
            allDevices.erase(it);
            break;
        }
    }
}

void HomeController::updateLightPtrs() {
    lightPtrs.clear();
    for (size_t i = 0; i < lights.size(); ++i) {
        Light* light = dynamic_cast<Light*>(lights[i]);
        if (light) {
            lightPtrs.push_back(light);
        }
    }
}

void HomeController::start() {
    // Open log file
    storage->openFile("msh_log.txt");
    storage->logSystemStart();
    
    isRunning = true;
    
    std::cout << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "       MY SWEET HOME SYSTEM STARTING       " << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << std::endl;
    
    // Apply default mode (Normal)
    std::cout << "[INIT] Applying default mode (Normal)..." << std::endl;
    modeManager->applyMode(lights, televisions, soundSystems);
    
    // Apply default state (Normal)
    std::cout << "[INIT] Applying default state (Normal)..." << std::endl;
    stateManager->applyState();
    
    // Activate security system
    std::cout << "[INIT] Activating security system..." << std::endl;
    securitySystem->activate();
    
    // Activate detection system
    std::cout << "[INIT] Activating detection system..." << std::endl;
    detectionSystem->activate();
    
    // Save initial state
    stateManager->saveState(modeManager->getCurrentModeName(), allDevices);
    
    std::cout << std::endl;
    std::cout << "[INIT] System initialization complete!" << std::endl;
    std::cout << std::endl;
    
    storage->logInfo("System initialized successfully");
}

void HomeController::run() {
    while (isRunning) {
        menu->displayMainMenu();
        
        int choice = menu->getMenuChoice();
        storage->logMenuSelection(choice);
        
        switch (choice) {
            case 1:
                handleGetStatus();
                break;
            case 2:
                handleAddDevice();
                break;
            case 3:
                handleRemoveDevice();
                break;
            case 4:
                handlePowerOn();
                break;
            case 5:
                handlePowerOff();
                break;
            case 6:
                handleChangeMode();
                break;
            case 7:
                handleChangeState();
                break;
            case 8:
                handleManual();
                break;
            case 9:
                handleAbout();
                break;
            case 10:
                handleShutdown();
                break;
            default:
                menu->displayError("Invalid menu option. Please try again.");
        }
        
        if (isRunning) {
            menu->waitForKey();
        }
    }
}

void HomeController::shutdown() {
    std::cout << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "      MY SWEET HOME SYSTEM SHUTTING DOWN   " << std::endl;
    std::cout << "============================================" << std::endl;
    
    // Deactivate systems
    securitySystem->deactivate();
    detectionSystem->deactivate();
    
    // Power off all non-critical devices
    for (size_t i = 0; i < allDevices.size(); ++i) {
        allDevices[i]->powerOff();
    }
    
    storage->logSystemShutdown();
    storage->closeFile();
    
    isRunning = false;
    
    std::cout << std::endl;
    std::cout << "Goodbye! Thank you for using My Sweet Home." << std::endl;
    std::cout << std::endl;
}

void HomeController::handleGetStatus() {
    menu->clearScreen();
    std::cout << std::endl;
    std::cout << "======================================================================" << std::endl;
    std::cout << "                        HOME STATUS REPORT                           " << std::endl;
    std::cout << "======================================================================" << std::endl;
    
    // Current state and mode
    std::cout << std::endl;
    std::cout << "--- SYSTEM STATUS ---" << std::endl;
    stateManager->displayCurrentState();
    std::cout << std::endl;
    modeManager->displayCurrentMode();
    
    // Security and detection
    std::cout << std::endl;
    securitySystem->displayStatus();
    std::cout << std::endl;
    detectionSystem->displayStatus();
    std::cout << std::endl;
    notificationSystem->displayStatus();
    
    // All devices
    std::cout << std::endl;
    displayAllDevices();
    
    // Alarm (singleton)
    std::cout << std::endl;
    std::cout << "--- ALARM ---" << std::endl;
    std::cout << "  " << alarm->getStatus() << std::endl;
    
    std::cout << std::endl;
    std::cout << "======================================================================" << std::endl;
}

void HomeController::handleAddDevice() {
    menu->displayAddDeviceSubmenu();
    char choice = menu->getCharChoice();
    
    if (choice == 'Q' || choice == 'q') {
        return;
    }
    
    std::cout << "  Enter number of devices to add: ";
    int count = menu->getNumberInput();
    
    if (count <= 0) {
        menu->displayError("Invalid number.");
        return;
    }
    
    std::cout << "  Select brand (1 or 2): ";
    int brandChoice = menu->getNumberInput();
    
    addDevices(choice, count, brandChoice);
    
    storage->logInfo("Added " + std::string(1, choice) + " device(s)");
}

void HomeController::handleRemoveDevice() {
    menu->displayRemoveDeviceSubmenu();
    char choice = menu->getCharChoice();
    
    if (choice == 'Q' || choice == 'q') {
        return;
    }
    
    std::vector<Device*>* targetList = NULL;
    std::string typeName;
    
    switch (choice) {
        case 'L': case 'l':
            targetList = &lights;
            typeName = "Light";
            break;
        case 'C': case 'c':
            targetList = &cameras;
            typeName = "Camera";
            break;
        case 'T': case 't':
            targetList = &televisions;
            typeName = "TV";
            break;
        case 'D': case 'd':
            targetList = &smokeDetectors;
            typeName = "Detector";
            break;
        case 'S': case 's':
            targetList = &soundSystems;
            typeName = "Sound System";
            break;
        default:
            menu->displayError("Invalid device type.");
            return;
    }
    
    if (targetList->empty()) {
        menu->displayError("No devices of this type to remove.");
        return;
    }
    
    // Display current devices
    displayDeviceList(*targetList, typeName + "s");
    
    std::cout << "  Enter device index to remove (1-" << targetList->size() << "): ";
    int index = menu->getNumberInput();
    
    removeDevice(choice, index);
}

void HomeController::handlePowerOn() {
    menu->displayPowerOnSubmenu();
    char choice = menu->getCharChoice();
    
    if (choice == 'Q' || choice == 'q') {
        return;
    }
    
    powerOnDevices(choice);
}

void HomeController::handlePowerOff() {
    menu->displayPowerOffSubmenu();
    char choice = menu->getCharChoice();
    
    if (choice == 'Q' || choice == 'q') {
        return;
    }
    
    powerOffDevices(choice);
}

void HomeController::handleChangeMode() {
    std::string oldMode = modeManager->getCurrentModeName();
    
    menu->displayModeSubmenu();
    char choice = menu->getCharChoice();
    
    if (choice == 'Q' || choice == 'q') {
        return;
    }
    
    modeManager->setMode(choice);
    modeManager->applyMode(lights, televisions, soundSystems);
    
    // Save state after mode change
    stateManager->saveState(modeManager->getCurrentModeName(), allDevices);
    
    storage->logModeChange(oldMode, modeManager->getCurrentModeName());
}

void HomeController::handleChangeState() {
    std::string oldState = stateManager->getCurrentStateName();
    
    menu->displayStateSubmenu();
    char choice = menu->getCharChoice();
    
    if (choice == 'Q' || choice == 'q') {
        return;
    }
    
    stateManager->setState(choice);
    
    // Save state after state change (except for 'previous' which restores)
    if (choice != 'P' && choice != 'p') {
        stateManager->saveState(modeManager->getCurrentModeName(), allDevices);
    }
    
    storage->logStateChange(oldState, stateManager->getCurrentStateName());
}

void HomeController::handleManual() {
    menu->displayManual();
    storage->logInfo("Manual displayed");
}

void HomeController::handleAbout() {
    menu->displayAbout();
    storage->logInfo("About displayed");
}

void HomeController::handleShutdown() {
    std::cout << std::endl;
    std::cout << "  Are you sure you want to shutdown? (Y/N): ";
    char confirm = menu->getCharChoice();
    
    if (confirm == 'Y' || confirm == 'y') {
        shutdown();
    }
}

void HomeController::addDevices(char deviceType, int count, int brandChoice) {
    Device* prototype = NULL;
    std::vector<Device*>* targetList = NULL;
    
    // Get prototype and target list
    switch (deviceType) {
        case 'L': case 'l':
            prototype = SimpleDeviceFactory::createDeviceByInput('L', brandChoice);
            targetList = &lights;
            break;
        case 'C': case 'c':
            prototype = SimpleDeviceFactory::createDeviceByInput('C', brandChoice);
            targetList = &cameras;
            break;
        case 'T': case 't':
            prototype = SimpleDeviceFactory::createDeviceByInput('T', brandChoice);
            targetList = &televisions;
            break;
        case 'S': case 's':
            prototype = SimpleDeviceFactory::createDeviceByInput('S', brandChoice);
            targetList = &soundSystems;
            break;
        case 'D': case 'd':
            // Detectors come as pairs
            for (int i = 0; i < count; ++i) {
                addDetectorPair(brandChoice);
            }
            menu->displaySuccess("Added detector pair(s).");
            return;
        default:
            menu->displayError("Invalid device type.");
            return;
    }
    
    if (!prototype) {
        menu->displayError("Failed to create device.");
        return;
    }
    
    // Copy configuration from existing device if available
    Device* configSource = targetList->empty() ? NULL : targetList->back();
    
    // Add the first device
    if (configSource) {
        prototype->copyConfigurationFrom(configSource);
    }
    targetList->push_back(prototype);
    registerDevice(prototype);
    
    // Clone for remaining devices (Prototype pattern)
    for (int i = 1; i < count; ++i) {
        Device* clone = prototype->clone();
        targetList->push_back(clone);
        registerDevice(clone);
    }
    
    // Update light pointers if needed
    if (deviceType == 'L' || deviceType == 'l') {
        updateLightPtrs();
    }
    
    std::ostringstream oss;
    oss << "Added " << count << " device(s) of type " << deviceType;
    menu->displaySuccess(oss.str());
}

void HomeController::removeDevice(char deviceType, int index) {
    std::vector<Device*>* targetList = NULL;
    std::vector<Device*>* pairedList = NULL;  // For detectors
    
    switch (deviceType) {
        case 'L': case 'l':
            targetList = &lights;
            break;
        case 'C': case 'c':
            targetList = &cameras;
            break;
        case 'T': case 't':
            targetList = &televisions;
            break;
        case 'D': case 'd':
            targetList = &smokeDetectors;
            pairedList = &gasDetectors;
            break;
        case 'S': case 's':
            targetList = &soundSystems;
            break;
        default:
            return;
    }
    
    if (index < 1 || index > (int)targetList->size()) {
        menu->displayError("Invalid device index.");
        return;
    }
    
    Device* device = (*targetList)[index - 1];
    unregisterDevice(device);
    targetList->erase(targetList->begin() + (index - 1));
    delete device;
    
    // Remove paired gas detector if removing smoke detector
    if (pairedList && index <= (int)pairedList->size()) {
        Device* paired = (*pairedList)[index - 1];
        unregisterDevice(paired);
        pairedList->erase(pairedList->begin() + (index - 1));
        delete paired;
    }
    
    // Update light pointers if needed
    if (deviceType == 'L' || deviceType == 'l') {
        updateLightPtrs();
    }
    
    menu->displaySuccess("Device removed.");
}

void HomeController::powerOnDevices(char deviceType) {
    std::vector<Device*>* targetList = NULL;
    
    switch (deviceType) {
        case 'L': case 'l':
            targetList = &lights;
            break;
        case 'C': case 'c':
            targetList = &cameras;
            break;
        case 'T': case 't':
            targetList = &televisions;
            break;
        case 'D': case 'd':
            std::cout << "[INFO] Detectors are always on." << std::endl;
            return;
        case 'S': case 's':
            targetList = &soundSystems;
            break;
        case 'A': case 'a':
            // Power on all
            for (size_t i = 0; i < allDevices.size(); ++i) {
                allDevices[i]->powerOn();
            }
            menu->displaySuccess("All devices powered on.");
            return;
        default:
            menu->displayError("Invalid device type.");
            return;
    }
    
    for (size_t i = 0; i < targetList->size(); ++i) {
        (*targetList)[i]->powerOn();
    }
    
    menu->displaySuccess("Devices powered on.");
}

void HomeController::powerOffDevices(char deviceType) {
    std::vector<Device*>* targetList = NULL;
    
    switch (deviceType) {
        case 'L': case 'l':
            targetList = &lights;
            break;
        case 'C': case 'c':
            targetList = &cameras;
            break;
        case 'T': case 't':
            targetList = &televisions;
            break;
        case 'D': case 'd':
            std::cout << "[WARNING] Detectors cannot be powered off (critical devices)." << std::endl;
            return;
        case 'S': case 's':
            targetList = &soundSystems;
            break;
        case 'A': case 'a':
            // Power off all (except critical)
            for (size_t i = 0; i < lights.size(); ++i) {
                lights[i]->powerOff();
            }
            for (size_t i = 0; i < cameras.size(); ++i) {
                cameras[i]->powerOff();
            }
            for (size_t i = 0; i < televisions.size(); ++i) {
                televisions[i]->powerOff();
            }
            for (size_t i = 0; i < soundSystems.size(); ++i) {
                soundSystems[i]->powerOff();
            }
            menu->displaySuccess("All non-critical devices powered off.");
            return;
        default:
            menu->displayError("Invalid device type.");
            return;
    }
    
    for (size_t i = 0; i < targetList->size(); ++i) {
        (*targetList)[i]->powerOff();
    }
    
    menu->displaySuccess("Devices powered off.");
}

void HomeController::displayDeviceList(const std::vector<Device*>& devices, const std::string& title) const {
    std::cout << "--- " << title << " (" << devices.size() << ") ---" << std::endl;
    if (devices.empty()) {
        std::cout << "  (No devices)" << std::endl;
    } else {
        for (size_t i = 0; i < devices.size(); ++i) {
            std::cout << "  [" << (i + 1) << "] " << devices[i]->getStatus() << std::endl;
        }
    }
}

void HomeController::displayAllDevices() const {
    std::cout << "--- CONNECTED DEVICES ---" << std::endl;
    std::cout << std::endl;
    
    displayDeviceList(lights, "Lights");
    std::cout << std::endl;
    displayDeviceList(cameras, "Cameras");
    std::cout << std::endl;
    displayDeviceList(televisions, "TVs");
    std::cout << std::endl;
    displayDeviceList(smokeDetectors, "Smoke Detectors");
    std::cout << std::endl;
    displayDeviceList(gasDetectors, "Gas Detectors");
    std::cout << std::endl;
    displayDeviceList(soundSystems, "Sound Systems");
}

void HomeController::addLight(int brandChoice) {
    Light* light;
    if (brandChoice == 1) {
        light = new PhilipsHueLight();
    } else {
        light = new IKEATradfriLight();
    }
    lights.push_back(light);
    registerDevice(light);
    updateLightPtrs();
}

void HomeController::addCamera(int brandChoice) {
    Camera* camera;
    if (brandChoice == 1) {
        camera = new SamsungCamera();
    } else {
        camera = new XiaomiCamera();
    }
    cameras.push_back(camera);
    registerDevice(camera);
}

void HomeController::addTV(int brandChoice) {
    Television* tv;
    if (brandChoice == 1) {
        tv = new SamsungTV();
    } else {
        tv = new LGTV();
    }
    televisions.push_back(tv);
    registerDevice(tv);
}

void HomeController::addDetectorPair(int brandChoice) {
    SmokeDetector* smoke;
    GasDetector* gas;
    
    if (brandChoice == 1) {
        NestDetectorFactory factory;
        smoke = factory.createSmokeDetector();
        gas = factory.createGasDetector();
    } else {
        BudgetDetectorFactory factory;
        smoke = factory.createSmokeDetector();
        gas = factory.createGasDetector();
    }
    
    smokeDetectors.push_back(smoke);
    gasDetectors.push_back(gas);
    registerDevice(smoke);
    registerDevice(gas);
}

void HomeController::addSoundSystem(int brandChoice) {
    SoundSystem* ss;
    if (brandChoice == 1) {
        ss = new SonosSoundSystem();
    } else {
        ss = new BoseSoundSystem();
    }
    soundSystems.push_back(ss);
    registerDevice(ss);
}

void HomeController::displayStatus() const {
    std::cout << std::endl;
    std::cout << "======================================================================" << std::endl;
    std::cout << "                        HOME STATUS REPORT                           " << std::endl;
    std::cout << "======================================================================" << std::endl;
    displayAllDevices();
}

bool HomeController::isSystemRunning() const {
    return isRunning;
}

void HomeController::simulateMotionDetection() {
    std::cout << std::endl;
    std::cout << "=== SIMULATION: Motion Detection ===" << std::endl;
    
    if (!cameras.empty()) {
        Camera* cam = dynamic_cast<Camera*>(cameras[0]);
        if (cam) {
            cam->detectMotion();
            securitySystem->handleMotionDetection();
        }
    }
}

void HomeController::simulateSmokeDetection() {
    std::cout << std::endl;
    std::cout << "=== SIMULATION: Smoke Detection ===" << std::endl;
    
    if (!smokeDetectors.empty()) {
        SmokeDetector* smoke = dynamic_cast<SmokeDetector*>(smokeDetectors[0]);
        if (smoke) {
            smoke->setSmokeLevel(80);  // High smoke level
            detectionSystem->handleSmokeDetection();
        }
    }
}

void HomeController::simulateGasDetection() {
    std::cout << std::endl;
    std::cout << "=== SIMULATION: Gas Detection ===" << std::endl;
    
    if (!gasDetectors.empty()) {
        GasDetector* gas = dynamic_cast<GasDetector*>(gasDetectors[0]);
        if (gas) {
            gas->setGasLevel(70);  // High gas level
            detectionSystem->handleGasDetection();
        }
    }
}

void HomeController::simulateDeviceFailure(int deviceIndex) {
    if (deviceIndex >= 0 && deviceIndex < (int)allDevices.size()) {
        std::cout << std::endl;
        std::cout << "=== SIMULATION: Device Failure ===" << std::endl;
        allDevices[deviceIndex]->setOperationMode(false);
    }
}
