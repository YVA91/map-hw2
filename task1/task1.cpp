#include <iostream>
#include <thread> 
#include <execution>
#include <chrono>
#include <Windows.h>
#include <locale>
#include <atomic>


using namespace std::chrono_literals;

void input(std::atomic<int> &x, int max) {

    while (x < max)
    {
        std::this_thread::sleep_for(1s);

        std::atomic_fetch_add(&x, 1);
       
        std::cout << "Людей в очереди: " << x << std::endl;
    }

}

void output(std::atomic<int> &x) {

    while (true) {
        std::this_thread::sleep_for(2s);
        std::atomic_fetch_add(&x, -1);
        std::cout << "Людей в очереди осталось: " << x << std::endl;
    }
}



int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::atomic<int> x = 0;
    int max = 10;

    std::thread th1(input, std::ref(x), max);
    std::thread th2(output, std::ref(x));

    th1.join();
    th2.join();

}