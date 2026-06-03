#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include <string>
#include <vector>
#include <windows.h>

struct Action {
    std::string rawHotkey;
    std::vector<std::string> rawCommands;
    
    UINT modifiers = 0;
    UINT vkCode = 0;
    std::vector<std::wstring> processesToKill; 
};

struct Config {
    bool showConsole = true;
    std::vector<Action> actions;
};

Config LoadConfig(const std::string& filePath);

#endif // CONFIG_MANAGER_HPP
