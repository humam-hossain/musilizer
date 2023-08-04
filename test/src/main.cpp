#include <iostream>
#include <dlfcn.h>
#include "math_operations.h"

int main() {
    void* mathLib = dlopen("./libmath_operations.so", RTLD_LAZY);
    if (!mathLib) {
        std::cerr << "Error loading library: " << dlerror() << std::endl;
        return 1;
    }

    using MathFunc = void(*)();
    MathFunc hello = reinterpret_cast<MathFunc>(dlsym(mathLib, "hello"));

    if (!hello) {
        std::cerr << "Error loading symbols: " << dlerror() << std::endl;
        dlclose(mathLib);
        return 1;
    }

    int x = 10, y = 5;

    hello();

    dlclose(mathLib);
    return 0;
}
