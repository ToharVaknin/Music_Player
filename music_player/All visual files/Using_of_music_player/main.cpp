#include"UserInterface.h"

void test();

int main()
{
    test();
    std::cout << std::endl;
    std::cout << "Leaks: " << _CrtDumpMemoryLeaks() << std::endl;
    return 0;
}

void test()
{
    UserInterface user;
    user.Play();
}