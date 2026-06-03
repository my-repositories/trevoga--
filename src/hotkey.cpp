#include <iostream>
#include <windows.h>

#include <config_manager.hpp>
#include <hotkey.hpp>
#include <process_utils.hpp>

static std::atomic<bool> g_Running{true};

static BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT) {
        std::cout << "\nЗавершение работы..." << std::endl;
        g_Running = false;
        PostThreadMessage(GetCurrentThreadId(), WM_USER, 0, 0);
        ExitProcess(0);
        return TRUE;
    }
    return FALSE;
}

void StartHotkeyListener() {
    if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
        std::cerr << "[Ошибка] Не удалось установить обработчик CTRL+C!" << std::endl;
        return;
    }

    Config config = LoadConfig("config.json");
    if (config.actions.empty()) {
        std::cerr << "[Ошибка] Конфиг пуст или не найден config.json в текущей папке." << std::endl;
        return;
    }

    std::cout << "Загружено хоткеев из конфига: " << config.actions.size() << std::endl;
    
    for (size_t i = 0; i < config.actions.size(); ++i) {
        const auto& action = config.actions[i];
        int hotkeyId = static_cast<int>(i + 1);

        if (RegisterHotKey(NULL, hotkeyId, action.modifiers, action.vkCode)) {
            std::cout << " -> Зарегистрирован хоткей: " << action.rawHotkey << std::endl;
        } else {
            std::cerr << "[Ошибка] Не удалось зарегистрировать: " << action.rawHotkey << std::endl;
        }
    }

    std::cout << "Для выхода нажмите CTRL + C в этом окне." << std::endl;

    MSG msg = {0};
    while (g_Running && GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            int targetIndex = msg.wParam - 1;
            
            if (targetIndex >= 0 && targetIndex < static_cast<int>(config.actions.size())) {
                const auto& triggeredAction = config.actions[targetIndex];
                
                std::cout << "\n[Хоткей] Нажата комбинация " << triggeredAction.rawHotkey << std::endl;
                
                for (const auto& processName : triggeredAction.processesToKill) {
                    KillProcessByName(processName);
                }
            }
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    for (size_t i = 0; i < config.actions.size(); ++i) {
        UnregisterHotKey(NULL, static_cast<int>(i + 1));
    }

    std::cout << "Ресурсы очищены. Программа завершена." << std::endl;
}
