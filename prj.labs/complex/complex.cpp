#include "complex.hpp"

#include <limits>
#include <cmath>
#include <stdexcept>

Complex Complex::operator-() const noexcept {
    return {-re, -im};
}

bool Complex::operator==(const Complex& rhs) const noexcept {
    double eps = 2 * std::numeric_limits<double>::epsilon();
    return std::abs(re - rhs.re) <= eps &&
        std::abs(im - rhs.im) <= eps;
}

bool Complex::operator!=(const Complex& rhs) const noexcept {
    return !operator==(rhs);
}

Complex& Complex::operator+=(const Complex& rhs) noexcept {
    re += rhs.re;
    im += rhs.im;
    return *this;
}

Complex& Complex::operator+=(const double rhs) noexcept {
    return operator+=(Complex(rhs));
}

Complex& Complex::operator-=(const Complex& rhs) noexcept {
    re -= rhs.re;
    im -= rhs.im;
    return *this;
}

Complex& Complex::operator-=(const double rhs) noexcept {
    return operator-=(Complex(rhs));
}


Complex& Complex::operator*=(const Complex& rhs) noexcept {
    double c = re;
    re = re * rhs.re - im * rhs.im;
    im = c * rhs.im + im * rhs.re;
    return *this;
}

Complex& Complex::operator*=(const double rhs) noexcept {
    return operator*=(Complex(rhs));
}

Complex& Complex::operator/=(const Complex& rhs) {
    const double de = rhs.re * rhs.re + rhs.im * rhs.im;
    if (de == 0.0) throw std::invalid_argument("Complex: division by zero");
    const double c = re;
    re = (c * rhs.re + im * rhs.im) / de;
    im = (im * rhs.re - c * rhs.im) / de;
    return *this;
}

Complex& Complex::operator/=(const double rhs) {
    if (rhs == 0.0) throw std::invalid_argument("Complex: division by zero");
    re /= rhs;
    im /= rhs;
    return *this;
}

std::ostream& Complex::write_to(std::ostream& ostrm) const noexcept {
    ostrm << left_brace << re << separator << im << right_brace;
    return ostrm;
}

std::istream& Complex::read_from(std::istream& istrm) noexcept {
    char leftbr, sep, rightbr;
    double real, imaginary;

    istrm >> leftbr >> real >> sep >> imaginary >> rightbr;

    if (istrm.good()) {

        if (leftbr == left_brace && sep == separator && rightbr == right_brace) {
            this->re = real;
            this->im = imaginary;
        }
        else {
            istrm.setstate(std::ios_base::failbit);
        }

    }

    return istrm;
}


Complex operator+(const Complex& lhs, const Complex& rhs) noexcept {
    return {lhs.re + rhs.re, lhs.im + rhs.im};
}

Complex operator+(const Complex& lhs, const double rhs) noexcept {
    return {lhs.re + rhs, lhs.im};
}

Complex operator+(const double lhs, const Complex& rhs) noexcept {
    return {lhs + rhs.re, rhs.im};
}

Complex operator-(const Complex& lhs, const Complex& rhs) noexcept {
    return {lhs.re - rhs.re, lhs.im - rhs.im};
}

Complex operator-(const Complex& lhs, const double rhs) noexcept {
    return {lhs.re - rhs, lhs.im};
}

Complex operator-(const double lhs, const Complex& rhs) noexcept {
    return {lhs - rhs.re, -rhs.im};
}

Complex operator*(const Complex& lhs, const Complex& rhs) noexcept {
    return {lhs.re * rhs.re - lhs.im * rhs.im, lhs.re * rhs.im + lhs.im * rhs.re};
}

Complex operator*(const Complex& lhs, const double rhs) noexcept {
    return {lhs.re * rhs, lhs.im * rhs};
}

Complex operator*(const double lhs, const Complex& rhs) noexcept {
    return {lhs * rhs.re, lhs * rhs.im};
}

Complex operator/(const Complex& lhs, const Complex& rhs) {
    double de = rhs.re * rhs.re + rhs.im * rhs.im;
    if (de == 0.0) throw std::invalid_argument("Complex: division by zero");
    return {(lhs.re * rhs.re + lhs.im * rhs.im) / de, (lhs.im * rhs.re - lhs.re * rhs.im) / de};
}

Complex operator/(const Complex& lhs, const double rhs) {
    if (rhs == 0.0) throw std::invalid_argument("Complex: division by zero");
    return {lhs.re / rhs, lhs.im / rhs};
}

Complex operator/(const double lhs, const Complex& rhs) {
    const double de = rhs.re * rhs.re + rhs.im * rhs.im;
    if (de == 0.0) throw std::invalid_argument("Complex: division by zero");
    return {(lhs * rhs.re) / de, (-lhs * rhs.im) / de};
}


