#include <iostream>
#include <limits>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;
// std::chrono::duration表示一段时间 可通过duration_cast进行类型转换
// std::chrono::time_point表示一个具体时间
// std::chrono::system_clock表示当前的系统时钟
int main() {
    std::chrono::milliseconds mill1(10);
    std::chrono::duration<int,std::ratio<1,1000>> mill2(10); // 等效std::chrono::milliseconds
    cout << mill1.count() << " " << mill2.count() << endl;

    std::chrono::steady_clock::time_point time1 = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); //休眠10ms
    std::this_thread::sleep_for(std::chrono::duration<double,std::ratio<1,1000>>(10.5)); //休眠10.5ms
    std::chrono::steady_clock::time_point time2 = std::chrono::steady_clock::now();
    std::chrono::duration<double,std::ratio<1,1000>> detaT = std::chrono::duration_cast<std::chrono::
    duration<double,std::ratio<1,1000>>>(time2-time1);
    cout << detaT.count() << endl;
    return 0;
}
