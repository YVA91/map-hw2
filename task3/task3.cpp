#include <iostream>
#include <thread> 
#include <execution>
#include <chrono>
#include <Windows.h>
#include <locale>
#include <atomic>
#include <mutex>

class Data {
public:
    int x;
    std::mutex m;

public:
    Data(int ax) {
        this->x = ax;
    }
    Data() {
        this->x = 0;
    }
};

    void swap_lock (Data &a, Data &b){
        std::lock(a.m, b.m);
        std::lock_guard(a.m, std::adopt_lock);
        Data c;
        c.x = a.x;
        a.x = b.x;
        b.x = c.x;
        std::lock_guard(b.m, std::adopt_lock);
    }

    void swap_scoped_lock(Data& a, Data& b) {
        std::scoped_lock(a.m, b.m);
        Data c;
        c.x = a.x;
        a.x = b.x;
        b.x = c.x;
    }

    void swap_unique_lock(Data& a, Data& b) {
        std::unique_lock mul1(a.m);
        Data c;
        c.x = a.x;
        a.x = b.x;
        std::unique_lock mul2(b.m);
        b.x = c.x;
        mul1.unlock();
        mul2.unlock();
    }

int main()
{
    Data a(1);
    Data b(2);

    Data a1(10);
    Data b1(20);

    Data a2(11);
    Data b2(22);

    std::thread sw_l(swap_lock, std::ref(a), std::ref(b));
    std::thread sw_sl(swap_scoped_lock, std::ref(a1), std::ref(b1));
    std::thread sw_ul(swap_unique_lock, std::ref(a2), std::ref(b2));

    sw_l.join();
    sw_sl.join();
    sw_ul.join();

    std::cout << a.x << std::endl;
    std::cout << b.x << std::endl;
    std::cout << a1.x << std::endl;
    std::cout << b1.x << std::endl;
    std::cout << a2.x << std::endl;
    std::cout << b2.x << std::endl;
}

