#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include <config_manager.hpp>

using json = nlohmann::json;

void ParseHotkeyStr(const std::string& str, UINT& modifiers, UINT& vkCode) {
    modifiers = MOD_NOREPEAT;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, '+')) {
        if (token == "CTRL")  modifiers |= MOD_CONTROL;
        else if (token == "ALT")   modifiers |= MOD_ALT;
        else if (token == "SHIFT") modifiers |= MOD_SHIFT;
        else if (token == "WIN")   modifiers |= MOD_WIN;
        else if (token.length() == 1) {
            vkCode = token[0]; 
        }
    }
}

std::wstring ParseCommand(const std::string& cmd) {
    std::string prefix = "KillProcessByName ";
    if (cmd.rfind(prefix, 0) == 0) {
        std::string procName = cmd.substr(prefix.length());
        return std::wstring(procName.begin(), procName.end());
    }
    return L"";
}

Config LoadConfig(const std::string& filePath) {
    Config config;
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "[Ошибка] Не удалось открыть файл конфигурации: " << filePath << std::endl;
        return config;
    }

    try {
        json j;
        file >> j;

        config.showConsole = j.value("show_console", true);

        if (j.contains("actions") && j["actions"].is_array()) {
            for (const auto& item : j["actions"]) {
                Action action;
                action.rawHotkey = item.value("hotkey", "");

                ParseHotkeyStr(action.rawHotkey, action.modifiers, action.vkCode);

                if (item.contains("commands") && item["commands"].is_array()) {
                    for (const auto& cmdJson : item["commands"]) {
                        std::string cmdStr = cmdJson.get<std::string>();
                        action.rawCommands.push_back(cmdStr);

                        std::wstring procName = ParseCommand(cmdStr);
                        if (!procName.empty()) {
                            action.processesToKill.push_back(procName);
                        }
                    }
                }

                if (action.vkCode != 0 && !action.processesToKill.empty()) {
                    config.actions.push_back(action);
                }
            }
        }
    } catch (const json::exception& e) {
        std::cerr << "[Ошибка JSON] " << e.what() << std::endl;
    }

    return config;
}
