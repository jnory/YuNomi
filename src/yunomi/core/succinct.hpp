#pragma once

#include <array>

namespace yunomi {
    namespace core {

        /**
         * @brief an implementation of a select function for 8bits value
         * @details
         * This class is an implementation of a select function.
         * `select` is one of the well known function, especially in the field "succinct data structure".
         * 
         * function `select(value, i)` is defined as:
         * 
         *     select(value, i) = \argmax_k rank(k) == i,
         * 
         * where:
         * 
         *     rank(value, i) = \sum_{j = 0}^{i - 1} value[j:j + 1].
         * 
         * On this implementation, `value` is restricted up to 8bits value (0 <= `value` < 256)
         * 
         */
        class Select8Bit {
        public:
            constexpr
            Select8Bit() {}

            /**
             * @brief execute the select operation for given input values
             * @param[in] value any unsigned value up to 8bits
             * @param[in] i specify the number that you want to get the index of i-th 1
             * @return uint8_t 
             * Index of digits that the given i-th one is found.
             * If i-th one is not found, it returns larger than 8.
             */
            constexpr
            uint8_t operator()(uint8_t value, uint8_t i) const {
                return select_impl(value, i, 0, 0);
            }

        private:
            constexpr
            uint8_t select_impl(uint8_t value, size_t i, uint8_t done, uint8_t popcount) const {
                return done >= 8?
                       done + popcount
                                : (((value >> done) & 0x1ULL) == 1?
                                   (i == 0? done : select_impl(value, i - 1, done + 1, popcount + 1))
                                                                  : select_impl(value, i, done + 1, popcount));
            }
        };

        /**
         * @brief precomputed cache of `Select8Bit` results for a given value.
         */
        class SelectDictImplValue {
        public:
            /**
             * @brief constructor. calculate all patterns for the given `value`
             * @param value target value that need to be precomputed
             */
            constexpr
            SelectDictImplValue(uint8_t value)
                    : dict_{
                    Select8Bit()(value, 0),
                    Select8Bit()(value, 1),
                    Select8Bit()(value, 2),
                    Select8Bit()(value, 3),
                    Select8Bit()(value, 4),
                    Select8Bit()(value, 5),
                    Select8Bit()(value, 6),
                    Select8Bit()(value, 7)
            }
            {}

            /**
             * @brief get a precomputed `select(value, i)` value
             * @param[in] i specify the number that you want to get the index of i-th 1
             * @return uint8_t index of digits that the given i-th one is found.
             * if i-th one is not found, it returns larger than 8.
             */
            constexpr uint8_t operator[](uint8_t i) const {
                return dict_[i];
            }

        private:
            const uint8_t dict_[8];
        };

        /**
         * @brief precomputed cache of `Select8Bit` results for any 8bits value
         */
        class SelectDictImpl {
        public:
            constexpr
            SelectDictImpl():
                    dict_{
                            SelectDictImplValue(0),
                            SelectDictImplValue(1),
                            SelectDictImplValue(2),
                            SelectDictImplValue(3),
                            SelectDictImplValue(4),
                            SelectDictImplValue(5),
                            SelectDictImplValue(6),
                            SelectDictImplValue(7),
                            SelectDictImplValue(8),
                            SelectDictImplValue(9),
                            SelectDictImplValue(10),
                            SelectDictImplValue(11),
                            SelectDictImplValue(12),
                            SelectDictImplValue(13),
                            SelectDictImplValue(14),
                            SelectDictImplValue(15),
                            SelectDictImplValue(16),
                            SelectDictImplValue(17),
                            SelectDictImplValue(18),
                            SelectDictImplValue(19),
                            SelectDictImplValue(20),
                            SelectDictImplValue(21),
                            SelectDictImplValue(22),
                            SelectDictImplValue(23),
                            SelectDictImplValue(24),
                            SelectDictImplValue(25),
                            SelectDictImplValue(26),
                            SelectDictImplValue(27),
                            SelectDictImplValue(28),
                            SelectDictImplValue(29),
                            SelectDictImplValue(30),
                            SelectDictImplValue(31),
                            SelectDictImplValue(32),
                            SelectDictImplValue(33),
                            SelectDictImplValue(34),
                            SelectDictImplValue(35),
                            SelectDictImplValue(36),
                            SelectDictImplValue(37),
                            SelectDictImplValue(38),
                            SelectDictImplValue(39),
                            SelectDictImplValue(40),
                            SelectDictImplValue(41),
                            SelectDictImplValue(42),
                            SelectDictImplValue(43),
                            SelectDictImplValue(44),
                            SelectDictImplValue(45),
                            SelectDictImplValue(46),
                            SelectDictImplValue(47),
                            SelectDictImplValue(48),
                            SelectDictImplValue(49),
                            SelectDictImplValue(50),
                            SelectDictImplValue(51),
                            SelectDictImplValue(52),
                            SelectDictImplValue(53),
                            SelectDictImplValue(54),
                            SelectDictImplValue(55),
                            SelectDictImplValue(56),
                            SelectDictImplValue(57),
                            SelectDictImplValue(58),
                            SelectDictImplValue(59),
                            SelectDictImplValue(60),
                            SelectDictImplValue(61),
                            SelectDictImplValue(62),
                            SelectDictImplValue(63),
                            SelectDictImplValue(64),
                            SelectDictImplValue(65),
                            SelectDictImplValue(66),
                            SelectDictImplValue(67),
                            SelectDictImplValue(68),
                            SelectDictImplValue(69),
                            SelectDictImplValue(70),
                            SelectDictImplValue(71),
                            SelectDictImplValue(72),
                            SelectDictImplValue(73),
                            SelectDictImplValue(74),
                            SelectDictImplValue(75),
                            SelectDictImplValue(76),
                            SelectDictImplValue(77),
                            SelectDictImplValue(78),
                            SelectDictImplValue(79),
                            SelectDictImplValue(80),
                            SelectDictImplValue(81),
                            SelectDictImplValue(82),
                            SelectDictImplValue(83),
                            SelectDictImplValue(84),
                            SelectDictImplValue(85),
                            SelectDictImplValue(86),
                            SelectDictImplValue(87),
                            SelectDictImplValue(88),
                            SelectDictImplValue(89),
                            SelectDictImplValue(90),
                            SelectDictImplValue(91),
                            SelectDictImplValue(92),
                            SelectDictImplValue(93),
                            SelectDictImplValue(94),
                            SelectDictImplValue(95),
                            SelectDictImplValue(96),
                            SelectDictImplValue(97),
                            SelectDictImplValue(98),
                            SelectDictImplValue(99),
                            SelectDictImplValue(100),
                            SelectDictImplValue(101),
                            SelectDictImplValue(102),
                            SelectDictImplValue(103),
                            SelectDictImplValue(104),
                            SelectDictImplValue(105),
                            SelectDictImplValue(106),
                            SelectDictImplValue(107),
                            SelectDictImplValue(108),
                            SelectDictImplValue(109),
                            SelectDictImplValue(110),
                            SelectDictImplValue(111),
                            SelectDictImplValue(112),
                            SelectDictImplValue(113),
                            SelectDictImplValue(114),
                            SelectDictImplValue(115),
                            SelectDictImplValue(116),
                            SelectDictImplValue(117),
                            SelectDictImplValue(118),
                            SelectDictImplValue(119),
                            SelectDictImplValue(120),
                            SelectDictImplValue(121),
                            SelectDictImplValue(122),
                            SelectDictImplValue(123),
                            SelectDictImplValue(124),
                            SelectDictImplValue(125),
                            SelectDictImplValue(126),
                            SelectDictImplValue(127),
                            SelectDictImplValue(128),
                            SelectDictImplValue(129),
                            SelectDictImplValue(130),
                            SelectDictImplValue(131),
                            SelectDictImplValue(132),
                            SelectDictImplValue(133),
                            SelectDictImplValue(134),
                            SelectDictImplValue(135),
                            SelectDictImplValue(136),
                            SelectDictImplValue(137),
                            SelectDictImplValue(138),
                            SelectDictImplValue(139),
                            SelectDictImplValue(140),
                            SelectDictImplValue(141),
                            SelectDictImplValue(142),
                            SelectDictImplValue(143),
                            SelectDictImplValue(144),
                            SelectDictImplValue(145),
                            SelectDictImplValue(146),
                            SelectDictImplValue(147),
                            SelectDictImplValue(148),
                            SelectDictImplValue(149),
                            SelectDictImplValue(150),
                            SelectDictImplValue(151),
                            SelectDictImplValue(152),
                            SelectDictImplValue(153),
                            SelectDictImplValue(154),
                            SelectDictImplValue(155),
                            SelectDictImplValue(156),
                            SelectDictImplValue(157),
                            SelectDictImplValue(158),
                            SelectDictImplValue(159),
                            SelectDictImplValue(160),
                            SelectDictImplValue(161),
                            SelectDictImplValue(162),
                            SelectDictImplValue(163),
                            SelectDictImplValue(164),
                            SelectDictImplValue(165),
                            SelectDictImplValue(166),
                            SelectDictImplValue(167),
                            SelectDictImplValue(168),
                            SelectDictImplValue(169),
                            SelectDictImplValue(170),
                            SelectDictImplValue(171),
                            SelectDictImplValue(172),
                            SelectDictImplValue(173),
                            SelectDictImplValue(174),
                            SelectDictImplValue(175),
                            SelectDictImplValue(176),
                            SelectDictImplValue(177),
                            SelectDictImplValue(178),
                            SelectDictImplValue(179),
                            SelectDictImplValue(180),
                            SelectDictImplValue(181),
                            SelectDictImplValue(182),
                            SelectDictImplValue(183),
                            SelectDictImplValue(184),
                            SelectDictImplValue(185),
                            SelectDictImplValue(186),
                            SelectDictImplValue(187),
                            SelectDictImplValue(188),
                            SelectDictImplValue(189),
                            SelectDictImplValue(190),
                            SelectDictImplValue(191),
                            SelectDictImplValue(192),
                            SelectDictImplValue(193),
                            SelectDictImplValue(194),
                            SelectDictImplValue(195),
                            SelectDictImplValue(196),
                            SelectDictImplValue(197),
                            SelectDictImplValue(198),
                            SelectDictImplValue(199),
                            SelectDictImplValue(200),
                            SelectDictImplValue(201),
                            SelectDictImplValue(202),
                            SelectDictImplValue(203),
                            SelectDictImplValue(204),
                            SelectDictImplValue(205),
                            SelectDictImplValue(206),
                            SelectDictImplValue(207),
                            SelectDictImplValue(208),
                            SelectDictImplValue(209),
                            SelectDictImplValue(210),
                            SelectDictImplValue(211),
                            SelectDictImplValue(212),
                            SelectDictImplValue(213),
                            SelectDictImplValue(214),
                            SelectDictImplValue(215),
                            SelectDictImplValue(216),
                            SelectDictImplValue(217),
                            SelectDictImplValue(218),
                            SelectDictImplValue(219),
                            SelectDictImplValue(220),
                            SelectDictImplValue(221),
                            SelectDictImplValue(222),
                            SelectDictImplValue(223),
                            SelectDictImplValue(224),
                            SelectDictImplValue(225),
                            SelectDictImplValue(226),
                            SelectDictImplValue(227),
                            SelectDictImplValue(228),
                            SelectDictImplValue(229),
                            SelectDictImplValue(230),
                            SelectDictImplValue(231),
                            SelectDictImplValue(232),
                            SelectDictImplValue(233),
                            SelectDictImplValue(234),
                            SelectDictImplValue(235),
                            SelectDictImplValue(236),
                            SelectDictImplValue(237),
                            SelectDictImplValue(238),
                            SelectDictImplValue(239),
                            SelectDictImplValue(240),
                            SelectDictImplValue(241),
                            SelectDictImplValue(242),
                            SelectDictImplValue(243),
                            SelectDictImplValue(244),
                            SelectDictImplValue(245),
                            SelectDictImplValue(246),
                            SelectDictImplValue(247),
                            SelectDictImplValue(248),
                            SelectDictImplValue(249),
                            SelectDictImplValue(250),
                            SelectDictImplValue(251),
                            SelectDictImplValue(252),
                            SelectDictImplValue(253),
                            SelectDictImplValue(254),
                            SelectDictImplValue(255)
                    } {}

            /**
             * @brief get a precomputed cache for given `value`
             * @param[in] value specify any unsigned value up to 8bits.
             * @return `SelectDictImpleValue&`
             * Precomputed cache for the given `value`. 
             */
            constexpr
            const SelectDictImplValue &operator[](uint8_t value) const {
                return dict_[value];
            }

        private:
            const SelectDictImplValue dict_[256];
        };

        /**
         * @brief Precomputed cache for `Select8Bit`.
         */
        class SelectDict{
        public:
            constexpr
            SelectDict(){}

            /**
             * @brief lookup precomputed value.
             * @param[in] value any unsigned value up to 8bits
             * @param[in] i specify the number that you want to get the index of i-th 1
             * @return uint8_t 
             * precomputed results of `Select8Bit()(value, i)`.
             * This is useful when `Select8Bit` seems to be slow.
             */
            constexpr
            uint8_t operator()(uint8_t value, uint8_t i) const {
                return dict_[value][i];
            }

        private:
            SelectDictImpl dict_;
        };

        /**
         *  @brief utility class for calculating select function for uint64_t value
         */
        class RegisterSuccinct {
        public:
            RegisterSuccinct(uint64_t value): value_(value){}

            /**
             * @brief calculate select function for `value_`
             */
            uint8_t select(uint8_t i) const {
                constexpr SelectDict select;
                uint64_t value = value_;
                uint8_t sel = 0;
                for(uint8_t j = 0; j < 8; j++){
                    uint8_t byte = (uint8_t)value;
                    uint8_t byte_sel = select(byte, i);
                    if(byte_sel < 8){
                        // found i-th 1.
                        return sel + byte_sel;
                    }else{
                        i = i - (byte_sel - 8);
                        sel += 8;
                        value = value >> 8;
                    }
                }
                // not found
                return not_found();
            }

            constexpr static
            uint8_t not_found() {
                return 64;
            }

        private:
            uint64_t value_;
        };

    }
}
