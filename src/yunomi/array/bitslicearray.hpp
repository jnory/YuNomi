#pragma once

#include <string.h>
#include <algorithm>
#include <cassert>

#include "../core/defines.hpp"
#include "../core/register.hpp"
#include "../types/bigint.hpp"
#include "../io/hdf5.hpp"

namespace yunomi{
    namespace array {
        class BitSliceArray{
        public:
            union BitsType {
                uint64_t *u64;
                types::uint128_t *u128;
            };

            class Value;

            class ConstValue{
            public:
                ConstValue(types::uint128_t const &block, uint64_t const &start, uint64_t const &length)
                        : block_(block), start_(start), length_(length){
                    assert(start < 64);
                }

                operator uint64_t() const {
                    types::uint128_t shifted = block_ >> start_;
                    types::uint128_t mask = (types::uint128_t(0x1ULL) << length_) - 1;
                    return shifted & mask;
                }

                types::uint128_t const &block() const {
                    return block_;
                }

            private:
                friend class Value;
                types::uint128_t const &block_;
                uint64_t const start_;
                uint64_t const length_;
            };

            class Value {
            public:
                Value(types::uint128_t &block, uint64_t start, uint64_t length)
                        : block_(block), cv_(block, start, length){}

                operator uint64_t() const {
                    return (uint64_t) cv_;
                }

                uint64_t operator=(uint64_t value) {
                    types::uint128_t mask = (types::uint128_t(0x1ULL) << (cv_.start_ + cv_.length_)) - 1;
                    mask = mask - ((types::uint128_t(0x1ULL) << cv_.start_) - 1);
                    mask = ~mask;
                    block_ = (block_ & mask) | (types::uint128_t(value) << cv_.start_);
                    return value;
                }

                operator ConstValue() const {
                    return cv_;
                }

            private:
                types::uint128_t &block_;
                ConstValue cv_;
            };

            BitSliceArray(uint64_t length=1024): length_(length){
                uint64_t n_data = calc_u64_size(length);
                bits_.u64 = new uint64_t[n_data];
                std::fill(bits_.u64, bits_.u64 + n_data, 0);
            }

            BitSliceArray(BitSliceArray &other): length_(other.length_) {
                uint64_t n_data = calc_u64_size(other.length_);
                bits_.u64 = new uint64_t[n_data];
                std::copy(other.bits_.u64, other.bits_.u64 + n_data, bits_.u64);
            }

            virtual ~BitSliceArray(){
                delete [] bits_.u64;
            }

            Value operator()(uint64_t start, uint64_t end){
                assert(start <= end);
                uint64_t data_idx = start >> UINT64_T_SIZE_BITS;
                uint64_t bit_idx = start & UINT64_T_SIZE_MASK;
                BitsType block;
                block.u64 = &bits_.u64[data_idx];
                return Value(*block.u128, bit_idx, end - start);
            }

            const ConstValue operator()(uint64_t start, uint64_t end) const {
                assert(start <= end);
                uint64_t data_idx = start >> UINT64_T_SIZE_BITS;
                uint64_t bit_idx = start & UINT64_T_SIZE_MASK;
                BitsType block;
                block.u64 = &bits_.u64[data_idx];
                return ConstValue(*block.u128, bit_idx, end - start);
            }

            uint64_t size() const {
                return length_;
            }

            uint64_t n_ones() const {
                uint64_t n = 0;
                uint64_t n_data = calc_u64_size(length_);
                for(uint64_t i = 0; i < n_data; ++i){
                    n += core::Register(bits_.u64[i]).n_ones();
                }
                return n;
            }

            void resize(uint64_t length){
                uint64_t n_data = calc_u64_size(length_);
                uint64_t *data = bits_.u64;

                uint64_t n_data_new = calc_u64_size(length);
                bits_.u64 = new uint64_t[n_data_new];
                std::copy(data, data + std::min(n_data, n_data_new), bits_.u64);
                if(n_data < n_data_new){
                    std::fill(bits_.u64 + n_data, bits_.u64 + n_data_new, 0);
                }
                delete [] data;
                length_ = length;
            }

            void clear() {
                uint64_t n_data = calc_u64_size(length_);
                std::fill(bits_.u64, bits_.u64 + n_data, 0);
            }

            void serialize(io::HDF5Group &g) const {
                g.create_dataset_immediate("length_", length_);
                uint64_t n_data = calc_u64_size(length_);
                g.create_dataset_array("bits_", bits_.u64, n_data);
            }

            void deserialize(io::HDF5Group &g) {
                length_ = g.dataset("length_").read_immediate<uint64_t>();
                uint64_t n_data = calc_u64_size(length_);
                auto ary = g.dataset("bits_").read_array<uint64_t>(n_data);
                delete [] bits_.u64;
                bits_.u64 = ary.release();
            }

        private:
            static uint64_t calc_u64_size(uint64_t length) {
                uint64_t n_data = length >> UINT64_T_SIZE_BITS;
                if((length & UINT64_T_SIZE_MASK) > 0) {
                    ++n_data;
                }
                ++n_data;
                return n_data;
            }

            BitsType bits_;
            uint64_t length_;
        };

    }
}
