#ifndef PROSTOMOLOTOV_A_S_25_17_COMPLEX_HPP
#define PROSTOMOLOTOV_A_S_25_17_COMPLEX_HPP

#include <iostream>

struct Complex {
    Complex() = default;
    Complex(const Complex&) = default;
    Complex(Complex&&) = default;
    explicit Complex(const double real) : re(real) {}
    Complex(const double real, const double imaginary) : re(real), im(imaginary) {}

    Complex& operator=(const Complex&) = default;
    Complex& operator=(Complex&&) = default;

    ~Complex() = default;

    [[nodiscard]] Complex operator-() const noexcept;

    [[nodiscard]] bool operator==(const Complex& rhs) const noexcept;
    [[nodiscard]] bool operator!=(const Complex& rhs) const noexcept;

    Complex& operator+=(const Complex& rhs) noexcept;
    Complex& operator+=(const double rhs) noexcept;
    Complex& operator-=(const Complex& rhs) noexcept;
    Complex& operator-=(const double rhs) noexcept;
    Complex& operator*=(const Complex& rhs) noexcept;
    Complex& operator*=(const double rhs) noexcept;
    Complex& operator/=(const Complex& rhs);
    Complex& operator/=(const double rhs);

    std::ostream& write_to(std::ostream& ostrm) const noexcept;
    std::istream& read_from(std::istream& istrm) noexcept;

    double re{ 0.0 };
    double im{ 0.0 };

    static const char left_brace{ '{' };
    static const char separator{ ',' };
    static const char right_brace{ '}' };
};

[[nodiscard]] Complex operator+(const Complex& lhs, const Complex& rhs) noexcept;
[[nodiscard]] Complex operator+(const Complex& lhs, const double rhs) noexcept;
[[nodiscard]] Complex operator+(const double lhs, const Complex& rhs) noexcept;
[[nodiscard]] Complex operator-(const Complex& lhs, const Complex& rhs) noexcept;
[[nodiscard]] Complex operator-(const Complex& lhs, const double rhs) noexcept;
[[nodiscard]] Complex operator-(const double lhs, const Complex& rhs) noexcept;
[[nodiscard]] Complex operator*(const Complex& lhs, const Complex& rhs) noexcept;
[[nodiscard]] Complex operator*(const Complex& lhs, const double rhs) noexcept;
[[nodiscard]] Complex operator*(const double lhs, const Complex& rhs) noexcept;
[[nodiscard]] Complex operator/(const Complex& lhs, const Complex& rhs);
[[nodiscard]] Complex operator/(const Complex& lhs, const double rhs);
[[nodiscard]] Complex operator/(const double lhs, const Complex& rhs);

inline std::ostream& operator<<(std::ostream& ostrm, const Complex& rhs) noexcept {
    return rhs.write_to(ostrm);
}

inline std::istream& operator>>(std::istream& istrm, Complex& rhs) noexcept {
    return rhs.read_from(istrm);
}

#endif //PROSTOMOLOTOV_A_S_25_17_COMPLEX_HPP