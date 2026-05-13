#include "complex.hpp"

#include <sstream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../prj.thirdparty/doctest/doctest.h"

TEST_CASE("[complex] - ctor default") {
    Complex c;
    CHECK(c.re == doctest::Approx(0.0));
    CHECK(c.im == doctest::Approx(0.0));
}

TEST_CASE("[complex] - ctor real") {
    Complex c(3.0);
    CHECK(c.re == doctest::Approx(3.0));
    CHECK(c.im == doctest::Approx(0.0));
}

TEST_CASE("[complex] - ctor real imag") {
    Complex c(3.0, -4.0);
    CHECK(c.re == doctest::Approx(3.0));
    CHECK(c.im == doctest::Approx(-4.0));
}

TEST_CASE("[complex] - op== op!=") {
    CHECK(Complex(1.0, 2.0) == Complex(1.0, 2.0));
    CHECK(!(Complex(1.0, 2.0) == Complex(1.0, 3.0)));
    CHECK(Complex(1.0, 2.0) != Complex(1.0, 3.0));
    CHECK(!(Complex(1.0, 2.0) != Complex(1.0, 2.0)));
}

TEST_CASE("[complex] - op- unary") {
    Complex c(3.0, -4.0);
    const Complex neg = -c;
    CHECK(neg == Complex(-3.0, 4.0));
    CHECK(c == Complex(3.0, -4.0));  // original must not be modified
}

TEST_CASE("[complex] - op+= Complex") {
    Complex c(1.0, 2.0);
    c += Complex(3.0, 4.0);
    CHECK(c == Complex(4.0, 6.0));
}

TEST_CASE("[complex] - op+= double") {
    Complex c(1.0, 2.0);
    c += 3.0;
    CHECK(c == Complex(4.0, 2.0));
}

TEST_CASE("[complex] - op-= Complex") {
    Complex c(4.0, 6.0);
    c -= Complex(1.0, 2.0);
    CHECK(c == Complex(3.0, 4.0));
}

TEST_CASE("[complex] - op-= double") {
    Complex c(4.0, 2.0);
    c -= 1.0;
    CHECK(c == Complex(3.0, 2.0));
}

TEST_CASE("[complex] - op*= Complex") {
    // (3+4i)(1+2i) = 3+6i+4i-8 = -5+10i
    Complex c(3.0, 4.0);
    c *= Complex(1.0, 2.0);
    CHECK(c == Complex(-5.0, 10.0));
}

TEST_CASE("[complex] - op*= double") {
    Complex c(3.0, 4.0);
    c *= 2.0;
    CHECK(c == Complex(6.0, 8.0));
}

TEST_CASE("[complex] - op/= Complex") {
    // (3+4i)/(1+2i): de=5, re=11/5=2.2, im=-2/5=-0.4
    Complex c(3.0, 4.0);
    c /= Complex(1.0, 2.0);
    CHECK(c == Complex(2.2, -0.4));
}

TEST_CASE("[complex] - op/= double") {
    Complex c(3.0, 4.0);
    c /= 2.0;
    CHECK(c == Complex(1.5, 2.0));
}

TEST_CASE("[complex] - op/= zero") {
    Complex c(3.0, 4.0);
    CHECK_THROWS_AS(c /= Complex(0.0, 0.0), std::invalid_argument);
    CHECK_THROWS_AS(c /= 0.0, std::invalid_argument);
}

TEST_CASE("[complex] - op+ Complex") {
    CHECK(Complex(1.0, 2.0) + Complex(3.0, 4.0) == Complex(4.0, 6.0));
}

TEST_CASE("[complex] - op+ double") {
    CHECK(Complex(1.0, 2.0) + 3.0 == Complex(4.0, 2.0));
    CHECK(3.0 + Complex(1.0, 2.0) == Complex(4.0, 2.0));
}

TEST_CASE("[complex] - op- Complex") {
    CHECK(Complex(4.0, 6.0) - Complex(1.0, 2.0) == Complex(3.0, 4.0));
}

TEST_CASE("[complex] - op- double") {
    CHECK(Complex(4.0, 2.0) - 1.0 == Complex(3.0, 2.0));
    // 1 - (3+4i) = -2 - 4i
    CHECK(1.0 - Complex(3.0, 4.0) == Complex(-2.0, -4.0));
}

TEST_CASE("[complex] - op* Complex") {
    CHECK(Complex(3.0, 4.0) * Complex(1.0, 2.0) == Complex(-5.0, 10.0));
}

TEST_CASE("[complex] - op* double") {
    CHECK(Complex(3.0, 4.0) * 2.0 == Complex(6.0, 8.0));
    CHECK(2.0 * Complex(3.0, 4.0) == Complex(6.0, 8.0));
}

TEST_CASE("[complex] - op/ Complex") {
    CHECK(Complex(3.0, 4.0) / Complex(1.0, 2.0) == Complex(2.2, -0.4));
    CHECK_THROWS_AS(Complex(3.0, 4.0) / Complex(0.0, 0.0), std::invalid_argument);
}

TEST_CASE("[complex] - op/ double") {
    CHECK(Complex(3.0, 4.0) / 2.0 == Complex(1.5, 2.0));
    CHECK_THROWS_AS(Complex(3.0, 4.0) / 0.0, std::invalid_argument);
}

TEST_CASE("[complex] - op/ double Complex") {
    // 10 / (3+4i): de=25, re=30/25=1.2, im=-40/25=-1.6
    CHECK(10.0 / Complex(3.0, 4.0) == Complex(1.2, -1.6));
    CHECK_THROWS_AS(1.0 / Complex(0.0, 0.0), std::invalid_argument);
}

TEST_CASE("[complex] - write_to") {
    std::ostringstream ss;
    ss << Complex(3.0, 4.0);
    CHECK("{3,4}" == ss.str());

    ss.str("");
    ss << Complex(3.5, -1.25);
    CHECK("{3.5,-1.25}" == ss.str());
}

TEST_CASE("[complex] - read_from") {
    Complex c;
    std::istringstream ss("{3.5,-1.25}");
    ss >> c;
    CHECK(c == Complex(3.5, -1.25));
}

TEST_CASE("[complex] - read_from bad format") {
    Complex c;
    std::istringstream ss("[3.5,-1.25]");
    ss >> c;
    CHECK(ss.fail());
}
