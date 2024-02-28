#include <iostream>
#include <thread> 
#include <execution>
#include <chrono>
#include <Windows.h>
#include <locale>
#include <atomic>
#include <mutex>
#include <cstdlib>



std::mutex mt;

void progress_bar(int num, int length) {
 
    float progress = 0.0;
    COORD coord;
 
    auto start = std::chrono::steady_clock::now();

    while (progress <= 1.0)
    {
        int bar = 50;
        std::this_thread::sleep_for(std::chrono::milliseconds(length));
        mt.lock();

        coord.X = 0;
        coord.Y = num + 2;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        std::cout << num << "\t" << std::this_thread::get_id() << "\t";
        int pos = bar * progress;

        for (int i = 0; i < bar; ++i) {
            if (i <= pos) {
                std::cout << char(219);
            }
         
        }
        std::cout << " ";
        mt.unlock();
        progress += 0.01;
    }

    auto end = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double time = static_cast<double>(diff) / 1000000;
    mt.lock();
    std::cout << "  " << time << "s";
    mt.unlock();
}


int main()
{
    std::cout << "#\t id\t\ Progress bar \t\t\t\t\t time" << std::endl;
    
    int num = 4;
    std::thread* th = new std::thread[num];

   
        for (int i = 0; i < num; i++) {
            th[i] = std::thread(progress_bar, i, i * 20);
        }
        for (int i = 0; i < num; i++) {
            th[i].join();
        }

        std::cout << std::endl;
    }
   


