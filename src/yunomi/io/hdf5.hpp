#pragma once

#include <string>
#include <H5Cpp.h>

namespace yunomi {
    namespace io {
        template <typename T>
        class HDF5DataType {
        public:
            static const H5::PredType &type(){
                throw std::runtime_error("Unimplemented type");
            }
        };

        template <>
        class HDF5DataType<uint64_t> {
        public:
            static const H5::PredType &type(){
                return H5::PredType::NATIVE_UINT64;
            }
        };

        template <>
        class HDF5DataType<uint32_t> {
        public:
            static const H5::PredType &type(){
                return H5::PredType::NATIVE_UINT32;
            }
        };

        template <>
        class HDF5DataType<uint16_t> {
        public:
            static const H5::PredType &type(){
                return H5::PredType::NATIVE_UINT16;
            }
        };

        template <>
        class HDF5DataType<uint8_t> {
        public:
            static const H5::PredType &type(){
                return H5::PredType::NATIVE_UINT8;
            }
        };

        class HDF5Dataset {
        public:
            HDF5Dataset(H5::DataSet const &dataset): dataset_(dataset) {}
            HDF5Dataset(HDF5Dataset const &other): dataset_(other.dataset_){}

            template <typename T>
            std::unique_ptr<T[]> read_array(uint64_t length) {
                auto base_type = HDF5DataType<T>::type();

                H5::DataSpace space = dataset_.getSpace();
                if(space.getSimpleExtentNdims() != 1){
                    throw std::runtime_error("[HDF5Dataset::read_array] ndim != 1");
                }

                hsize_t size;
                space.getSimpleExtentDims(&size);
                if(space.getSimpleExtentNdims() != 1){
                    throw std::runtime_error("[HDF5Dataset::read_array] size != 1");
                }

                hsize_t s = length;
                H5::ArrayType array_type(base_type, 1, &s);

                std::unique_ptr<T[]> data(new T[length]);
                dataset_.read(data.get(), array_type);

                return data;
            }

            template <typename T>
            T read_immediate() {
                auto data_type = HDF5DataType<T>::type();
                H5::DataSpace space = dataset_.getSpace();
                if(space.getSimpleExtentNdims() != 1){
                    throw std::runtime_error("[HDF5Dataset::read_immediate] ndim != 1");
                }
                hsize_t size;
                space.getSimpleExtentDims(&size);
                if(size != 1){
                    throw std::runtime_error("[HDF5Dataset::read_immediate size != 1");
                }

                T data;
                dataset_.read(&data, data_type);
                return data;
            }

        private:
            H5::DataSet dataset_;
        };

        class HDF5Group {
        public:
            HDF5Group(H5::Group const &group): group_(group) {}
            HDF5Group(HDF5Group const &other): group_(other.group_){}

            HDF5Group group(std::string const &name) {
                return HDF5Group(group_.openGroup(name));
            }

            HDF5Group create_group(std::string const &name) {
                return HDF5Group(group_.createGroup(name));
            }

            template <typename T>
            HDF5Dataset create_dataset_array(std::string const &name, T *data, uint64_t size) {
                auto base_type = HDF5DataType<T>::type();
                hsize_t s = size;
                H5::ArrayType array_type(base_type, 1, &s);
                hsize_t dim = 1;
                H5::DataSpace space(1, &dim);
                H5::DataSet dataset = group_.createDataSet(name, array_type, space);
                dataset.write(data, array_type);
                return HDF5Dataset(dataset);
            }

            template <typename T>
            HDF5Dataset create_dataset_immediate(std::string const &name, T data) {
                auto data_type = HDF5DataType<T>::type();
                uint64_t size = 1;
                H5::DataSpace space(1, &size);
                H5::DataSet dataset = group_.createDataSet(name, data_type, space);
                dataset.write(&data, data_type);
                return HDF5Dataset(dataset);
            }

            HDF5Dataset dataset(std::string const &name) {
                return HDF5Dataset(group_.openDataSet(name));
            }

        private:
            H5::Group group_;
        };

        class HDF5File {
        public:
            HDF5File(std::string const &path, std::string const &mode)
                    : file_(path, mode=="w"?H5F_ACC_TRUNC:H5F_ACC_RDONLY) {}

            virtual ~HDF5File() {
            }

            void close(){
                file_.close();
            }

            HDF5Group group(std::string const &name) {
                return HDF5Group(file_.openGroup(name));
            }

            HDF5Group create_group(std::string const &name) {
                return HDF5Group(file_.createGroup(name));
            }

        private:
            H5::H5File file_;
        };
    }
}