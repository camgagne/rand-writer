// Copyright 2025 Cameron Gagne

#include <iostream>
#include "RandWriter.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_kZero) {
    std::string input = "foobar";
    RandWriter rw(input, 0);

    BOOST_REQUIRE_NO_THROW(rw.freq(""));
    BOOST_CHECK_EQUAL(rw.freq(""), input.size());

    BOOST_CHECK_EQUAL(rw.freq("", 'o'), 2);

    char c = rw.kRand("");
    BOOST_CHECK(input.find(c) != std::string::npos);

    std::string generated = rw.generate("", 10);
    BOOST_CHECK_EQUAL(generated.size(), 10);
}

BOOST_AUTO_TEST_CASE(testNoErrorChecking) {
    std::string input = "foobar";
    RandWriter rw(input, 3);

    BOOST_REQUIRE_NO_THROW(rw.freq("foo"));
    BOOST_REQUIRE_NO_THROW(rw.freq("oob", 'a'));
    BOOST_REQUIRE_NO_THROW(rw.kRand("foo"));
    BOOST_REQUIRE_NO_THROW(rw.generate("foo", 10));
}

BOOST_AUTO_TEST_CASE(testWrongLength) {
    std::string input = "foobar";
    RandWriter rw(input, 3);

    BOOST_REQUIRE_THROW(rw.freq("fo"), std::invalid_argument);
    BOOST_REQUIRE_THROW(rw.freq("foobar"), std::invalid_argument);
    BOOST_REQUIRE_THROW(rw.freq("o", 'a'), std::invalid_argument);
    BOOST_REQUIRE_THROW(rw.kRand("foob"), std::invalid_argument);
    BOOST_REQUIRE_THROW(rw.generate("fo", 10), std::invalid_argument);
    BOOST_REQUIRE_EQUAL(rw.generate("foo", 10).size(), 10);
}

BOOST_AUTO_TEST_CASE(testWrongStart) {
    std::string input = "foobar";
    RandWriter rw(input, 3);

    BOOST_REQUIRE_THROW(rw.kRand("yea"), std::invalid_argument);
    BOOST_REQUIRE_THROW(rw.generate("yea", 10), std::invalid_argument);
    BOOST_REQUIRE_EQUAL(rw.generate("foo", 10)[0], 'f');
}

BOOST_AUTO_TEST_CASE(testWrongDistribution) {
    std::string input = "aaaab";
    RandWriter rw(input, 1);

    const int iterations = 10000;
    int count_a = 0;
    int count_b = 0;
    for (int i = 0; i < iterations; i++) {
        char result = rw.kRand("a");
        if (result == 'a')
            count_a++;
        else if (result == 'b')
            count_b++;
    }
    double ratio_a = static_cast<double>(count_a) / iterations;
    double ratio_b = static_cast<double>(count_b) / iterations;

    // 5% tolerance.
    BOOST_CHECK_CLOSE(ratio_a, 0.75, 5.0);
    BOOST_CHECK_CLOSE(ratio_b, 0.25, 5.0);
}
