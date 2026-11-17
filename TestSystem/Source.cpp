#include "TestingSystem.h"

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    setcolor(White, Blue);

    TestingSystem system;
    system.run();

    setcolor(White, Blue);

    return 0;
}