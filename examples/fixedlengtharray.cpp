#include <iostream>

#include "yunomi/array/fixedlengtharray.hpp"

void example_fixedlengtharray(){
    // create an array. length is 100, each slot is made of 10 bits.
    yunomi::array::FixedLengthArray array(100, 10);

    // set values
    for(uint64_t i = 0; i < 100; ++i){
        array[i] = i;
    }

    // get values
    std::cout << "FixedLengthArray: array[10]=" << array[10] << std::endl;
    std::cout << "FixedLengthArray: array[20]=" << array[20] << std::endl;
}

void example_constlengtharray(){
    // create an array. length is 100, each slot is made of 10 bits.
    yunomi::array::ConstLengthArray<10> array(100);

    // set values
    for(uint64_t i = 0; i < 100; ++i){
        array[i] = i;
    }

    // get values
    std::cout << "ConstLengthArray: array[10]=" << array[10] << std::endl;
    std::cout << "ConstLengthArray: array[20]=" << array[20] << std::endl;
}

int main(void){
    example_fixedlengtharray();
    example_constlengtharray();

    return 0;
}
