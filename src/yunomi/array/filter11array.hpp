#pragma once

#include <cstdint>
#include "bitslicearray.hpp"
#include "../core/filter11.hpp"
#include "../serialize/hdf5.hpp"

namespace yunomi {
    namespace array {
        class Filter11Array {
            class ConstValue {
            public:
                ConstValue(types::uint128_t block, uint64_t start, uint64_t length)
                        : block_(block), start_(start), length_(length){}

                operator uint64_t() const {
                    assert(length_ <= UINT64_T_SIZE);
                    types::uint128_t mask = (types::uint128_t(0x1ULL) << length_) - 1;
                    return (block_ >> start_) & mask;
                }

            private:
                types::uint128_t block_;
                uint64_t start_;
                uint64_t length_;
            };

        public:
            Filter11Array(array::BitSliceArray const &array, bool array_is_frozen=true): array_(array) {
                if(array_is_frozen){
                    build();
                }
            }

            void build(){
                std::uint64_t size = array_.size() >> UINT64_T_SIZE_BITS;
                if((array_.size() & UINT64_T_SIZE_MASK) != 0){
                    ++size;
                }
                carry_.resize(size);
                core::Filter11 f11;
                core::Converted64 conv;
                conv.carry = false;
                for(uint64_t i = 0; i < carry_.size(); ++i){
                    carry_(i, i + 1) = conv.carry?1:0;
                    conv.value = array_(i * UINT64_T_SIZE, std::min<uint64_t>(array_.size(), (i + 1) * UINT64_T_SIZE));
                    conv = f11(conv);
                }
            }

            virtual ~Filter11Array(){}

            const ConstValue operator()(std::size_t start, std::size_t end) const {
                assert(end > start);
                assert(end - start <= 64);
                uint64_t start_idx = start >> UINT64_T_SIZE_BITS;
                uint64_t end_idx = end >> UINT64_T_SIZE_BITS;
                const array::BitSliceArray::ConstValue area = array_(
                        start_idx * UINT64_T_SIZE, (end_idx + 1) * UINT64_T_SIZE);
                types::uint128_t const &block = area.block();
                core::Filter11 f11;
                core::Converted64 conv;
                conv.value = block.lower;
                conv.carry = carry_(start_idx, start_idx + 1) == 1;
                conv = f11(conv);
                uint64_t lower = conv.value;
                conv.value = block.upper;
                conv = f11(conv);
                uint64_t upper = conv.value;
                return ConstValue(
                        types::uint128_t(lower, upper),
                        start & UINT64_T_SIZE_MASK,
                        end - start
                );
            }

            std::size_t size() const {
                return array_.size();
            }

            std::size_t n_ones() const {
                core::Converted64 conv;
                core::Filter11 f11;
                conv.carry = false;
                uint64_t n_ones = 0;
                for(uint64_t i = 0; i < array_.size(); i += UINT64_T_SIZE){
                    conv.value = array_(i, i + UINT64_T_SIZE);
                    conv = f11(conv);
                    n_ones += core::Register(conv.value).n_ones();
                }
                return n_ones;
            }

            void resize(std::size_t length){
                throw std::runtime_error("Unsupported function");
            }

            void serialize(io::HDF5Group &g) const {
                auto group_carry = g.create_group("carry_");
                carry_.serialize(group_carry);
            }

            void deserialize(io::HDF5Group &g) {
                auto group_carry = g.group("carry_");
                carry_.deserialize(group_carry);
            }

        private:
            array::BitSliceArray const &array_;
            array::BitSliceArray carry_;
        };
    }
}

