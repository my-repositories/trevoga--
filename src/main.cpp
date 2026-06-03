#include <consoleapi2.h>
#include <iostream>

#include <config_manager.hpp>
#include <hotkey.hpp>

int main() {
    Config config = LoadConfig("config.json");
    if (config.actions.empty()) {
        std::cerr << "[Ошибка] Конфиг пуст или не найден config.json в текущей папке." << std::endl;
        return 1;
    }

    if (!config.showConsole) {
        FreeConsole();
    } else {
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    }
    
    StartHotkeyListener(config);
    return 0;
}
