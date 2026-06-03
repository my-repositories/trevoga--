#include "process_utils.hpp"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

void KillProcessByName(std::wstring_view processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "[Ошибка] Не удалось сделать снимок процессов." << std::endl;
        return;
    }

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnapshot, &pe)) {
        do {
            if (processName == pe.szExeFile) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                if (hProcess != NULL) {
                    if (TerminateProcess(hProcess, 0)) {
                        std::cout << "[Успех] Процесс " << pe.th32ProcessID << " (" 
                                  << "CalculatorApp.exe) успешно закрыт." << std::endl;
                    } else {
                        std::cerr << "[Ошибка] Не удалось закрыть процесс. Код: " << GetLastError() << std::endl;
                    }
                    CloseHandle(hProcess);
                } else {
                    std::cerr << "[Ошибка] Нет доступа для закрытия процесса. Код: " << GetLastError() << std::endl;
                }
            }
        } while (Process32NextW(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
}
