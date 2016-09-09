#pragma once

#include <algorithm>
#include "yunomi/core/register.hpp"
#include "yunomi/array/bitslicearray.hpp"
#include "yunomi/array/fixedlengtharray.hpp"
#include "dual_block_index.hpp"

namespace yunomi {
    namespace succinct {
        template <typename Index, typename BitArray = array::BitSliceArray>
        class Select {
            /*
             * The definition of select(i)
             * select(i) = \argmax_k rank(k) == i
             */

        public:
            Select(Index &index): index_(index){}

            uint64_t operator()(std::size_t i) const {
                return index_.select(i);
            }

        private:
            Index &index_;
        };
    }
}