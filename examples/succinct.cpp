#include <iostream>

#include "yunomi/array/bitslicearray.hpp"
#include "yunomi/array/fixedlengtharray.hpp"
#include "yunomi/succinct/dual_block_index.hpp"
#include "yunomi/succinct/rank.hpp"
#include "yunomi/succinct/select.hpp"

int main(void){
    // create an array of 100 bits
    yunomi::array::BitSliceArray array(100);

    // set values
    for(uint64_t i = 0; i < 10; ++i){
        array(i * 10, (i + 1) * 10) = i;
    }

    // create an index for rank/select query
    yunomi::succinct::DualBlockIndex< yunomi::array::ConstLengthArray<16>, yunomi::array::ConstLengthArray<8> > index(array);

    // create rank/select objects
    yunomi::succinct::Rank< decltype(index) > rank(index);
    yunomi::succinct::Select< decltype(index) > select(index);

    // query rank
    std::cerr << "rank(0)=" << rank(0) << std::endl;
    std::cerr << "rank(10)=" << rank(10) << std::endl;
    std::cerr << "rank(20)=" << rank(20) << std::endl;

    // query select
    std::cerr << "select(0)=" << select(0) << std::endl;
    std::cerr << "select(1)=" << select(1) << std::endl;
    std::cerr << "select(10)=" << select(10) << std::endl;

    return 0;
}