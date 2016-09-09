#include <iostream>

#include "yunomi/array/fibonacciarray.hpp"

int main(void){
    // create an array of 1024 bits.
    yunomi::array::BitSliceArray array(100);

    // set values
    for(uint64_t i = 0; i < 10; ++i){
        array(i * 10, (i + 1) * 10) = i;
    }

    // filter that passes only "11" position.
    yunomi::array::Filter11Array f11(array);

    // get values
    for(uint64_t i = 0; i < 10; ++i) {
        std::cout << "f11[" << i << "] = " << std::hex << f11(i * 10, (i + 1) * 10) << std::dec << std::endl;
    }

    return 0;
}