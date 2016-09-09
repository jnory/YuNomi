#include <iostream>

#include "yunomi/array/bitslicearray.hpp"

int main(void){
    // create an array of 1024 bits.
    yunomi::array::BitSliceArray array(1024);

    // set values
    array(0, 10) = 1023;
    array(10, 20) = 1000;

    // get values
    std::cout << "[0, 10] = " << array(0, 10) << std::endl;
    std::cout << "[10, 20] = " << array(10, 20) << std::endl;

    return 0;
}