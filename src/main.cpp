#include <consoleapi2.h>

#include <hotkey.hpp>

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    
    StartHotkeyListener();
    return 0;
}
