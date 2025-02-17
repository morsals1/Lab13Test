#include <windows.h>
#include <conio.h>
#include <iostream>

int main() {


    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;


    wchar_t choice[100];
    BOOL fConnected;
    HANDLE hPipe;
    DWORD  cbRead;
    DWORD  cbWritten;

    wchar_t cmdline[] = L"C:\\Users\\st310-11\\Desktop\\меньшиков ПР-32\\Lab_Process13-main\\Lab_Process13child\\Lab13Child\\x64\\Debug\\Lab13Child.exe";

    hPipe = CreateNamedPipe(cmdline, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES,
        1024, 1024, 5000, NULL);

    if (hPipe == NULL) {
        std::cerr << "Ошибка создания pipe: " << GetLastError() << std::endl;
        return 1;
    }

    fConnected = ConnectNamedPipe(hPipe, NULL);

    while (true) {
        std::cout << "1. Создать новый поток\n";
        std::cout << "2. Выйти\n";
        std::cout << "Выберите действие: ";

        _getws_s(choice);
        _getch();

        if (choice == L"1") {
            if (!WriteFile(hPipe, choice, wcslen(choice) + 1, &cbWritten, NULL))
                break;
        }
        else if (choice == L"2") {
            if (!WriteFile(hPipe, choice, wcslen(choice) + 1, &cbWritten, NULL))
                break;
        }
        else {
            std::cout << "Неверный выбор.\n";
        }
    }

    if (!CreateProcess(NULL, cmdline, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cerr << "Ошибка создания процесса: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "процесс создан\n";

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::cout << "процесс завершен. Нажмите любую клавишу для выхода.\n";
    _getch();

    return 0;
}