#include<boost/python.hpp>
#include<vector>

#include "../include/array_value_reader.h"
#include "../include/fib_array.h"
#include "../include/y_bitarray.h"

namespace yunomi {
    template <typename T>
    class FibArrayBuilder {
    public:
        FibArrayBuilder(){}
        virtual ~FibArrayBuilder(){}

        void append(T value){
            array_.push_back(value);
        }

        void build(std::string path){
            array_value_reader<T> reader(array_);
            fib_array<T> array(reader);

            FILE *fp = std::fopen(path.c_str(), "wb");
            array.dump(fp);
            std::fclose(fp);
        }

    private:
        std::vector<T> array_;
    };

    typedef FibArrayBuilder<uint32_t> BuilderUInt32;
}

BOOST_PYTHON_MODULE(_yunomi){
    using namespace boost::python;
    class_<yunomi::BuilderUInt32>("BuilderUInt32")
            .def("append", &yunomi::BuilderUInt32::append)
            .def("build", &yunomi::BuilderUInt32::build);
}
