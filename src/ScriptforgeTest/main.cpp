#include <iostream>
#include "Scriptforge.hpp"

// 测试头文件包含
#ifndef SCRIPTFORGE_HPP
#error "Scriptforge.hpp not found!"
#endif

int main() {
    std::cout << "Scriptforge version test" << std::endl;

    // 如果这行编译成功，说明头文件找到了
    // 但如果链接失败，说明实现文件有问题
    std::cout << Scriptforge::Time::format_now_local() << std::endl;

    return 0;
}
