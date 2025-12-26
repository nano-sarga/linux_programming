#include "shared_array.h"
#include <iostream>
#include <unistd.h>

int main()
{
    shared_array arr("demo_array", 5);

    while (true)
    {
        arr.acquire();

        std::cout << "[SECOND] ";
        for (size_t i = 0; i < arr.size(); ++i)
        {
            arr[i] += 10;
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;

        arr.release();

        sleep(2);
    }
}

