#pragma once

#include "bitslicearray.hpp"
#include "filter11array.hpp"
#include "yunomi/core/register.hpp"
#include "yunomi/succinct/dual_block_index.hpp"
#include "yunomi/succinct/select.hpp"
#include "yunomi/code/fibonacci.hpp"


namespace yunomi {
    namespace array {
        template <typename LargeBlockArray, typename SmallBlockArray>
        class FibonacciArrayImpl {
        public:
            FibonacciArrayImpl(BitSliceArray const &data)
                    : data_(data), filtered_(data_, false), index_(filtered_, false), select_(index_){}
            uint64_t operator[](uint64_t i) const {
                uint64_t start = i > 0? select_(i - 1) + 1: 0;
                uint64_t end = select_(i) + 1;
                uint64_t encoded = data_(start, end);
                return encoded;
            }

            void build(){
                filtered_.build();
                index_.build();
            }

            void serialize(io::HDF5Group &g) const {
                auto group_filtered = g.create_group("filtered_");
                filtered_.serialize(group_filtered);
                auto group_index = g.create_group("index_");
                index_.serialize(group_index);
            }

            void deserialize(io::HDF5Group &g) {
                auto group_filtered_ = g.group("filtered_");
                filtered_.deserialize(group_filtered_);
                auto group_index = g.group("index_");
                index_.deserialize(group_index);
            }

        private:
            BitSliceArray const &data_;
            Filter11Array filtered_;
            succinct::DualBlockIndex< LargeBlockArray, SmallBlockArray, decltype(filtered_)> index_;
            succinct::Select<decltype(index_), decltype(data_)> select_;
        };

        template <typename LargeBlockArray, typename SmallBlockArray>
        class FibonacciArrayTmpl;

        template <typename LargeBlockArray, typename SmallBlockArray>
        class FibonacciDataStorageTmpl {
            friend class FibonacciArrayTmpl<LargeBlockArray, SmallBlockArray>;
        public:
            FibonacciDataStorageTmpl(): tail_(0), assigned_(false){}

            void push_back(uint64_t value) {
                uint64_t encoded = fib_.encode(value);
                uint64_t n_digits = core::Register(encoded).n_digits();
                if(data_.size() < tail_ + n_digits){
                    data_.resize((tail_ + n_digits) * 2);
                }
                data_(tail_, tail_ + n_digits) = encoded;
                tail_ += n_digits;
            }

            void serialize(io::HDF5Group &g) const {
                auto group_data = g.create_group("data_");
                data_.serialize(group_data);
                g.create_dataset_immediate("tail_", tail_);
            }

            void deserialize(io::HDF5Group &g) {
                auto group_data = g.group("data_");
                data_.deserialize(group_data);
                tail_ = g.dataset("tail_").read_immediate<uint64_t>();
            }

        private:
            code::Fibonacci fib_;
            BitSliceArray data_;
            uint64_t tail_;
            bool assigned_;
        };

        template <typename LargeBlockArray, typename SmallBlockArray>
        class FibonacciArrayTmpl{
        public:
            FibonacciArrayTmpl(FibonacciDataStorageTmpl<LargeBlockArray, SmallBlockArray> &storage, bool storage_is_frozen=true)
                    : storage_(storage), impl_(storage.data_) {
                if(storage_.assigned_){
                    throw std::runtime_error("[yunomi::array::FibonacciArrayTmpl] Given storage is already assigned to another object.");
                }
                storage_.assigned_ = true;
                if(storage_is_frozen){
                    build();
                }
            }

            void build(){
                storage_.data_.resize(storage_.tail_);
                impl_.build();
            }

            uint64_t operator[](uint64_t i){
                uint64_t encoded = impl_[i];
                return storage_.fib_.decode(encoded);
            }

            void serialize(io::HDF5Group &g) const {
                auto group_impl = g.create_group("impl_");
                impl_.serialize(group_impl);
            }

            void deserialize(io::HDF5Group &g) {
                auto group_impl = g.group("impl_");
                impl_.deserialize(group_impl);
            }

        private:
            FibonacciDataStorageTmpl<LargeBlockArray, SmallBlockArray> &storage_;
            FibonacciArrayImpl<LargeBlockArray, SmallBlockArray> impl_;
        };

        typedef FibonacciDataStorageTmpl<ConstLengthArray<16>, ConstLengthArray<8>> FibonacciDataStorage;
        typedef FibonacciArrayTmpl<ConstLengthArray<16>, ConstLengthArray<8>> FibonacciArray;
    }
}