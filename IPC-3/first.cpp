#include "shared_array.h"
#include <iostream>
#include <unistd.h>

int main()
{
    shared_array arr("demo_array", 5);
    int counter = 0;

    while (true)
    {
        arr.acquire();

        for (size_t i = 0; i < arr.size(); ++i)
            arr[i] = counter + i;

        std::cout << "[FIRST] ";
        for (size_t i = 0; i < arr.size(); ++i)
            std::cout << arr[i] << " ";
        std::cout << std::endl;

        arr.release();

        ++counter;
        sleep(1);
    }
}

