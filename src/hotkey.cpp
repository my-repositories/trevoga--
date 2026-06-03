#include <iostream>
#include <windows.h>

#include <hotkey.hpp>
#include <process_utils.hpp>

void StartHotkeyListener() {
    std::cout << "Программа запущена. Нажмите CTRL + ALT + K для вывода 42." << std::endl;
    std::cout << "Для выхода нажмите CTRL + C в этом окне." << std::endl;

    if (!RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT, 0x4B)) {
        std::cerr << "Ошибка: Не удалось зарегистрировать хоткей CTRL+ALT+K!" << std::endl;
        return;
    }

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY && msg.wParam == 1) {
            std::cout << "Хоткей нажат. Инициирую закрытие калькулятора..." << std::endl;
            KillProcessByName(L"CalculatorApp.exe");
        }
    }

    UnregisterHotKey(NULL, 1);
}
