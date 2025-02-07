#include <iostream>
#include <functional>  // 引入 std::function

// 定义回调函数类型
typedef std::function<void()> Callback;

// 定义一个类，封装调用回调函数的逻辑
class FunctionSelector {
public:
    // 接受回调函数作为参数
    void callFunction(int choice, Callback callback) {
        std::cout << "User selected choice: " << choice << std::endl;
        // 执行回调函数
        callback();
    }
};

// 不同的功能函数
void func1() {
    std::cout << "Function 1 called!" << std::endl;
}

void func2() {
    std::cout << "Function 2 called!" << std::endl;
}

void func3() {
    std::cout << "Function 3 called!" << std::endl;
}

int main() {
    FunctionSelector selector;
    int choice;

    // 提示用户输入选择
    std::cout << "Enter a number (1-3) to call a function: ";
    std::cin >> choice;

    // 根据用户输入的选项，选择并传递回调函数
    switch (choice) {
        case 1:
            selector.callFunction(choice, func1);
            break;
        case 2:
            selector.callFunction(choice, func2);
            break;
        case 3:
            selector.callFunction(choice, func3);
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            break;
    }

    return 0;
}
