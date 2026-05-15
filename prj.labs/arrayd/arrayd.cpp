#include "arrayd.hpp"

#include <cstring>
#include <stdexcept>

ArrayD::ArrayD(const ArrayD& src)
    : capacity_(src.size_)
    , size_(src.size_) {
    if (size_ > 0) {
        data_ = new float[size_];
        std::memcpy(data_, src.data_, size_ * sizeof(float));
    }
}

ArrayD::ArrayD(const std::ptrdiff_t size) {
    if (size <= 0) {
        throw std::invalid_argument("ArrayD: non-positive size");
    }
    data_     = new float[size]{};
    capacity_ = size;
    size_     = size;
}

ArrayD::~ArrayD() {
    delete[] data_;
}

ArrayD& ArrayD::operator=(const ArrayD& rhs) {
    if (this == &rhs) return *this;
    if (capacity_ < rhs.size_) {
        delete[] data_;
        data_     = rhs.size_ > 0 ? new float[rhs.size_] : nullptr;
        capacity_ = rhs.size_;
    }
    size_ = rhs.size_;
    if (size_ > 0) {
        std::memcpy(data_, rhs.data_, size_ * sizeof(float));
    }
    return *this;
}

void ArrayD::resize(const std::ptrdiff_t size) {
    if (size < 0) {
        throw std::invalid_argument("ArrayD: negative size");
    }
    if (capacity_ < size) {
        float* new_data = new float[size]{};
        if (size_ > 0) {
            std::memcpy(new_data, data_, size_ * sizeof(float));
        }
        delete[] data_;
        data_     = new_data;
        capacity_ = size;
    } else if (size_ < size) {
        std::memset(data_ + size_, 0, (size - size_) * sizeof(float));
    }
    size_ = size;
}

float& ArrayD::operator[](const std::ptrdiff_t idx) {
    if (idx < 0 || idx >= size_) {
        throw std::out_of_range("ArrayD: index out of range");
    }
    return data_[idx];
}

float ArrayD::operator[](const std::ptrdiff_t idx) const {
    if (idx < 0 || idx >= size_) {
        throw std::out_of_range("ArrayD: index out of range");
    }
    return data_[idx];
}

void ArrayD::insert(const std::ptrdiff_t idx, const float val) {
    if (idx < 0 || idx > size_) {
        throw std::out_of_range("ArrayD: index out of range");
    }
    resize(size_ + 1);
    if (idx < size_ - 1) {
        std::memmove(data_ + idx + 1, data_ + idx, (size_ - 1 - idx) * sizeof(float));
    }
    data_[idx] = val;
}

void ArrayD::remove(const std::ptrdiff_t idx) {
    if (idx < 0 || idx >= size_) {
        throw std::out_of_range("ArrayD: index out of range");
    }
    if (idx < size_ - 1) {
        std::memmove(data_ + idx, data_ + idx + 1, (size_ - 1 - idx) * sizeof(float));
    }
    resize(size_ - 1);
}
