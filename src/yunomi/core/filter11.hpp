#pragma once

namespace yunomi {
    namespace core {
        struct Converted {
            uint8_t value;
            bool carry;

            constexpr
            Converted(uint8_t value, bool carry) :value(value), carry(carry){}

            constexpr
            bool operator==(Converted const &other) const{
                return value == other.value && carry == other.carry;
            }
        };
        struct Converted64 {
            uint64_t value;
            bool carry;
        };

        class FilterEleven8bit {
        public:
            constexpr FilterEleven8bit() {}

            constexpr
            Converted operator()(uint8_t value, bool carry) const {
                return generate(value, filter_impl(value, carry));
            }

        private:
            constexpr
            bool calc_carry(uint8_t orig_value, uint8_t converted_value) const {
                return ((orig_value >> 7) > 0) && ((converted_value >> 7) == 0);
            }

            constexpr
            Converted generate(uint8_t orig_value, uint8_t converted_value) const {
                return Converted(converted_value, calc_carry(orig_value, converted_value));
            }

            constexpr
            uint8_t first1(uint8_t value) const {
                return value - (value & (value - 1));
            }

            constexpr
            uint8_t first11(uint8_t value) const {
                return first1(value & (value << 0x1ULL));
            }

            constexpr
            uint8_t broadcast_lower(uint8_t value) const {
                return value | (value - 1);
            }

            constexpr
            uint8_t mask_remove_first11(uint8_t value_first11) const {
                // if value == 0, broadcast_lower() returns 255,
                // resulting in generated mask is 0.
                return 255 - broadcast_lower(value_first11);
            }

            constexpr
            bool carry_affected(uint8_t value) const {
                return (value & 0x1ULL) > 0;
            }

            constexpr
            uint8_t filter_impl_no_carry_sub(uint8_t value_orig, uint8_t value_first11) const {
                return value_first11 + filter_impl_no_carry(value_orig & mask_remove_first11(value_first11));
            }

            constexpr
            uint8_t filter_impl_no_carry(uint8_t value) const {
                return (value == 0)?value:filter_impl_no_carry_sub(value, first11(value));
            }

            constexpr
            uint8_t filter_impl_carry(uint8_t value) const {
                return carry_affected(value)?0x1ULL + filter_impl_no_carry(value - 1):filter_impl_no_carry(value);
            }

            constexpr
            uint8_t filter_impl(uint8_t value, bool carry) const {
                return carry?filter_impl_carry(value):filter_impl_no_carry(value);
            }
        };

        class FilterElevenCarryDict {
        public:
            constexpr
            FilterElevenCarryDict(uint8_t value)
                    : carry_(FilterEleven8bit()(value, true)),
                      no_carry_(FilterEleven8bit()(value, false))
            {}

            const Converted &operator[](bool c) const {
                return c?carry_:no_carry_;
            }

        private:
            const Converted carry_;
            const Converted no_carry_;
        };

        class FilterElevenDict {
        public:
            constexpr
            FilterElevenDict():
                    dict_{
                            FilterElevenCarryDict(0),
                            FilterElevenCarryDict(1),
                            FilterElevenCarryDict(2),
                            FilterElevenCarryDict(3),
                            FilterElevenCarryDict(4),
                            FilterElevenCarryDict(5),
                            FilterElevenCarryDict(6),
                            FilterElevenCarryDict(7),
                            FilterElevenCarryDict(8),
                            FilterElevenCarryDict(9),
                            FilterElevenCarryDict(10),
                            FilterElevenCarryDict(11),
                            FilterElevenCarryDict(12),
                            FilterElevenCarryDict(13),
                            FilterElevenCarryDict(14),
                            FilterElevenCarryDict(15),
                            FilterElevenCarryDict(16),
                            FilterElevenCarryDict(17),
                            FilterElevenCarryDict(18),
                            FilterElevenCarryDict(19),
                            FilterElevenCarryDict(20),
                            FilterElevenCarryDict(21),
                            FilterElevenCarryDict(22),
                            FilterElevenCarryDict(23),
                            FilterElevenCarryDict(24),
                            FilterElevenCarryDict(25),
                            FilterElevenCarryDict(26),
                            FilterElevenCarryDict(27),
                            FilterElevenCarryDict(28),
                            FilterElevenCarryDict(29),
                            FilterElevenCarryDict(30),
                            FilterElevenCarryDict(31),
                            FilterElevenCarryDict(32),
                            FilterElevenCarryDict(33),
                            FilterElevenCarryDict(34),
                            FilterElevenCarryDict(35),
                            FilterElevenCarryDict(36),
                            FilterElevenCarryDict(37),
                            FilterElevenCarryDict(38),
                            FilterElevenCarryDict(39),
                            FilterElevenCarryDict(40),
                            FilterElevenCarryDict(41),
                            FilterElevenCarryDict(42),
                            FilterElevenCarryDict(43),
                            FilterElevenCarryDict(44),
                            FilterElevenCarryDict(45),
                            FilterElevenCarryDict(46),
                            FilterElevenCarryDict(47),
                            FilterElevenCarryDict(48),
                            FilterElevenCarryDict(49),
                            FilterElevenCarryDict(50),
                            FilterElevenCarryDict(51),
                            FilterElevenCarryDict(52),
                            FilterElevenCarryDict(53),
                            FilterElevenCarryDict(54),
                            FilterElevenCarryDict(55),
                            FilterElevenCarryDict(56),
                            FilterElevenCarryDict(57),
                            FilterElevenCarryDict(58),
                            FilterElevenCarryDict(59),
                            FilterElevenCarryDict(60),
                            FilterElevenCarryDict(61),
                            FilterElevenCarryDict(62),
                            FilterElevenCarryDict(63),
                            FilterElevenCarryDict(64),
                            FilterElevenCarryDict(65),
                            FilterElevenCarryDict(66),
                            FilterElevenCarryDict(67),
                            FilterElevenCarryDict(68),
                            FilterElevenCarryDict(69),
                            FilterElevenCarryDict(70),
                            FilterElevenCarryDict(71),
                            FilterElevenCarryDict(72),
                            FilterElevenCarryDict(73),
                            FilterElevenCarryDict(74),
                            FilterElevenCarryDict(75),
                            FilterElevenCarryDict(76),
                            FilterElevenCarryDict(77),
                            FilterElevenCarryDict(78),
                            FilterElevenCarryDict(79),
                            FilterElevenCarryDict(80),
                            FilterElevenCarryDict(81),
                            FilterElevenCarryDict(82),
                            FilterElevenCarryDict(83),
                            FilterElevenCarryDict(84),
                            FilterElevenCarryDict(85),
                            FilterElevenCarryDict(86),
                            FilterElevenCarryDict(87),
                            FilterElevenCarryDict(88),
                            FilterElevenCarryDict(89),
                            FilterElevenCarryDict(90),
                            FilterElevenCarryDict(91),
                            FilterElevenCarryDict(92),
                            FilterElevenCarryDict(93),
                            FilterElevenCarryDict(94),
                            FilterElevenCarryDict(95),
                            FilterElevenCarryDict(96),
                            FilterElevenCarryDict(97),
                            FilterElevenCarryDict(98),
                            FilterElevenCarryDict(99),
                            FilterElevenCarryDict(100),
                            FilterElevenCarryDict(101),
                            FilterElevenCarryDict(102),
                            FilterElevenCarryDict(103),
                            FilterElevenCarryDict(104),
                            FilterElevenCarryDict(105),
                            FilterElevenCarryDict(106),
                            FilterElevenCarryDict(107),
                            FilterElevenCarryDict(108),
                            FilterElevenCarryDict(109),
                            FilterElevenCarryDict(110),
                            FilterElevenCarryDict(111),
                            FilterElevenCarryDict(112),
                            FilterElevenCarryDict(113),
                            FilterElevenCarryDict(114),
                            FilterElevenCarryDict(115),
                            FilterElevenCarryDict(116),
                            FilterElevenCarryDict(117),
                            FilterElevenCarryDict(118),
                            FilterElevenCarryDict(119),
                            FilterElevenCarryDict(120),
                            FilterElevenCarryDict(121),
                            FilterElevenCarryDict(122),
                            FilterElevenCarryDict(123),
                            FilterElevenCarryDict(124),
                            FilterElevenCarryDict(125),
                            FilterElevenCarryDict(126),
                            FilterElevenCarryDict(127),
                            FilterElevenCarryDict(128),
                            FilterElevenCarryDict(129),
                            FilterElevenCarryDict(130),
                            FilterElevenCarryDict(131),
                            FilterElevenCarryDict(132),
                            FilterElevenCarryDict(133),
                            FilterElevenCarryDict(134),
                            FilterElevenCarryDict(135),
                            FilterElevenCarryDict(136),
                            FilterElevenCarryDict(137),
                            FilterElevenCarryDict(138),
                            FilterElevenCarryDict(139),
                            FilterElevenCarryDict(140),
                            FilterElevenCarryDict(141),
                            FilterElevenCarryDict(142),
                            FilterElevenCarryDict(143),
                            FilterElevenCarryDict(144),
                            FilterElevenCarryDict(145),
                            FilterElevenCarryDict(146),
                            FilterElevenCarryDict(147),
                            FilterElevenCarryDict(148),
                            FilterElevenCarryDict(149),
                            FilterElevenCarryDict(150),
                            FilterElevenCarryDict(151),
                            FilterElevenCarryDict(152),
                            FilterElevenCarryDict(153),
                            FilterElevenCarryDict(154),
                            FilterElevenCarryDict(155),
                            FilterElevenCarryDict(156),
                            FilterElevenCarryDict(157),
                            FilterElevenCarryDict(158),
                            FilterElevenCarryDict(159),
                            FilterElevenCarryDict(160),
                            FilterElevenCarryDict(161),
                            FilterElevenCarryDict(162),
                            FilterElevenCarryDict(163),
                            FilterElevenCarryDict(164),
                            FilterElevenCarryDict(165),
                            FilterElevenCarryDict(166),
                            FilterElevenCarryDict(167),
                            FilterElevenCarryDict(168),
                            FilterElevenCarryDict(169),
                            FilterElevenCarryDict(170),
                            FilterElevenCarryDict(171),
                            FilterElevenCarryDict(172),
                            FilterElevenCarryDict(173),
                            FilterElevenCarryDict(174),
                            FilterElevenCarryDict(175),
                            FilterElevenCarryDict(176),
                            FilterElevenCarryDict(177),
                            FilterElevenCarryDict(178),
                            FilterElevenCarryDict(179),
                            FilterElevenCarryDict(180),
                            FilterElevenCarryDict(181),
                            FilterElevenCarryDict(182),
                            FilterElevenCarryDict(183),
                            FilterElevenCarryDict(184),
                            FilterElevenCarryDict(185),
                            FilterElevenCarryDict(186),
                            FilterElevenCarryDict(187),
                            FilterElevenCarryDict(188),
                            FilterElevenCarryDict(189),
                            FilterElevenCarryDict(190),
                            FilterElevenCarryDict(191),
                            FilterElevenCarryDict(192),
                            FilterElevenCarryDict(193),
                            FilterElevenCarryDict(194),
                            FilterElevenCarryDict(195),
                            FilterElevenCarryDict(196),
                            FilterElevenCarryDict(197),
                            FilterElevenCarryDict(198),
                            FilterElevenCarryDict(199),
                            FilterElevenCarryDict(200),
                            FilterElevenCarryDict(201),
                            FilterElevenCarryDict(202),
                            FilterElevenCarryDict(203),
                            FilterElevenCarryDict(204),
                            FilterElevenCarryDict(205),
                            FilterElevenCarryDict(206),
                            FilterElevenCarryDict(207),
                            FilterElevenCarryDict(208),
                            FilterElevenCarryDict(209),
                            FilterElevenCarryDict(210),
                            FilterElevenCarryDict(211),
                            FilterElevenCarryDict(212),
                            FilterElevenCarryDict(213),
                            FilterElevenCarryDict(214),
                            FilterElevenCarryDict(215),
                            FilterElevenCarryDict(216),
                            FilterElevenCarryDict(217),
                            FilterElevenCarryDict(218),
                            FilterElevenCarryDict(219),
                            FilterElevenCarryDict(220),
                            FilterElevenCarryDict(221),
                            FilterElevenCarryDict(222),
                            FilterElevenCarryDict(223),
                            FilterElevenCarryDict(224),
                            FilterElevenCarryDict(225),
                            FilterElevenCarryDict(226),
                            FilterElevenCarryDict(227),
                            FilterElevenCarryDict(228),
                            FilterElevenCarryDict(229),
                            FilterElevenCarryDict(230),
                            FilterElevenCarryDict(231),
                            FilterElevenCarryDict(232),
                            FilterElevenCarryDict(233),
                            FilterElevenCarryDict(234),
                            FilterElevenCarryDict(235),
                            FilterElevenCarryDict(236),
                            FilterElevenCarryDict(237),
                            FilterElevenCarryDict(238),
                            FilterElevenCarryDict(239),
                            FilterElevenCarryDict(240),
                            FilterElevenCarryDict(241),
                            FilterElevenCarryDict(242),
                            FilterElevenCarryDict(243),
                            FilterElevenCarryDict(244),
                            FilterElevenCarryDict(245),
                            FilterElevenCarryDict(246),
                            FilterElevenCarryDict(247),
                            FilterElevenCarryDict(248),
                            FilterElevenCarryDict(249),
                            FilterElevenCarryDict(250),
                            FilterElevenCarryDict(251),
                            FilterElevenCarryDict(252),
                            FilterElevenCarryDict(253),
                            FilterElevenCarryDict(254),
                            FilterElevenCarryDict(255)
                    } {}

            constexpr
            const FilterElevenCarryDict &operator[](uint8_t value) const {
                return dict_[value];
            }

        private:
            const FilterElevenCarryDict dict_[256];
        };

        class Filter11 {
        public:
            Filter11() {}

            Converted64 operator()(Converted64 const &value) {
                constexpr FilterElevenDict dict;
                Converted64 ret;
                ret.value = 0;
                ret.carry = value.carry;
                bool &carry = ret.carry;
                for(uint8_t i = 0; i < 8; ++i){
                    uint8_t v = (uint8_t)(value.value >> (i * 8));
                    const Converted &conv = dict[v][carry];
                    carry = conv.carry;
                    ret.value |= ((uint64_t)conv.value) << (i * 8);
                }
                return ret;
            }
        };
    }
}