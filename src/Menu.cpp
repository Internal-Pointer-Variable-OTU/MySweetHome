#include "Menu.h"
#include <iostream>
#include <iomanip>
#include <cstdio>

Menu::Menu() {}

Menu::~Menu() {}

void Menu::printLine(char c) const {
    for (int i = 0; i < MENU_WIDTH; ++i) {
        std::cout << c;
    }
    std::cout << std::endl;
}

void Menu::printCentered(const std::string& text) const {
    int padding = (MENU_WIDTH - text.length()) / 2;
    if (padding < 0) padding = 0;
    for (int i = 0; i < padding; ++i) {
        std::cout << ' ';
    }
    std::cout << text << std::endl;
}

void Menu::displayMainMenu() const {
    std::cout << std::endl;
    printLine('=');
    printCentered("MY SWEET HOME (MSH)");
    printLine('=');
    std::cout << std::endl;
    
    std::cout << "  [1] Get Home Status" << std::endl;
    std::cout << "      (Show state, mode, sensors and actuators info)" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  [2] Add Device" << std::endl;
    std::cout << "      (L)ight, Smoke & Gas (D)etectors, (C)amera, (T)V, (S)ound System" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  [3] Remove Device" << std::endl;
    std::cout << "      (L)ight, Smoke & Gas (D)etectors, (C)amera, (T)V, (S)ound System" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  [4] Power On Device" << std::endl;
    std::cout << "      (L)ight, Smoke & Gas (D)etectors, (C)amera, (T)V, (S)ound System" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  [5] Power Off Device" << std::endl;
    std::cout << "      (L)ight, Smoke & Gas (D)etectors, (C)amera, (T)V, (S)ound System" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  [6] Change Mode" << std::endl;
    std::cout << "      (N)ormal, (E)vening, (P)arty, (C)inema" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  [7] Change State" << std::endl;
    std::cout << "      (N)ormal, (H)igh Performance, (L)ow Power, (S)leep, (P)revious" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  [8] Manual (Display user manual)" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  [9] About (Product and developer info)" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  [10] Shutdown (Shut down the system)" << std::endl;
    std::cout << std::endl;
    
    printLine('-');
    std::cout << "  Enter your choice: ";
}

void Menu::displayManual() const {
    clearScreen();
    printLine('=');
    printCentered("MY SWEET HOME - USER MANUAL");
    printLine('=');
    std::cout << std::endl;
    
    std::cout << "  1. HOME STATUS" << std::endl;
    std::cout << "     View current system state, mode, and all connected devices." << std::endl;
    std::cout << std::endl;
    
    std::cout << "  2. ADD DEVICE" << std::endl;
    std::cout << "     Add new devices to the system. Specify device type and quantity." << std::endl;
    std::cout << "     Configuration from existing devices can be copied to new ones." << std::endl;
    std::cout << std::endl;
    
    std::cout << "  3. REMOVE DEVICE" << std::endl;
    std::cout << "     Remove devices from the system by selecting type and index." << std::endl;
    std::cout << std::endl;
    
    std::cout << "  4. POWER ON DEVICE" << std::endl;
    std::cout << "     Turn on specific devices. Note: Detectors are always on." << std::endl;
    std::cout << std::endl;
    
    std::cout << "  5. POWER OFF DEVICE" << std::endl;
    std::cout << "     Turn off specific devices. Detectors and Alarm cannot be turned off." << std::endl;
    std::cout << std::endl;
    
    std::cout << "  6. CHANGE MODE" << std::endl;
    std::cout << "     Switch between operating modes:" << std::endl;
    std::cout << "     - Normal: Lights ON, TV OFF, Music OFF" << std::endl;
    std::cout << "     - Evening: Lights OFF, TV OFF, Music OFF" << std::endl;
    std::cout << "     - Party: Lights ON, TV OFF, Music ON" << std::endl;
    std::cout << "     - Cinema: Lights OFF, TV ON, Music OFF" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  7. CHANGE STATE" << std::endl;
    std::cout << "     Switch between system states:" << std::endl;
    std::cout << "     - Normal: Standard operation" << std::endl;
    std::cout << "     - High Performance: Maximum responsiveness" << std::endl;
    std::cout << "     - Low Power: Energy saving mode" << std::endl;
    std::cout << "     - Sleep: Minimal operation" << std::endl;
    std::cout << "     - Previous: Restore previous state from history" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  8. SECURITY SYSTEM" << std::endl;
    std::cout << "     When motion is detected: Alarm -> Lights On -> Call Police" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  9. DETECTION SYSTEM" << std::endl;
    std::cout << "     When smoke/gas detected: Alarm -> Blink Lights -> Call Fire Station" << std::endl;
    std::cout << "     User can acknowledge to interrupt the sequence." << std::endl;
    std::cout << std::endl;
    
    printLine('=');
}

void Menu::displayAbout() const {
    clearScreen();
    printLine('=');
    printCentered("ABOUT MY SWEET HOME");
    printLine('=');
    std::cout << std::endl;
    
    printCentered("Version 1.0.0");
    std::cout << std::endl;
    
    std::cout << "  Product: My Sweet Home (MSH)" << std::endl;
    std::cout << "  Description: Smart Home Management System" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  Course: CENG-464 Design Patterns with C++" << std::endl;
    std::cout << "  Instructor: Murat Yilmaz" << std::endl;
    std::cout << "  University: OSTIM Technical University" << std::endl;
    std::cout << std::endl;
    
    printLine('-');
    std::cout << "  Development Team:" << std::endl;
    printLine('-');
    std::cout << "  [Integrator]  - Anıl Akpınar - Integration & Build System" << std::endl;
    std::cout << "  [Developer 1] - Ömer Faruk Çakmak - Device Core & Factories" << std::endl;
    std::cout << "  [Developer 2] - Mehmet Efe Ergin - Actuators & Prototype" << std::endl;
    std::cout << "  [Developer 3] - Zeynep Bilge Tuğ - Detectors & Alarm" << std::endl;
    std::cout << "  [Developer 4] - Hüseyin Saçıkay - Storage & Menu" << std::endl;
    std::cout << "  [Developer 5] - Yusuf Berk Baytok - Mode System" << std::endl;
    std::cout << "  [Developer 6] - Yunis Ibrahimov - State History" << std::endl;
    std::cout << "  [Developer 7] - Burak Yıldırım - Security System" << std::endl;
    std::cout << "  [Developer 8] - Dünyamalı Goyushlu - Detection System" << std::endl;
    std::cout << std::endl;
    
    printLine('-');
    std::cout << "  Design Patterns Used:" << std::endl;
    printLine('-');
    std::cout << "  - Singleton (Alarm, Storage)" << std::endl;
    std::cout << "  - Factory Method (SimpleDeviceFactory)" << std::endl;
    std::cout << "  - Abstract Factory (DeviceFactory, DetectorFactory)" << std::endl;
    std::cout << "  - Prototype (Device cloning)" << std::endl;
    std::cout << "  - State (Mode states, System states)" << std::endl;
    std::cout << "  - Memento (State history)" << std::endl;
    std::cout << "  - Observer (Device failure notifications)" << std::endl;
    std::cout << "  - Strategy (Notification strategies)" << std::endl;
    std::cout << "  - Chain of Responsibility (Security/Detection sequences)" << std::endl;
    std::cout << "  - Template Method (Device power on/off)" << std::endl;
    std::cout << "  - Facade (HomeController, Security/Detection systems)" << std::endl;
    std::cout << std::endl;
    
    printLine('=');
}

void Menu::displayAddDeviceSubmenu() const {
    std::cout << std::endl;
    printLine('-');
    std::cout << "  ADD DEVICE SUBMENU" << std::endl;
    printLine('-');
    std::cout << "  Select device type:" << std::endl;
    std::cout << "  (L) Light" << std::endl;
    std::cout << "  (D) Smoke & Gas Detectors" << std::endl;
    std::cout << "  (C) Camera" << std::endl;
    std::cout << "  (T) TV" << std::endl;
    std::cout << "  (S) Sound System" << std::endl;
    std::cout << "  (Q) Cancel" << std::endl;
    std::cout << std::endl;
    std::cout << "  Enter choice: ";
}

void Menu::displayRemoveDeviceSubmenu() const {
    std::cout << std::endl;
    printLine('-');
    std::cout << "  REMOVE DEVICE SUBMENU" << std::endl;
    printLine('-');
    std::cout << "  Select device type:" << std::endl;
    std::cout << "  (L) Light" << std::endl;
    std::cout << "  (D) Smoke & Gas Detectors" << std::endl;
    std::cout << "  (C) Camera" << std::endl;
    std::cout << "  (T) TV" << std::endl;
    std::cout << "  (S) Sound System" << std::endl;
    std::cout << "  (Q) Cancel" << std::endl;
    std::cout << std::endl;
    std::cout << "  Enter choice: ";
}

void Menu::displayPowerOnSubmenu() const {
    std::cout << std::endl;
    printLine('-');
    std::cout << "  POWER ON DEVICE SUBMENU" << std::endl;
    printLine('-');
    std::cout << "  Select device type:" << std::endl;
    std::cout << "  (L) Light" << std::endl;
    std::cout << "  (D) Detectors (always on)" << std::endl;
    std::cout << "  (C) Camera" << std::endl;
    std::cout << "  (T) TV" << std::endl;
    std::cout << "  (S) Sound System" << std::endl;
    std::cout << "  (A) ALL devices" << std::endl;
    std::cout << "  (Q) Cancel" << std::endl;
    std::cout << std::endl;
    std::cout << "  Enter choice: ";
}

void Menu::displayPowerOffSubmenu() const {
    std::cout << std::endl;
    printLine('-');
    std::cout << "  POWER OFF DEVICE SUBMENU" << std::endl;
    printLine('-');
    std::cout << "  Select device type:" << std::endl;
    std::cout << "  (L) Light" << std::endl;
    std::cout << "  (D) Detectors (cannot be turned off)" << std::endl;
    std::cout << "  (C) Camera" << std::endl;
    std::cout << "  (T) TV" << std::endl;
    std::cout << "  (S) Sound System" << std::endl;
    std::cout << "  (A) ALL devices (except critical)" << std::endl;
    std::cout << "  (Q) Cancel" << std::endl;
    std::cout << std::endl;
    std::cout << "  Enter choice: ";
}

void Menu::displayModeSubmenu() const {
    std::cout << std::endl;
    printLine('-');
    std::cout << "  CHANGE MODE SUBMENU" << std::endl;
    printLine('-');
    std::cout << "  Select mode:" << std::endl;
    std::cout << "  (N) Normal Mode - Light ON, TV OFF, Music OFF" << std::endl;
    std::cout << "  (E) Evening Mode - Light OFF, TV OFF, Music OFF" << std::endl;
    std::cout << "  (P) Party Mode - Light ON, TV OFF, Music ON" << std::endl;
    std::cout << "  (C) Cinema Mode - Light OFF, TV ON, Music OFF" << std::endl;
    std::cout << "  (Q) Cancel" << std::endl;
    std::cout << std::endl;
    std::cout << "  Enter choice: ";
}

void Menu::displayStateSubmenu() const {
    std::cout << std::endl;
    printLine('-');
    std::cout << "  CHANGE STATE SUBMENU" << std::endl;
    printLine('-');
    std::cout << "  Select state:" << std::endl;
    std::cout << "  (N) Normal - Standard operation" << std::endl;
    std::cout << "  (H) High Performance - Maximum performance" << std::endl;
    std::cout << "  (L) Low Power - Energy saving" << std::endl;
    std::cout << "  (S) Sleep - Minimal operation" << std::endl;
    std::cout << "  (P) Previous - Restore previous state" << std::endl;
    std::cout << "  (Q) Cancel" << std::endl;
    std::cout << std::endl;
    std::cout << "  Enter choice: ";
}

int Menu::getMenuChoice() const {
    int choice;
    std::cin >> choice;
    return choice;
}

char Menu::getCharChoice() const {
    char choice;
    std::cin >> choice;
    return choice;
}

int Menu::getNumberInput() const {
    int num;
    std::cin >> num;
    return num;
}

void Menu::clearScreen() const {
    // Cross-platform clear screen
    std::cout << "\033[2J\033[1;1H";
}

void Menu::waitForKey() const {
    std::cout << std::endl;
    std::cout << "  Press ENTER to continue...";
    std::cin.ignore();
    std::cin.get();
}

void Menu::displayMessage(const std::string& msg) const {
    std::cout << std::endl;
    std::cout << "  " << msg << std::endl;
}

void Menu::displayError(const std::string& err) const {
    std::cout << std::endl;
    std::cout << "  [ERROR] " << err << std::endl;
}

void Menu::displaySuccess(const std::string& msg) const {
    std::cout << std::endl;
    std::cout << "  [SUCCESS] " << msg << std::endl;
}