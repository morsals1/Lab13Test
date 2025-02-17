#include <windows.h>
#include <iostream>
#include <conio.h>
#include <vector>


DWORD WINAPI ThreadFunc(LPVOID param) {
    int id = (int)(uintptr_t)param;
    std::cout << "Поток " << id << " начал выполнение.\n";

    srand(static_cast<unsigned int>(time(nullptr)) + id);

    while (true) {
        int randomNumber = rand();
        std::cout << "Поток " << id << " сгенерировал число: " << randomNumber << "\n";

        Sleep(1000);
    }

    std::cout << "Поток " << id << " завершил выполнение.\n";
    return 0;
}

int main() {

    std::vector<HANDLE> threads;
    int nextThreadId = 1;
    wchar_t cmdline[] = L"C:\\Users\\st310-11\\Desktop\\меньшиков ПР-32\\Lab_Process13-main\\Lab_Process13-main\\LabProcess13\\x64\\Debug\\LabProcess13.exe";

    HANDLE hPipe;
    DWORD  cbWritten;
    DWORD  cbRead;
    wchar_t choice[100];

    hPipe = CreateFile(cmdline, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hPipe == NULL) {
        std::cerr << "Ошибка создания pipe: " << GetLastError() << std::endl;
        return 1;
    }

    while (true) {
        std::cout << choice ;

        if (ReadFile(hPipe, choice, 1024, &cbRead, NULL)) {
            HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID)(uintptr_t)nextThreadId++, 0, NULL);
            if (hThread == NULL) {
                std::cerr << "Ошибка создания потока\n";
            }
            else {
                threads.push_back(hThread);
                std::cout << "Поток " << nextThreadId - 1 << " создан.\n";
            }
        }
        else if (ReadFile(hPipe, choice, 1024, &cbRead, NULL)) {
            break;
        }
        else {
            std::cout << "Неверный выбор.\n";
        }
    }

    for (size_t i = 0; i < threads.size(); ++i) {
        TerminateThread(threads[i], 0);
        CloseHandle(threads[i]);
    }

    std::cout << "Все потоки завершены. Нажмите любую клавишу для выхода.\n";
    _getch();

    return 0;
}