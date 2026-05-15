#pragma once
#ifndef PROSTOMOLOTOV_A_S_25_17_ARRAYD_HPP
#define PROSTOMOLOTOV_A_S_25_17_ARRAYD_HPP

#include <cstddef>

class ArrayD {
public:
    ArrayD() = default;
    ArrayD(const ArrayD& src);
    explicit ArrayD(const std::ptrdiff_t size);
    ~ArrayD();

    ArrayD& operator=(const ArrayD& rhs);

    [[nodiscard]] std::ptrdiff_t size() const noexcept { return size_; }

    void resize(const std::ptrdiff_t size);

    [[nodiscard]] float& operator[](const std::ptrdiff_t idx);
    [[nodiscard]] float  operator[](const std::ptrdiff_t idx) const;

    void insert(const std::ptrdiff_t idx, const float val);
    void remove(const std::ptrdiff_t idx);

private:
    std::ptrdiff_t capacity_ = 0;
    std::ptrdiff_t size_     = 0;
    float*         data_     = nullptr;
};

#endif
