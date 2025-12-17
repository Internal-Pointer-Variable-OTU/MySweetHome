#ifndef MENU_H
#define MENU_H

#include <string>

// Command Pattern - Menu commands
class MenuCommand {
public:
    virtual ~MenuCommand() {}
    virtual void execute() = 0;
    virtual std::string getDescription() const = 0;
};

// Menu display and input handling
class Menu {
private:
    static const int MENU_WIDTH = 70;
    
    void printLine(char c = '=') const;
    void printCentered(const std::string& text) const;

public:
    Menu();
    ~Menu();
    
    void displayMainMenu() const;
    void displayManual() const;
    void displayAbout() const;
    void displayAddDeviceSubmenu() const;
    void displayRemoveDeviceSubmenu() const;
    void displayPowerOnSubmenu() const;
    void displayPowerOffSubmenu() const;
    void displayModeSubmenu() const;
    void displayStateSubmenu() const;
    
    int getMenuChoice() const;
    char getCharChoice() const;
    int getNumberInput() const;
    
    void clearScreen() const;
    void waitForKey() const;
    void displayMessage(const std::string& msg) const;
    void displayError(const std::string& err) const;
    void displaySuccess(const std::string& msg) const;
};

#endif // MENU_H
