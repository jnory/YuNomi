#pragma once

#include <algorithm>
#include <array>
#include "yunomi/core/register.hpp"

namespace yunomi {
    namespace code {
        struct LargestFactor {
            uint64_t index;
            uint64_t rest;
        };

        class FibFunc {
        public:
            FibFunc() {
                answers_[0] = 1;
                answers_[1] = 1;
                for(uint64_t i = 2; i < 64; ++i){
                    answers_[i] = answers_[i - 2] + answers_[i - 1];
                }
            }

            LargestFactor lookup_maximum_index_smaller_than(uint64_t value) const {
                auto it = std::upper_bound(std::begin(answers_), std::end(answers_), value);
                LargestFactor ret;
                --it;
                ret.index = it - std::begin(answers_);
                ret.rest = value - *it;
                return ret;
            }

            uint64_t operator[](uint64_t i) const {
                return answers_[i];
            }

        private:
            std::array<uint64_t, 64> answers_;
        };

        class Fibonacci {
        public:
            Fibonacci() {}

            uint64_t encode(uint64_t value){
                uint64_t ret = 0;
                while(value != 0){
                    auto fact = ff_.lookup_maximum_index_smaller_than(value);
                    assert(fact.index >= 1);
                    ret += 0x1ULL << (fact.index - 1);
                    value = fact.rest;
                }
                ret += 0x1ULL << yunomi::core::Register(ret).n_digits();
                return ret;
            }

            uint64_t decode(uint64_t value){
                assert(value > 0);
                uint64_t significant = 0x1ULL << (yunomi::core::Register(value).n_digits() - 1);
                value -= significant;

                uint64_t ret = 0;
                while(value != 0){
                    uint64_t pos = yunomi::core::Register(value).n_digits();
                    significant = 0x1ULL << (pos - 1);
                    ret += ff_[pos];

                    value -= significant;
                }
                return ret;
            }

        private:
            FibFunc ff_;
        };
    }
}
