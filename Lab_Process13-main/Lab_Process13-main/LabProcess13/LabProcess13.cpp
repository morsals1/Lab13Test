#include <windows.h>
#include <conio.h>
#include <iostream>

int main() {
    setlocale(LC_ALL, "Rus");

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // Имя канала
    const wchar_t* pipeName = L"\\\\.\\pipe\\MyPipe";

    // Создаем именованный канал
    HANDLE hPipe = CreateNamedPipe(
        pipeName,                       // Имя канала
        PIPE_ACCESS_DUPLEX,             // Двусторонний доступ
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,       // Максимальное количество экземпляров
        1024,                           // Размер выходного буфера
        1024,                           // Размер входного буфера
        0,                              // Таймаут по умолчанию
        NULL                            // Атрибуты безопасности по умолчанию
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Ошибка создания канала: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Канал создан. Ожидание подключения дочернего процесса...\n";

    // Ожидаем подключения дочернего процесса
    if (!ConnectNamedPipe(hPipe, NULL)) {
        std::cerr << "Ошибка подключения к каналу: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return 1;
    }

    std::cout << "Дочерний процесс подключен.\n";

    // Запускаем дочерний процесс
    wchar_t cmdline[] = L"C:\\Users\\st310-11\\Desktop\\меньшиков ПР-32\\Lab_Process13-main\\Lab_Process13child\\Lab13Child\\x64\\Debug\\Lab13Child.exe";

    if (!CreateProcess(NULL, cmdline, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cerr << "Ошибка создания процесса: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return 1;
    }

    std::cout << "Дочерний процесс создан.\n";

    // Отправка команд через канал
    while (true) {
        std::cout << "1. Создать новый поток\n";
        std::cout << "2. Выйти\n";
        std::cout << "Выберите действие: ";

        wchar_t choice[100];
        _getws_s(choice);

        if (wcscmp(choice, L"1") == 0) {
            // Отправляем команду "create"
            if (!WriteFile(hPipe, L"create", (wcslen(L"create") + 1) * sizeof(wchar_t), NULL, NULL)) {
                std::cerr << "Ошибка отправки команды: " << GetLastError() << std::endl;
                break;
            }
            std::cout << "Команда 'create' отправлена.\n";
        } else if (wcscmp(choice, L"2") == 0) {
            // Отправляем команду "exit"
            if (!WriteFile(hPipe, L"exit", (wcslen(L"exit") + 1) * sizeof(wchar_t), NULL, NULL)) {
                std::cerr << "Ошибка отправки команды: " << GetLastError() << std::endl;
                break;
            }
            std::cout << "Команда 'exit' отправлена.\n";
            break;
        } else {
            std::cout << "Неверный выбор.\n";
        }
    }

    // Закрываем канал и завершаем процесс
    CloseHandle(hPipe);
    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::cout << "Дочерний процесс завершен. Нажмите любую клавишу для выхода.\n";
    _getch();

    return 0;
}
