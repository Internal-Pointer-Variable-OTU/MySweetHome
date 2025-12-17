# My Sweet Home (MSH) - Project Report

## 1. Executive Summary
My Sweet Home (MSH) is a comprehensive Smart Home Management System designed to demonstrate advanced software engineering principles and Design Patterns using C++98. The system simulates a real-world IoT environment where users can control lights, cameras, security systems, and environmental detectors through a unified interface.

The project features a robust **C++ Backend** that handles the core logic and state management, coupled with a modern **Node.js & Web Interface** that allows users to interact with the system via a web browser, simulating a remote control panel.

---

## 2. System Architecture & Features

### 2.1 Core C++ Application
The heart of the system is the `msh` executable. It manages:
- **Device Lifecycle:** Creation, configuration, and control of IoT devices.
- **System States:** Managing different operating modes (Normal, Party, Sleep).
- **Automation Sequences:** Complex event chains like Security Alarms and Fire Detection.
- **Persistence:** Saving and loading system state using a singleton storage mechanism.

### 2.2 Web Interface & Docker Environment
To modernize the user experience, the system includes a web-based terminal interface.
- **Web Socket Communication:** Real-time bi-directional communication between the web UI and the C++ process.
- **Docker Isolation:** The application is containerized. This ensures that every user gets a fresh, isolated instance of the smart home system.
- **Session Management:** The `server.js` handles multi-user sessions, automatically cleaning up resources (files, processes) after inactivity, ensuring system stability.

### 2.3 Key Features
- **Smart Security:** Automated response sequences to motion detection (Alarm -> Lights -> Police).
- **Hazard Detection:** Integrated smoke and gas detection sequences with user-interruptible alarms.
- **Undo/Redo Capability:** Using the Memento pattern to restore previous home states.
- **Clonable Devices:** Ability to duplicate device configurations instantly.

---

## 3. Design Patterns Analysis

The project implements a wide array of GoF (Gang of Four) Design Patterns to solve specific architectural challenges.

### 3.1 Creational Patterns

#### Singleton
- **Usage:** `Alarm`, `Storage`
- **Reasoning:** In a real house, there is typically one main alarm system and one central database/storage. The Singleton pattern ensures exactly one instance of these classes exists and provides a global access point.

#### Factory Method
- **Usage:** `SimpleDeviceFactory`
- **Reasoning:** Encapsulates the object creation logic. The client code (`HomeController`) requests a "TV" or "Light" without needing to know the specific class constructor details.

#### Abstract Factory
- **Usage:** `DeviceFactory`, `DetectorFactory`
- **Reasoning:** Used to create families of related devices (e.g., creating a suite of Samsung devices vs. Sony devices) ensuring compatibility and consistency.

#### Prototype
- **Usage:** `Device::clone()`
- **Reasoning:** Allows for creating new devices by copying an existing one. This is useful when adding a new light that should have the same configuration (color, brightness) as an existing one.

### 3.2 Structural Patterns

#### Facade
- **Usage:** `HomeController`
- **Reasoning:** Provides a simplified interface to the complex subsystem of devices. The `Menu` interacts only with `HomeController`, which handles the coordination between `Light`, `TV`, `SoundSystem`, etc.

#### Chain of Responsibility
- **Usage:** `SecuritySystem` (AlarmHandler -> LightHandler -> PoliceHandler)
- **Reasoning:** Decouples the sender of a request (Motion Detected) from the receivers. The signal passes through a chain where the Alarm rings, then Lights turn on, then Police is notified. This allows dynamic reordering or adding new steps without modifying the core logic.

### 3.3 Behavioral Patterns

#### State
- **Usage:** `ModeManager` (NormalMode, PartyMode, etc.)
- **Reasoning:** Allows the system to alter its behavior when its internal state changes. For example, in "Party Mode", turning on the system automatically plays music and adjusts lights, whereas in "Sleep Mode", it ensures quietness.

#### Observer
- **Usage:** `NotificationSystem` (Subject) and `IDeviceObserver` (Observer)
- **Reasoning:** Defines a subscription mechanism to notify multiple objects about any events that happen to the device (e.g., a bulb burning out triggers a notification to the user app and the maintenance log).

#### Memento
- **Usage:** `StateManager`, `HomeMemento`
- **Reasoning:** Captures and externalizes the object's internal state so that it can be restored later. This powers the "Undo" feature, allowing users to revert the home to a previous configuration.

#### Strategy
- **Usage:** `NotificationStrategy`
- **Reasoning:** Defines a family of algorithms (Email Notification, SMS Notification, App Alert) and makes them interchangeable. The user can switch notification methods at runtime.

#### Template Method
- **Usage:** `Device::powerOn()`, `Device::powerOff()`
- **Reasoning:** Defines the skeleton of an algorithm in the base class (e.g., check electricity -> perform startup -> log status) but lets subclasses override specific steps (e.g., TV warms up tube, Light just glows).

---

## 4. Conclusion
The My Sweet Home project successfully demonstrates how design patterns can be applied to create flexible, maintainable, and scalable software architectures. By decoupling components through patterns like Observer, Facade, and Chain of Responsibility, the system can easily evolve—adding new device types or security protocols—without breaking existing functionality.