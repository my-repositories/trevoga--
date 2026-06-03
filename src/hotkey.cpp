#include <iostream>
#include <windows.h>

#include <hotkey.hpp>

void StartHotkeyListener() {
    std::cout << "Программа запущена. Нажмите CTRL + ALT + K для вывода 42." << std::endl;
    std::cout << "Для выхода нажмите CTRL + C в этом окне." << std::endl;

    // Регистрация хоткея CTRL+ALT+K (0x4B — код клавиши 'K')
    if (!RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT, 0x4B)) {
        std::cerr << "Ошибка: Не удалось зарегистрировать хоткей CTRL+ALT+K!" << std::endl;
        return;
    }

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY && msg.wParam == 1) {
            std::cout << 42 << std::endl;
        }
    }

    UnregisterHotKey(NULL, 1);
}
