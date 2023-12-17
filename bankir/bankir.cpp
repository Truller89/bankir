#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
size_t amountOfCompletedThreads = 0;
std::mutex mute;

struct resources {
    int a;
    int b;
    int c;
};

void loadResources(resources& bank, int a, int b, int c, int time, int number) {
    std::cout << "Поток #" << number << " получает ресурсы." << std::endl;
    while (true) { 
        mute.lock();
        Sleep(1000);
        if (a <= bank.a and b <= bank.b and c <= bank.c) {
            bank.a = bank.a - a;
            bank.b = bank.b - b;
            bank.c = bank.c - c;
            std::cout << "Поток #" << number << " получил ресурсы." << "(Остаток:" << bank.a << "; " << bank.b << "; " << bank.c << ")" << std::endl;
            mute.unlock();
            Sleep(time * 1000);
            bank.a = bank.a + a;
            bank.b = bank.b + b;
            bank.c = bank.c + c;
            break;
        }
        mute.unlock();
    }
    std::cout << "Поток #" << number << " освободил ресурсы." << "(Остаток:" << bank.a << "; " << bank.b << "; " << bank.c << ")" << std::endl;
    ++amountOfCompletedThreads;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    resources bank = { 28, 21, 12 };
    std::cout << "Введите количество потоков: ";
    int n;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        int a, b, c, t;
        std::cout << "Введите ресурс а: " << std::endl;
        std::cin >> a;
        std::cout << "Введите ресурс б: " << std::endl;
        std::cin >> b;
        std::cout << "Введите ресурс с: " << std::endl;
        std::cin >> c;
        std::cout << "Введите время: " << std::endl;
        std::cin >> t;
        std::thread th(loadResources, std::ref(bank), a, b, c, t, i);
        th.detach();
    }

    while (amountOfCompletedThreads < n) {};
}
