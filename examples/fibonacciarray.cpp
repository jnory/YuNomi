#include <iostream>

#include "yunomi/array/fibonacciarray.hpp"

int main(void){
    // create a data storage
    yunomi::array::FibonacciDataStorage storage;

    // set values (acceptable values are larger than zero)
    for(uint64_t i = 0; i < 1024; ++i){
        storage.push_back(i + 1);
    }

    // create an array object from the storage
    yunomi::array::FibonacciArray fa(storage);

    // get values
    std::cout << "FibonacciArray: array[10]=" << fa[10] << std::endl;
    std::cout << "FibonacciArray: array[20]=" << fa[20] << std::endl;

    return 0;
}