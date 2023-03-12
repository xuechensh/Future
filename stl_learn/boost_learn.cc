#include <boost/progress.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace boost;
using namespace std;

int main()
{
    progress_display p_d(100); // - 创建并设置总进度值，自动初始化起始值为0

    for (int i = 0; i < 100; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(10));
        ++p_d; // - 增加进度值，一次只能增加一个
    }
    cout << "执行完毕！" << endl;
    return 0;
}