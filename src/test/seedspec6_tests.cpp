// Copyright (c) 2020 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <test/setup_common.h>

#include <boost/test/unit_test.hpp>

#include <array>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <tuple>

BOOST_FIXTURE_TEST_SUITE(seedspec6_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(test_string_constructor_parse) {
    using ExpectedTup = std::tuple<std::vector<uint8_t>, uint16_t, std::string>;
    const ExpectedTup expected[] = {
        // Note: Double-brace for the std::array initializer is required for older GCC, see issue #170
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x02,0x25,0xb7,0x95}}, 9333, "2.37.183.149:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x03,0x87,0xbb,0x0a}}, 9333, "3.135.187.10:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x03,0xd2,0x60,0x09}}, 9333, "3.210.96.9:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x05,0x67,0x89,0x92}}, 9333, "5.103.137.146:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x05,0xbd,0x99,0x85}}, 9333, "5.189.153.133:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x05,0x27,0x7a,0x11}}, 9333, "5.39.122.17:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x05,0x2c,0x61,0x6e}}, 9333, "5.44.97.110:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x05,0x38,0x28,0x01}}, 9333, "5.56.40.1:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x05,0x09,0x1d,0x36}}, 9333, "5.9.29.54:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x0d,0x7d,0xdd,0x09}}, 9333, "13.125.221.9:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x0d,0xe6,0x55,0x8c}}, 9333, "13.230.85.140:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x0d,0x5a,0x8d,0x0d}}, 9333, "13.90.141.13:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x0e,0xa1,0x03,0x88}}, 9333, "14.161.3.136:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x12,0x9c,0x3e,0x8e}}, 9333, "18.156.62.142:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x12,0xdf,0xa4,0x7b}}, 9333, "18.223.164.123:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x17,0x98,0x00,0xc4}}, 9333, "23.152.0.196:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x18,0xc0,0x3c,0xdc}}, 9333, "24.192.60.220:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x1f,0xdc,0x38,0xc3}}, 9333, "31.220.56.195:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x1f,0x19,0xf1,0xe0}}, 9333, "31.25.241.224:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x22,0xe1,0x6b,0x8e}}, 9333, "34.225.107.142:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x22,0xf3,0x13,0xc1}}, 9333, "34.243.19.193:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x22,0xf4,0xbc,0xda}}, 9333, "34.244.188.218:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x22,0x5c,0x51,0x7c}}, 9333, "34.92.81.124:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x23,0xc1,0xda,0xfd}}, 9333, "35.193.218.253:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x23,0xd0,0x6d,0xb5}}, 9333, "35.208.109.181:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x23,0xd1,0x03,0x10}}, 9333, "35.209.3.16:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x23,0xf0,0xdd,0x5a}}, 9333, "35.240.221.90:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x25,0x7b,0x99,0xcd}}, 9333, "37.123.153.205:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x25,0xdd,0xd1,0xde}}, 23333, "37.221.209.222:23333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x26,0x8f,0x42,0x76}}, 9333, "38.143.66.118:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x26,0x57,0x36,0xa3}}, 9334, "38.87.54.163:9334"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x27,0x69,0x7e,0x86}}, 9333, "39.105.126.134:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x27,0x61,0xb1,0x37}}, 9333, "39.97.177.55:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2d,0x97,0x7d,0x18}}, 9333, "45.151.125.24:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2d,0x20,0x4f,0x67}}, 9333, "45.32.79.103:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2d,0x4d,0x0c,0x63}}, 9333, "45.77.12.99:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2d,0x59,0x10,0x11}}, 9333, "45.89.16.17:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2e,0x67,0xfb,0xa0}}, 9333, "46.103.251.160:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2e,0xa5,0xdd,0xd1}}, 9333, "46.165.221.209:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2e,0xa6,0x8e,0x34}}, 9333, "46.166.142.52:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2f,0xdd,0x8a,0xf0}}, 9333, "47.221.138.240:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2f,0xfe,0x80,0x95}}, 8363, "47.254.128.149:8363"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2f,0x3e,0x3d,0x58}}, 9333, "47.62.61.88:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2f,0x5e,0x27,0x57}}, 9333, "47.94.39.87:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x32,0x27,0xf5,0x1a}}, 9333, "50.39.245.26:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x33,0x0f,0x6a,0x15}}, 9333, "51.15.106.21:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x34,0xa8,0x85,0x8c}}, 9333, "52.168.133.140:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x34,0xcb,0x42,0x34}}, 9333, "52.203.66.52:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x34,0xf6,0xa6,0xc0}}, 9333, "52.246.166.192:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x34,0x47,0x59,0xaf}}, 9333, "52.71.89.175:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x34,0x52,0x13,0x0a}}, 9333, "52.82.19.10:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x36,0x98,0xb6,0x19}}, 9333, "54.152.182.25:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x3b,0x6e,0x04,0x77}}, 9333, "59.110.4.119:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x3c,0xf9,0xd7,0xe0}}, 9333, "60.249.215.224:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x3d,0x7c,0x97,0x9f}}, 9333, "61.124.151.159:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x3d,0x12,0x97,0x8e}}, 9333, "61.18.151.142:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x3e,0xd2,0x6e,0xb5}}, 9333, "62.210.110.181:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x3e,0x2a,0x8a,0xa2}}, 9333, "62.42.138.162:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x42,0x97,0xf2,0x9a}}, 9333, "66.151.242.154:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x42,0xbb,0x41,0x06}}, 9333, "66.187.65.6:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x42,0x2a,0x4a,0xfa}}, 9333, "66.42.74.250:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x42,0x60,0xc7,0xf9}}, 9333, "66.96.199.249:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x43,0xef,0x03,0x92}}, 9333, "67.239.3.146:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x44,0xb7,0x4c,0xd7}}, 9333, "68.183.76.215:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x46,0x43,0x92,0x2d}}, 10503, "70.67.146.45:10503"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x47,0x8b,0x7f,0x49}}, 9333, "71.139.127.73:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x47,0x43,0x41,0xe6}}, 12502, "71.67.65.230:12502"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x47,0x54,0xd2,0xcf}}, 9333, "71.84.210.207:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x47,0x5d,0x20,0xf7}}, 9333, "71.93.32.247:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x48,0xb7,0x23,0x12}}, 9333, "72.183.35.18:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x48,0xc4,0x80,0xc1}}, 9333, "72.196.128.193:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x4c,0x54,0x4f,0xd3}}, 9333, "76.84.79.211:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x4d,0x78,0x7a,0x72}}, 9333, "77.120.122.114:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x4e,0x22,0x7e,0x32}}, 9333, "78.34.126.50:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x4e,0x61,0xce,0x95}}, 9333, "78.97.206.149:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x4f,0x37,0x1e,0xbe}}, 9333, "79.55.30.190:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x50,0xb3,0xe2,0x30}}, 9333, "80.179.226.48:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x50,0xc3,0xf3,0x2d}}, 9333, "80.195.243.45:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x51,0xa9,0xfb,0x7c}}, 10333, "81.169.251.124:10333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x52,0x75,0xa6,0x4d}}, 8331, "82.117.166.77:8331"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x52,0xc8,0xcd,0x1e}}, 8331, "82.200.205.30:8331"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x52,0xdd,0x6b,0xd8}}, 9333, "82.221.107.216:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x53,0xdd,0xd3,0x74}}, 9333, "83.221.211.116:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x54,0xd5,0xbc,0x40}}, 9333, "84.213.188.64:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x54,0x2c,0xea,0xdc}}, 9333, "84.44.234.220:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x55,0x0a,0xc9,0x0f}}, 9333, "85.10.201.15:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x55,0x94,0x0e,0xa0}}, 9333, "85.148.14.160:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x55,0xd1,0xf0,0x5c}}, 9333, "85.209.240.92:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x55,0x40,0x23,0x7a}}, 9333, "85.64.35.122:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x57,0x90,0xb7,0x16}}, 9333, "87.144.183.22:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x58,0xc6,0x21,0xd6}}, 9333, "88.198.33.214:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x58,0xd0,0x03,0xc3}}, 8331, "88.208.3.195:8331"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x59,0x84,0xef,0x39}}, 9333, "89.132.239.57:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x59,0xa0,0x71,0xae}}, 9333, "89.160.113.174:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x59,0xa3,0x94,0x79}}, 9333, "89.163.148.121:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x59,0xb3,0xf7,0xec}}, 9333, "89.179.247.236:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x59,0x28,0x07,0x61}}, 9333, "89.40.7.97:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x59,0x07,0x19,0xc4}}, 9333, "89.7.25.196:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5b,0x79,0xa3,0xc3}}, 19334, "91.121.163.195:19334"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5b,0xc5,0x2c,0x90}}, 9333, "91.197.44.144:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5c,0x2a,0x68,0x0d}}, 9333, "92.42.104.13:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5d,0x68,0xd0,0x77}}, 9333, "93.104.208.119:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5d,0xee,0x65,0xa8}}, 9333, "93.238.101.168:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5d,0x5a,0xc1,0xc3}}, 8331, "93.90.193.195:8331"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5e,0x82,0x10,0x60}}, 9333, "94.130.16.96:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5e,0x13,0x49,0x6a}}, 8331, "94.19.73.106:8331"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5e,0xf4,0x61,0xf4}}, 8533, "94.244.97.244:8533"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5e,0xf7,0x86,0xba}}, 9333, "94.247.134.186:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5f,0xd8,0x0c,0xad}}, 9333, "95.216.12.173:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5f,0x3f,0xe5,0x29}}, 9333, "95.63.229.41:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x60,0x7e,0x75,0x05}}, 9333, "96.126.117.5:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x64,0x0b,0x84,0xf1}}, 9333, "100.11.132.241:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x65,0x5c,0x27,0x74}}, 9334, "101.92.39.116:9334"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x66,0x44,0x56,0x31}}, 9333, "102.68.86.49:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x67,0xe6,0xb9,0x04}}, 9333, "103.230.185.4:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x68,0x9a,0xbd,0x18}}, 9333, "104.154.189.24:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x68,0xc4,0xff,0x5b}}, 9333, "104.196.255.91:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x68,0xee,0x83,0x74}}, 9333, "104.238.131.116:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x6a,0xa7,0xc5,0x21}}, 9333, "106.167.197.33:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x6b,0xac,0x09,0xd1}}, 9333, "107.172.9.209:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x6b,0xae,0x46,0xfa}}, 9333, "107.174.70.250:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x6b,0xbf,0x75,0xaf}}, 9333, "107.191.117.175:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x6c,0xad,0x30,0x98}}, 9333, "108.173.48.152:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x6d,0x91,0x2f,0x9e}}, 9333, "109.145.47.158:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x6d,0xc3,0x96,0x6a}}, 9333, "109.195.150.106:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x6f,0x5a,0x97,0x30}}, 9333, "111.90.151.48:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x75,0x14,0x41,0x36}}, 9333, "117.20.65.54:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x77,0x03,0xf8,0x6c}}, 9113, "119.3.248.108:9113"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x79,0xc7,0x3b,0x80}}, 9333, "121.199.59.128:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x79,0xfe,0xaf,0x1b}}, 9333, "121.254.175.27:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x86,0xd1,0x51,0xfb}}, 9333, "134.209.81.251:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x88,0x90,0xd7,0xdb}}, 8133, "136.144.215.219:8133"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x8a,0xc9,0xde,0xd9}}, 9333, "138.201.222.217:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x8a,0x44,0xf9,0x2f}}, 9333, "138.68.249.47:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x8b,0xa2,0x15,0x50}}, 9333, "139.162.21.80:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x8e,0x44,0x52,0xe2}}, 9333, "142.68.82.226:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x90,0x4c,0x2c,0x39}}, 9333, "144.76.44.57:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x95,0xd2,0xee,0x1f}}, 9333, "149.210.238.31:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x98,0x88,0x17,0x0f}}, 9333, "152.136.23.15:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x9b,0x8a,0xc0,0xe4}}, 9333, "155.138.192.228:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x9e,0xb1,0xba,0x37}}, 9333, "158.177.186.55:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xa2,0xdc,0x2f,0x96}}, 9333, "162.220.47.150:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xa2,0xf2,0xa8,0x37}}, 9333, "162.242.168.55:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xa2,0x3e,0x15,0xb6}}, 9333, "162.62.21.182:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xa7,0xac,0x29,0x8c}}, 9333, "167.172.41.140:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xa8,0xeb,0x48,0xc4}}, 9333, "168.235.72.196:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xaa,0x34,0x47,0x62}}, 9333, "170.52.71.98:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xac,0x68,0x22,0xd6}}, 9333, "172.104.34.214:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xac,0xf9,0x4d,0x94}}, 9333, "172.249.77.148:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xac,0x5d,0x85,0x77}}, 6359, "172.93.133.119:6359"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xad,0xd0,0xcb,0x52}}, 9333, "173.208.203.82:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xad,0xd4,0xc1,0x17}}, 9333, "173.212.193.23:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xad,0x4f,0x81,0x72}}, 8888, "173.79.129.114:8888"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xad,0x5e,0x32,0x6b}}, 9333, "173.94.50.107:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xae,0x8d,0xc4,0xcc}}, 9333, "174.141.196.204:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xaf,0xb0,0x8e,0x6e}}, 9333, "175.176.142.110:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xb0,0xdf,0x89,0x25}}, 8335, "176.223.137.37:8335"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xb1,0x26,0xd7,0x09}}, 9333, "177.38.215.9:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xb2,0xda,0x6f,0xfa}}, 9333, "178.218.111.250:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xb2,0x3b,0x22,0x04}}, 9333, "178.59.34.4:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xb2,0x3f,0x58,0x04}}, 9333, "178.63.88.4:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xb3,0xda,0x5a,0xeb}}, 9333, "179.218.90.235:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xb7,0x6f,0xea,0xdd}}, 9333, "183.111.234.221:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xb9,0x6b,0x50,0x01}}, 9333, "185.107.80.1:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xb9,0x19,0x3c,0xc7}}, 8331, "185.25.60.199:8331"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xb9,0x1a,0x1c,0xab}}, 9333, "185.26.28.171:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xb9,0x32,0x25,0xab}}, 9333, "185.50.37.171:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xb9,0x51,0xa4,0x26}}, 9333, "185.81.164.38:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xbc,0x14,0xb8,0x7a}}, 9333, "188.20.184.122:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xbc,0xe2,0x96,0xa6}}, 9333, "188.226.150.166:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xbc,0x28,0x5d,0xcd}}, 9333, "188.40.93.205:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xbc,0x42,0x1a,0x3d}}, 9333, "188.66.26.61:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xbd,0xd3,0x92,0x8c}}, 9333, "189.211.146.140:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xbe,0x02,0x82,0x1b}}, 9333, "190.2.130.27:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc0,0xbb,0x79,0x2b}}, 9333, "192.187.121.43:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc0,0x1e,0x59,0x8e}}, 60714, "192.30.89.142:60714"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc0,0x51,0xdc,0xd2}}, 9333, "192.81.220.210:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc1,0x87,0x0a,0xd7}}, 9333, "193.135.10.215:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc1,0x8a,0xda,0x43}}, 9333, "193.138.218.67:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc1,0xa9,0xf4,0xbd}}, 9333, "193.169.244.189:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc1,0x1d,0xbb,0x4e}}, 9333, "193.29.187.78:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc2,0x0e,0xf6,0xcd}}, 8444, "194.14.246.205:8444"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc2,0x95,0x5a,0x13}}, 9333, "194.149.90.19:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc2,0x63,0x15,0x87}}, 9333, "194.99.21.135:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc3,0x7a,0x96,0xad}}, 9333, "195.122.150.173:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc3,0x9a,0xa8,0x81}}, 9333, "195.154.168.129:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc3,0xb5,0xf0,0xe5}}, 9333, "195.181.240.229:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc6,0xcc,0xe5,0x22}}, 9333, "198.204.229.34:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc6,0x1b,0x53,0xd2}}, 13020, "198.27.83.210:13020"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc6,0x32,0xb3,0x56}}, 9333, "198.50.179.86:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xcb,0xad,0x74,0x22}}, 9333, "203.173.116.34:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xce,0x7c,0x95,0x43}}, 9333, "206.124.149.67:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd1,0xa0,0x21,0xe9}}, 9333, "209.160.33.233:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd2,0xf2,0x1b,0xed}}, 9333, "210.242.27.237:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd3,0x20,0x75,0x6a}}, 9333, "211.32.117.106:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd3,0x2b,0x08,0xa8}}, 8335, "211.43.8.168:8335"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd3,0x3f,0xd4,0x1a}}, 9333, "211.63.212.26:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd4,0x20,0xe6,0xdb}}, 9333, "212.32.230.219:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd5,0xca,0xd4,0x4e}}, 9334, "213.202.212.78:9334"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd5,0xe8,0x7c,0x79}}, 8444, "213.232.124.121:8444"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd8,0x6c,0xec,0xb4}}, 17777, "216.108.236.180:17777"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd9,0x75,0x1d,0x47}}, 9333, "217.117.29.71:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd9,0xa0,0xa9,0xe2}}, 9333, "217.160.169.226:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd9,0xd9,0x37,0x0e}}, 9333, "217.217.55.14:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd9,0x1f,0xa6,0x7b}}, 9333, "217.31.166.123:9333"},
        {{{0x20,0x01,0x04,0x70,0x8f,0x9e,0x09,0x44,0x50,0x54,0x00,0xff,0xfe,0xd7,0xc1,0x64}}, 9333, "[2001:470:8f9e:944:5054:ff:fed7:c164]:9333"},
        {{{0x20,0x01,0x06,0x7c,0x2b,0x8c,0x00,0x01,0x02,0x25,0x90,0xff,0xfe,0x54,0x53,0x06}}, 9333, "[2001:67c:2b8c:1:225:90ff:fe54:5306]:9333"},
        {{{0x26,0x04,0x01,0x80,0x00,0xf4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x61}}, 9333, "[2604:180:f4::61]:9333"},
        {{{0x26,0x04,0xe8,0x80,0x00,0x00,0x00,0x15,0x0e,0xc4,0x7a,0xff,0xfe,0x4a,0x40,0xe1}}, 9333, "[2604:e880:0:15:ec4:7aff:fe4a:40e1]:9333"},
        {{{0x2a,0x00,0x17,0x68,0x20,0x01,0x00,0x27,0x00,0x00,0x00,0x00,0x00,0x00,0xa1,0xcb}}, 9333, "[2a00:1768:2001:27::a1cb]:9333"},
        {{{0x2a,0x00,0x7c,0x80,0x00,0x00,0x01,0x0b,0x00,0x00,0x00,0x00,0x00,0x00,0x4a,0xd9}}, 9333, "[2a00:7c80:0:10b::4ad9]:9333"},
        {{{0x2a,0x01,0x05,0xf0,0xbe,0xef,0x00,0x05,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x01}}, 52102, "[2a01:5f0:beef:5:0:3:0:1]:52102"},
        {{{0x2a,0x01,0x07,0xa0,0x00,0x02,0x13,0x7a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09}}, 9333, "[2a01:7a0:2:137a::9]:9333"},
        {{{0x2a,0x02,0x47,0x80,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x88,0x4a}}, 9333, "[2a02:4780:1:1::1:884a]:9333"},
        {{{0x2a,0x02,0x7b,0x40,0xb0,0xdf,0x89,0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01}}, 8335, "[2a02:7b40:b0df:8925::1]:8335"},
        {{{0x2a,0x02,0xa3,0x11,0x81,0x43,0x8c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04}}, 8343, "[2a02:a311:8143:8c00::4]:8343"},
        {{{0x2a,0x03,0x1b,0x20,0x00,0x01,0xf4,0x10,0x00,0x40,0x00,0x00,0x00,0x00,0xac,0xef}}, 9333, "[2a03:1b20:1:f410:40::acef]:9333"},
        {{{0x2a,0x04,0x21,0x80,0x00,0x01,0x00,0x11,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x16}}, 9333, "[2a04:2180:1:11:f000::16]:9333"},
        {{{0x2a,0x0a,0x51,0xc0,0x00,0x00,0x01,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04}}, 9333, "[2a0a:51c0:0:136::4]:9333"},
        {{{0x2c,0x0f,0xf5,0x98,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x4f,0xc0,0xfe}}, 9333, "[2c0f:f598:6::54f:c0fe]:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x03,0x01,0xc8,0xe7}}, 19333, "3.1.200.231:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x12,0xca,0xd5,0xa8}}, 9333, "18.202.213.168:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x12,0xd0,0xcf,0x97}}, 19333, "18.208.207.151:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x22,0x4c,0xd5,0xe9}}, 19333, "34.76.213.233:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x22,0x4d,0x92,0xc0}}, 19333, "34.77.146.192:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x23,0xd1,0x67,0x84}}, 19333, "35.209.103.132:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x23,0xe2,0xf2,0xa4}}, 19333, "35.226.242.164:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2f,0x6f,0xa2,0x92}}, 9333, "47.111.162.146:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x2f,0x4b,0xda,0x4f}}, 19334, "47.75.218.79:19334"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x33,0x0f,0x7d,0x05}}, 19333, "51.15.125.5:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x34,0x89,0x30,0x2d}}, 19333, "52.137.48.45:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x34,0xd2,0x12,0x54}}, 19333, "52.210.18.84:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x34,0xd3,0xd3,0x0b}}, 19333, "52.211.211.11:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x36,0xa4,0xae,0x49}}, 9333, "54.164.174.73:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x47,0x8b,0x7f,0x49}}, 19333, "71.139.127.73:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x54,0x26,0x03,0xc7}}, 18433, "84.38.3.199:18433"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5b,0x79,0x58,0x34}}, 18130, "91.121.88.52:18130"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5f,0xb3,0xb7,0x8f}}, 19333, "95.179.183.143:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x5f,0xd8,0xae,0x10}}, 19333, "95.216.174.16:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x67,0x4c,0x24,0x71}}, 18331, "103.76.36.113:18331"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x77,0x03,0xa8,0x54}}, 19333, "119.3.168.84:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x79,0x28,0x4c,0x40}}, 9333, "121.40.76.64:9333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x8b,0xa2,0x4c,0x90}}, 19334, "139.162.76.144:19334"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x9d,0xe6,0x72,0x0e}}, 19333, "157.230.114.14:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x9f,0x41,0xa3,0x0f}}, 19333, "159.65.163.15:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xa5,0x16,0x58,0xa3}}, 19333, "165.22.88.163:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xac,0x69,0xe9,0x11}}, 19333, "172.105.233.17:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc1,0x87,0x0a,0xd7}}, 19333, "193.135.10.215:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc2,0x0e,0xf7,0x83}}, 19333, "194.14.247.131:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xc3,0x9a,0xb1,0x31}}, 19333, "195.154.177.49:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xcb,0xa2,0x50,0x65}}, 19333, "203.162.80.101:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xce,0xbd,0xcb,0xa8}}, 19333, "206.189.203.168:19333"},
        {{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xd4,0x2f,0xfe,0x58}}, 19333, "212.47.254.88:19333"},
        {{{0x20,0x01,0x41,0xd0,0x00,0x01,0x8d,0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01}}, 18130, "[2001:41d0:1:8d34::1]:18130"},
        {{{0x2a,0x01,0x04,0xf8,0x01,0x90,0x42,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02}}, 19333, "[2a01:4f8:190:4210::2]:19333"},
        {{{0x2a,0x0a,0x51,0xc0,0x00,0x00,0x01,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04}}, 19333, "[2a0a:51c0:0:136::4]:19333"},
    };

    const std::string nonroutable[] = {
        "192.168.0.123:9333",
        "10.10.10.2:9333",
        "127.0.0.1:9333",
        "[::1]:19333"
    };

    const std::string invalid[] = {
        "193.135.10.215",  // missing port
        "193.135.10.215:65537", // port out of range
        "193.135.10.215:0", // port is 0
        "172.105.233.17:-1", // port is negative
        "www.google.com:9333", // non-numeric
    };

    for (const auto &tup : expected) {
        const auto &data = std::get<0>(tup);
        const auto port = std::get<1>(tup);
        const std::string &host = std::get<2>(tup);
        BOOST_TEST_MESSAGE("Testing: " + host);
        const SeedSpec6 spec{data, port};
        const auto addrBytes = spec.GetAddrBytes();
        BOOST_CHECK_EQUAL_COLLECTIONS(data.begin(), data.end(), addrBytes.begin(), addrBytes.end());
        BOOST_CHECK_EQUAL(spec.GetPort(), port);
        const SeedSpec6 specFromHostStr{host},
                        specFromHostCStr{host.c_str()};
        BOOST_CHECK_MESSAGE(specFromHostStr == spec, "Checking that parsing constructor produced the expected results");
        BOOST_CHECK_MESSAGE(specFromHostStr == specFromHostCStr, "Checking both C-style and C++-style parsing constructors match");
    }


    for (const auto &str : nonroutable) {
        BOOST_TEST_MESSAGE(str + " should be non-routable");
        BOOST_CHECK_THROW(SeedSpec6{str}, std::invalid_argument);
    }

    for (const auto &str : invalid) {
        BOOST_TEST_MESSAGE(str + " should be invalid");
        BOOST_CHECK_THROW(SeedSpec6{str}, std::invalid_argument);
    }
}

BOOST_AUTO_TEST_SUITE_END()

