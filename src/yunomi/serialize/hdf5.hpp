#pragma once

#include <string>
#include "../io/hdf5.hpp"

namespace yunomi {
    namespace serialize {
        class HDF5Serializer {
        public:
            HDF5Serializer(std::string path): file_(path, "w"), closed_(false) {}

            virtual ~HDF5Serializer(){
                close();

            }

            void close(){
                if(!closed_){
                    file_.close();
                    closed_ = true;
                }
            }

            template <typename T>
            void serialize(std::string const &name, T const &object){
                auto group = file_.create_group(name);
                object.serialize(group);
            }


        private:
            io::HDF5File file_;
            bool closed_;
        };

        class HDF5Deserializer {
        public:
            HDF5Deserializer(std::string path) : file_(path, "r"), closed_(false) {}

            virtual ~HDF5Deserializer(){
                close();
            }

            void close(){
                if(!closed_){
                    file_.close();
                    closed_ = true;
                }
            }

            template <typename T>
            void deserialize(std::string const &name, T &object){
                auto group = file_.group(name);
                object.deserialize(group);
            }

        private:
            io::HDF5File file_;
            bool closed_;
        };
    }
}
