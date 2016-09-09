#pragma once
#include <cstddef>
#include <vector>
#include "bitslicearray.hpp"

namespace yunomi {
    namespace array {
        class FixedLengthValue;

        class FixedLengthConstValue {
            friend class FixedLengthValue;
        public:
            FixedLengthConstValue(array::BitSliceArray const &bits, uint64_t const &pos, uint8_t const n): bits_(bits), pos_(pos), n_(n){}

            operator uint64_t() const{
                return bits_(pos_ * n_, (pos_ + 1) * n_);
            }

        private:
            array::BitSliceArray const &bits_;
            uint64_t const pos_;
            std::uint8_t const n_;
        };

        class FixedLengthValue {
        public:
            FixedLengthValue(array::BitSliceArray &bits, uint64_t pos, uint8_t n): bits_(bits), cv_(bits_, pos, n){}

            operator uint64_t() const{
                return cv_;
            }

            uint64_t operator=(uint64_t value){
                bits_(cv_.pos_ * cv_.n_, (cv_.pos_ + 1) * cv_.n_) = value;
                return value;
            }

        private:
            array::BitSliceArray &bits_;
            FixedLengthConstValue const cv_;
        };

        template <uint8_t n = 64>
        class ConstLengthArray {
        public:
            ConstLengthArray(uint64_t size=0, uint8_t ignore=0): bits_(size * n), size_(size) {
                static_assert(0 < n && n < UINT64_T_SIZE, "yunomi::array::ConstLengthArray n > 64");
            }

            FixedLengthValue operator[](uint64_t pos) {
                return FixedLengthValue(bits_, pos, n);
            }

            const FixedLengthConstValue operator[](uint64_t pos) const {
                return FixedLengthConstValue(bits_, pos, n);
            }

            uint64_t size() const {
                return size_;
            }

            void resize(uint64_t size, uint8_t ignore=0) {
                bits_.resize(size * n);
                size_ = size;
            }

            constexpr uint8_t bits_per_slot() const {
                return n;
            }

            void serialize(io::HDF5Group &g) const {
                auto group = g.create_group("bits_");
                bits_.serialize(group);
                g.create_dataset_immediate("size_", size_);
            }

            void deserialize(io::HDF5Group &g) {
                auto group = g.group("bits_");
                bits_.deserialize(group);
                size_ = g.dataset("size_").read_immediate<uint64_t>();
            }

        private:
            array::BitSliceArray bits_;
            uint64_t size_;
        };

        template <>
        class ConstLengthArray<8> {
        public:
            ConstLengthArray(uint64_t size=0, uint8_t ignore=0): bits_(size, 0){
            }

            uint8_t &operator[](uint64_t pos) {
                return bits_[pos];
            }

            const uint8_t operator[](uint64_t pos) const {
                return bits_[pos];
            }

            uint64_t size() const {
                return bits_.size();
            }

            void resize(uint64_t size, uint8_t ignore=0) {
                bits_.resize(size, 0);
            }

            constexpr uint8_t bits_per_slot() const {
                return 8;
            }

            void serialize(io::HDF5Group &g) const {
                uint64_t size = bits_.size();
                g.create_dataset_immediate("size", size);
                std::unique_ptr<uint8_t[]> data(new uint8_t[size]);
                std::copy(std::begin(bits_), std::end(bits_), data.get());
                g.create_dataset_array("bits_", data.get(), size);
            }

            void deserialize(io::HDF5Group &g) {
                uint64_t size = g.dataset("size").read_immediate<uint64_t>();
                auto data = g.dataset("bits_").read_array<uint8_t>(size);
                bits_.resize(size, 0);
                std::copy(data.get(), data.get() + size, std::begin(bits_));
            }

        private:
            std::vector<uint8_t> bits_;
        };

        template <>
        class ConstLengthArray<16> {
        public:
            ConstLengthArray(uint64_t size=0, uint8_t ignore=0): bits_(size, 0){
            }

            uint16_t &operator[](uint64_t pos) {
                return bits_[pos];
            }

            const uint16_t operator[](uint64_t pos) const {
                return bits_[pos];
            }

            uint64_t size() const {
                return bits_.size();
            }

            void resize(uint64_t size, uint8_t ignore=0) {
                bits_.resize(size, 0);
            }

            constexpr uint8_t bits_per_slot() const {
                return 16;
            }

            void serialize(io::HDF5Group &g) const {
                uint64_t size = bits_.size();
                g.create_dataset_immediate("size", size);
                std::unique_ptr<uint16_t[]> data(new uint16_t[size]);
                std::copy(std::begin(bits_), std::end(bits_), data.get());
                g.create_dataset_array("bits_", data.get(), size);
            }

            void deserialize(io::HDF5Group &g) {
                uint64_t size = g.dataset("size").read_immediate<uint64_t>();
                auto data = g.dataset("bits_").read_array<uint16_t>(size);
                bits_.resize(size, 0);
                std::copy(data.get(), data.get() + size, std::begin(bits_));
            }

        private:
            std::vector<uint16_t> bits_;
        };

        template <>
        class ConstLengthArray<32> {
        public:
            ConstLengthArray(uint64_t size=0, uint8_t ignore=0): bits_(size, 0){
            }

            uint32_t &operator[](uint64_t pos) {
                return bits_[pos];
            }

            const uint32_t operator[](uint64_t pos) const {
                return bits_[pos];
            }

            uint64_t size() const {
                return bits_.size();
            }

            void resize(uint64_t size, uint8_t ignore=0) {
                bits_.resize(size, 0);
            }

            constexpr uint8_t bits_per_slot() const {
                return 32;
            }

            void serialize(io::HDF5Group &g) const {
                uint64_t size = bits_.size();
                g.create_dataset_immediate("size", size);
                std::unique_ptr<uint32_t[]> data(new uint32_t[size]);
                std::copy(std::begin(bits_), std::end(bits_), data.get());
                g.create_dataset_array("bits_", data.get(), size);
            }

            void deserialize(io::HDF5Group &g) {
                uint64_t size = g.dataset("size").read_immediate<uint64_t>();
                auto data = g.dataset("bits_").read_array<uint32_t>(size);
                bits_.resize(size, 0);
                std::copy(data.get(), data.get() + size, std::begin(bits_));
            }

        private:
            std::vector<uint32_t> bits_;
        };

        template <>
        class ConstLengthArray<64> {
        public:
            ConstLengthArray(uint64_t size=0, uint8_t ignore=0): bits_(size, 0){
            }

            uint64_t &operator[](uint64_t pos) {
                return bits_[pos];
            }

            const uint64_t operator[](uint64_t pos) const {
                return bits_[pos];
            }

            uint64_t size() const {
                return bits_.size();
            }

            void resize(uint64_t size, uint8_t ignore=0) {
                bits_.resize(size, 0);
            }

            constexpr uint8_t bits_per_slot() const {
                return 64;
            }

            void serialize(io::HDF5Group &g) const {
                uint64_t size = bits_.size();
                g.create_dataset_immediate("size", size);
                std::unique_ptr<uint64_t[]> data(new uint64_t[size]);
                std::copy(std::begin(bits_), std::end(bits_), data.get());
                g.create_dataset_array("bits_", data.get(), size);
            }

            void deserialize(io::HDF5Group &g) {
                uint64_t size = g.dataset("size").read_immediate<uint64_t>();
                auto data = g.dataset("bits_").read_array<uint64_t>(size);
                bits_.resize(size, 0);
                std::copy(data.get(), data.get() + size, std::begin(bits_));
            }

        private:
            std::vector<uint64_t> bits_;
        };

        class FixedLengthArray {
        public:
            FixedLengthArray(uint64_t size=0, uint8_t n_digits=1):
                    bits_(size * n_digits), size_(size), n_digits_(n_digits){
                assert(0 < n_digits && n_digits <= UINT64_T_SIZE);
            }

            FixedLengthValue operator[](uint64_t pos) {
                return FixedLengthValue(bits_, pos, n_digits_);
            }

            const FixedLengthConstValue operator[](uint64_t pos) const {
                return FixedLengthConstValue(bits_, pos, n_digits_);
            }

            uint64_t size() const {
                return size_;
            }

            void resize(uint64_t size, uint8_t n_digits) {
                if(n_digits == n_digits_){
                    bits_.resize(size * n_digits_);
                }else{
                    FixedLengthArray ary(*this);
                    bits_.resize(size * n_digits);
                    bits_.clear();
                    n_digits_ = n_digits;
                    for(uint64_t i = 0; i < std::min(size_, size); ++i){
                        (*this)[i] = uint64_t(ary[i]);
                    }
                }
                size_ = size;
            }

            constexpr uint8_t bits_per_slot() const {
                return n_digits_;
            }

            void serialize(io::HDF5Group &g) const {
                auto group = g.create_group("bits_");
                bits_.serialize(group);
                g.create_dataset_immediate("size_", size_);
                g.create_dataset_immediate("n_digits_", n_digits_);
            }

            void deserialize(io::HDF5Group &g) {
                auto group = g.group("bits_");
                bits_.deserialize(group);
                size_ = g.dataset("size_").read_immediate<uint64_t>();
                n_digits_ = g.dataset("n_digits_").read_immediate<uint8_t>();
            }

        private:
            array::BitSliceArray bits_;
            uint64_t size_;
            uint8_t n_digits_;
        };
    }
}
