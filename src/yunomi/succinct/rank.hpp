#pragma once

#include <algorithm>
#include "yunomi/core/register.hpp"
#include "yunomi/array/bitslicearray.hpp"
#include "yunomi/array/fixedlengtharray.hpp"
#include "dual_block_index.hpp"

namespace yunomi {
    namespace succinct {
        template <typename Index, typename BitArray=array::BitSliceArray>
        class Rank {
            /*
             * The definition of rank[i]
             * rank[i] = \sum_{j = 0}^{i - 1} array(j, j + 1)
             */

        public:
            Rank(Index &index): index_(index){
            }

            uint64_t operator()(std::size_t i) const {
                return index_.rank(i);
            }

        private:
            Index &index_;
        };
    }
}