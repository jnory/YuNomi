#include <iostream>
#include "yunomi/io/file.hpp"
#include "yunomi/serialize/hdf5.hpp"
#include "yunomi/array/fibonacciarray.hpp"

int main(void){
    std::string path = "serialize_example.h5";
    if(yunomi::io::File::exists(path)){
        std::cerr << "File already exists." << std::endl;
        exit(1);
    }

    /*
     * Create array objects
     */
    // create a data storage
    yunomi::array::FibonacciDataStorage storage;

    // set values (acceptable values are larger than zero)
    for(uint64_t i = 0; i < 1024; ++i){
        storage.push_back(i + 1);
    }

    // create an array object from the storage
    yunomi::array::FibonacciArray fa(storage);

    /*
     * Serialize objects
     */
    // create serializer
    yunomi::serialize::HDF5Serializer serializer(path);

    // serialize objects
    serializer.serialize("storage", storage);
    serializer.serialize("array", fa);

    // close the serialize object
    serializer.close();

    /*
     * deserialize objects
     */
    // create a data storage to load data
    yunomi::array::FibonacciDataStorage storage2;

    // create an array object from the storage
    // please set false for the second argument.
    yunomi::array::FibonacciArray fa2(storage2, false);

    // create deserializer
    yunomi::serialize::HDF5Deserializer deserializer(path);

    // serialize objects
    deserializer.deserialize("storage", storage2);
    deserializer.deserialize("array", fa2);

    // close the serialize object
    deserializer.close();

    // get values
    std::cout << "FibonacciArray: array[10]=" << fa2[10] << std::endl;
    std::cout << "FibonacciArray: array[20]=" << fa2[20] << std::endl;

    // remove the serialized file.
    yunomi::io::File::remove(path);

    return 0;
}