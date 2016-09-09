#pragma once


#include "yunomi/core/succinct.hpp"

namespace yunomi {
    namespace succinct {
        template <typename LargeBlockArray, typename SmallBlockArray, typename BitArray = array::BitSliceArray>
        class DualBlockIndex {
        private:
            class LargeBlocks {
            public:
                LargeBlocks() : block_length_(0){}

                void build(BitArray const &array){
                    block_length_ = calc_block_length(array);
                    assert(core::Register(block_length_).n_digits() <= blocks_.bits_per_slot());
                    blocks_.resize(array.size() / block_length_ + 1, core::Register(array.n_ones()).n_digits());
                    build_impl(array);
                }

                uint64_t operator[](std::size_t i) const {
                    return blocks_[i];
                }

                constexpr uint64_t block_length() const {
                    return block_length_;
                }

                std::size_t size() const {
                    return blocks_.size();
                }

                void serialize(io::HDF5Group &g) const {
                    g.create_dataset_immediate("block_length_", block_length_);
                    auto group = g.create_group("blocks_");
                    blocks_.serialize(group);
                }

                void deserialize(io::HDF5Group &g) {
                    block_length_ = g.dataset("block_length_").read_immediate<uint64_t>();
                    auto group = g.group("blocks_");
                    blocks_.deserialize(group);
                }

            private:
                void build_impl(BitArray const &array){
                    blocks_[0] = 0;
                    std::size_t n_blocks = blocks_.size() - 1;
                    for(std::size_t i = 0; i < n_blocks; ++i){
                        uint64_t start = i * block_length_;
                        uint64_t end = std::min<uint64_t>((i + 1) * block_length_, array.size());
                        uint64_t cursor = start;
                        uint64_t count = blocks_[i];
                        while(cursor < end){
                            uint64_t target = std::min<uint64_t>(cursor + 64, end);
                            count += core::Register(array(cursor, target)).n_ones();
                            cursor = target;
                        }
                        blocks_[i + 1] = count;
                    }
                }

                static uint64_t calc_block_length(BitArray const &array) {
                    uint64_t length = core::Register(array.size()).n_digits();
                    uint64_t block_size = length * length;

                    return block_size;
                }

                uint64_t block_length_;
                LargeBlockArray blocks_;
            };

            class SmallBlocks {
            public:
                SmallBlocks() : block_length_(0){}

                void build(BitArray const &array, LargeBlocks const &largeBlocks) {
                    block_length_ = calc_block_length(array);
                    assert(core::Register(block_length_).n_digits() <= blocks_.bits_per_slot());
                    blocks_.resize(array.size() / block_length_ + 1, core::Register(largeBlocks.block_length()).n_digits());
                    build_impl(array, largeBlocks);
                }

                uint64_t operator[](std::size_t i) const {
                    return blocks_[i];
                }

                constexpr uint64_t block_length() const {
                    return block_length_;
                }

                std::size_t size() const {
                    return blocks_.size();
                }

                void serialize(io::HDF5Group &g) const {
                    g.create_dataset_immediate("block_length_", block_length_);
                    auto group = g.create_group("blocks_");
                    blocks_.serialize(group);
                }

                void deserialize(io::HDF5Group &g) {
                    block_length_ = g.dataset("block_length_").read_immediate<uint64_t>();
                    auto group = g.group("blocks_");
                    blocks_.deserialize(group);
                }

            private:
                void build_impl(BitArray const &array, LargeBlocks const &largeBlocks){
                    blocks_[0] = 0;
                    std::size_t n_blocks = blocks_.size() - 1;
                    for(std::size_t i = 0; i < n_blocks; ++i){
                        uint64_t accum = blocks_[i] + get_large_block_value(
                                array, block_length_, largeBlocks, largeBlocks.block_length(), i);

                        blocks_[i + 1] = accum + core::Register(array(
                                i * block_length_,
                                std::min<std::size_t>((i + 1) * block_length_, array.size())
                        )).n_ones() - get_large_block_value(
                                array, block_length_, largeBlocks, largeBlocks.block_length(), i + 1
                        );
                    }

                }

                static uint64_t calc_block_length(BitArray const &array) {
                    uint64_t length = core::Register(array.size()).n_digits();
                    uint64_t ret = length / 2;
                    assert(ret <= 64);
                    return ret;
                }

                std::size_t get_large_block_value(
                        BitArray const &array, uint64_t block_length, LargeBlocks const &largeBlocks,
                        uint64_t large_block_length, std::size_t i
                ) const {
                    return largeBlocks[std::min<std::size_t>((i * block_length), array.size()) / large_block_length];
                }

                uint64_t block_length_;
                SmallBlockArray blocks_;
            };

            class SmallBlockView {
            public:
                SmallBlockView(const LargeBlocks &large, const SmallBlocks &small): large_(large), small_(small) {
                }

                uint64_t operator[](std::size_t i) const {
                    return large_[i * small_.block_length() / large_.block_length()] + small_[i];
                }

            private:
                const LargeBlocks &large_;
                const SmallBlocks &small_;
            };


        public:
            DualBlockIndex(BitArray const &array, bool array_is_frozen=true): array_(array){
                if(array_is_frozen){
                    build();
                }
            }

            uint64_t rank(std::size_t i) const {
                /*
                 * The definition of rank(i)
                 * rank(i) = \sum_{j = 0}^{i - 1} array(j, j + 1)
                 */
                uint64_t small_blocks_index = i / small_blocks_.block_length();
                uint64_t small_blocks_edge = small_blocks_index * small_blocks_.block_length();
                uint64_t large_blocks_index = small_blocks_edge / large_blocks_.block_length();
                return large_blocks_[large_blocks_index]
                       + small_blocks_[small_blocks_index]
                       + core::Register(array_(small_blocks_edge, i)).n_ones();
            }

            uint64_t select(std::size_t i) const {
                /*
                 * The definition of select(i)
                 * select(i) = \argmax_k rank(k) == i
                 */
                uint64_t block_pos_large = search_blocks(i, large_blocks_, 0, large_blocks_.size() - 1);

                uint64_t bit_start_small = block_pos_large * large_blocks_.block_length();
                uint64_t bit_end_small = bit_start_small + large_blocks_.block_length();

                uint64_t block_start_small = bit_start_small / small_blocks_.block_length();
                uint64_t block_end_small = std::min<uint64_t>(
                        bit_end_small / small_blocks_.block_length(), small_blocks_.size() - 1);

                SmallBlockView view(large_blocks_, small_blocks_);
                uint64_t block_pos_small = search_blocks(i, view, block_start_small, block_end_small);

                uint64_t bit_start = block_pos_small * small_blocks_.block_length();
                uint64_t bit_end = std::min<uint64_t>(
                        bit_start + small_blocks_.block_length(), array_.size());

                // rank(bit_start)
                uint64_t rank_bit_start = view[block_pos_small];
                uint64_t expected_ones = i - rank_bit_start;

                uint64_t bits = array_(bit_start, bit_end);
                uint64_t n_ones_in_block = core::RegisterSuccinct(bits).select(expected_ones);
                return bit_start + (n_ones_in_block == core::RegisterSuccinct::not_found() ? bit_end - bit_start : n_ones_in_block);
            }

            void build(){
                large_blocks_.build(array_);
                small_blocks_.build(array_, large_blocks_);
            }

            void serialize(io::HDF5Group &g) const {
                auto group_large_blocks = g.create_group("large_blocks_");
                large_blocks_.serialize(group_large_blocks);
                auto group_small_blocks = g.create_group("small_blocks_");
                small_blocks_.serialize(group_small_blocks);
            }

            void deserialize(io::HDF5Group &g) {
                auto group_large_blocks = g.group("large_blocks_");
                large_blocks_.deserialize(group_large_blocks);
                auto group_small_blocks = g.group("small_blocks_");
                small_blocks_.deserialize(group_small_blocks);
            }

        private:
            template<typename Blocks>
            uint64_t search_blocks(std::size_t i, const Blocks &blocks, std::size_t start, std::size_t end) const {
                assert(core::Register(start).n_digits() < 64);
                assert(core::Register(end).n_digits() < 64);
                assert(blocks[0] == 0);

                std::size_t low = start;
                std::size_t high = end;

                std::size_t not_found = end + 1;
                std::size_t found = not_found;

                while(low <= high) {
                    std::size_t mid = (low + high) / 2;

                    uint64_t mid_value = blocks[mid];
                    if(mid_value < i) {
                        low = mid + 1;
                    }else if(i < mid_value){
                        assert(mid != 0);
                        high = mid - 1;
                    }else{
                        found = mid;
                        low = mid + 1;
                    }
                }

                if(found == not_found){
                    return high;
                }else{
                    return found;
                }
            }

            BitArray const &array_;
            LargeBlocks large_blocks_;
            SmallBlocks small_blocks_;
        };
    }
}



