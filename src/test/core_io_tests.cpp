// Copyright (c) 2018-2019 The Lambda developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <core_io.h>

#include <script/script_flags.h>
#include <test/setup_common.h>

#include <boost/test/unit_test.hpp>

#include <string>

BOOST_FIXTURE_TEST_SUITE(core_io_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(parse_hex_test) {
    std::string s = "0x";
    BOOST_CHECK_THROW(ParseScript(s), std::runtime_error);

    for (int numZeroes = 1; numZeroes <= 32; numZeroes++) {
        s += "0";
        if (numZeroes % 2 == 0) {
            BOOST_CHECK_NO_THROW(ParseScript(s));
        } else {
            BOOST_CHECK_THROW(ParseScript(s), std::runtime_error);
        }
    }
}

static void PrintLE(std::ostringstream &testString, size_t bytes, size_t pushLength) {
    testString << "0x";
    while (bytes != 0) {
        testString << std::setfill('0') << std::setw(2) << std::hex << pushLength % 256;
        pushLength /= 256;
        bytes--;
    }
}

static std::string TestPushOpcode(size_t pushWidth, size_t pushLength, size_t actualLength) {
    std::ostringstream testString;

    switch (pushWidth) {
        case 1:
            testString << "PUSHDATA1 ";
            break;
        case 2:
            testString << "PUSHDATA2 ";
            break;
        case 4:
            testString << "PUSHDATA4 ";
            break;
        default:
            assert(false);
    }
    PrintLE(testString, pushWidth, pushLength);
    testString << " 0x";

    for (size_t i = 0; i < actualLength; i++) {
        testString << "01";
    }

    return testString.str();
}

BOOST_AUTO_TEST_CASE(printle_tests) {
    // Ensure the test generator is doing what we think it is.
    std::ostringstream testString;
    PrintLE(testString, 04, 0x8001);
    BOOST_CHECK_EQUAL(testString.str(), "0x01800000");
}

BOOST_AUTO_TEST_CASE(testpushopcode_tests) {
    BOOST_CHECK_EQUAL(TestPushOpcode(1, 2, 2), "PUSHDATA1 0x02 0x0101");
    BOOST_CHECK_EQUAL(TestPushOpcode(2, 2, 2), "PUSHDATA2 0x0200 0x0101");
    BOOST_CHECK_EQUAL(TestPushOpcode(4, 2, 2), "PUSHDATA4 0x02000000 0x0101");
}

BOOST_AUTO_TEST_CASE(parse_push_test) {
    BOOST_CHECK_NO_THROW(ParseScript("0x01 0x01"));
    BOOST_CHECK_NO_THROW(ParseScript("0x01 XOR"));
    BOOST_CHECK_NO_THROW(ParseScript("0x01 1"));
    BOOST_CHECK_NO_THROW(ParseScript("0x01 ''"));
    BOOST_CHECK_NO_THROW(ParseScript("0x02 0x0101"));
    BOOST_CHECK_NO_THROW(ParseScript("0x02 42"));
    BOOST_CHECK_NO_THROW(ParseScript("0x02 'a'"));

    BOOST_CHECK_THROW(ParseScript("0x01 0x0101"), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript("0x01 42"), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript("0x02 0x01"), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript("0x02 XOR"), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript("0x02 1"), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript("0x02 ''"), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript("0x02 0x010101"), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript("0x02 'ab'"), std::runtime_error);

    // Note sizes are LE encoded.  Also, some of these values are not
    // minimally encoded intentionally -- nor are they being required to be
    // minimally encoded.
    BOOST_CHECK_NO_THROW(ParseScript("PUSHDATA4 0x02000000 0x0101"));
    BOOST_CHECK_THROW(ParseScript("PUSHDATA4 0x03000000 0x0101"), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript("PUSHDATA4 0x02000000 0x010101"), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript("PUSHDATA4 0x020000 0x0101"), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript("PUSHDATA4 0x0200000000 0x0101"), std::runtime_error);

    BOOST_CHECK_NO_THROW(ParseScript("PUSHDATA2 0x0200 0x0101"));
    BOOST_CHECK_THROW(ParseScript("PUSHDATA2 0x0300 0x0101"), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript("PUSHDATA2 0x030000 0x0101"), std::runtime_error);
    BOOST_CHECK_NO_THROW(ParseScript("PUSHDATA1 0x02 0x0101"));
    BOOST_CHECK_THROW(ParseScript("PUSHDATA1 0x02 0x010101"), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript("PUSHDATA1 0x0200 0x010101"), std::runtime_error);

    // Ensure pushdata handling is not using 1's complement
    BOOST_CHECK_NO_THROW(ParseScript(TestPushOpcode(1, 0xC8, 0xC8)));
    BOOST_CHECK_THROW(ParseScript(TestPushOpcode(1, 0xC8, 0xC9)), std::runtime_error);

    BOOST_CHECK_NO_THROW(ParseScript(TestPushOpcode(2, 0x8000, 0x8000)));
    BOOST_CHECK_THROW(ParseScript(TestPushOpcode(2, 0x8000, 0x8001)), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript(TestPushOpcode(2, 0x8001, 0x8000)), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript(TestPushOpcode(2, 0x80, 0x81)), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript(TestPushOpcode(2, 0x80, 0x7F)), std::runtime_error);

    // Can't build something too long.
    BOOST_CHECK_NO_THROW(ParseScript(TestPushOpcode(4, 0x8000, 0x8000)));
    BOOST_CHECK_THROW(ParseScript(TestPushOpcode(4, 0x8000, 0x8001)), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript(TestPushOpcode(4, 0x8001, 0x8000)), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript(TestPushOpcode(4, 0x80, 0x81)), std::runtime_error);
    BOOST_CHECK_THROW(ParseScript(TestPushOpcode(4, 0x80, 0x7F)), std::runtime_error);
}

void TestFormatRoundTrip(const std::string &script) {
    BOOST_CHECK_EQUAL(script, FormatScript(ParseScript(script)));
}

BOOST_AUTO_TEST_CASE(format_script_test) {
    TestFormatRoundTrip("0 1 5 CHECKDATASIG CHECKSIG XOR NOP5 NOP10 "
                        "CHECKDATASIGVERIFY DEPTH RETURN VERIFY SPLIT INVERT "
                        "EQUAL HASH256 GREATERTHANOREQUAL RSHIFT");
}

BOOST_AUTO_TEST_CASE(parse_hash_str) {
    { //uint160
        uint160 expected, parsed;
        const uint8_t expectedB[] = {11, 7, 174, 137, 172, 8, 44, 53, 28, 68, 166, 150, 72, 157, 105, 93, 215, 100, 211, 80};
        std::memcpy(expected.begin(), expectedB, std::min(size_t(expected.size()), sizeof(expectedB)));
        BOOST_CHECK_MESSAGE(ParseHashStr("50d364d75d699d4896a6441c352c08ac89ae070b", parsed) && parsed == expected,
                            "Parsing hash160 should yield the expected result");
    }
    { //uint256
        uint256 expected, parsed;
        const uint8_t expectedB[] = {254, 163, 200, 88, 44, 199, 20, 58, 138, 71, 160, 166, 241, 110, 115, 45, 234,
                                     234, 81, 59, 86, 139, 184, 78, 232, 1, 251, 90, 2, 231, 211, 45};
        std::memcpy(expected.begin(), expectedB, std::min(size_t(expected.size()), sizeof(expectedB)));
        BOOST_CHECK_MESSAGE(ParseHashStr("2dd3e7025afb01e84eb88b563b51eaea2d736ef1a6a0478a3a14c72c58c8a3fe", parsed)
                            && parsed == expected, "Parsing hash256 should yield the expected result");
    }
}

BOOST_AUTO_TEST_SUITE_END()
