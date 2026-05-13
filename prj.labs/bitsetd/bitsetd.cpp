#include "bitsetd.hpp"

#include <algorithm>
#include <stdexcept>

BitsetD::BitsetD(const std::int32_t size, const bool val) : size_(size), chunks_(chunk_count()) {
    if (size <= 0) { throw std::invalid_argument("Size cannot be 0 or below 0"); }
    fill(val);
}

BitsetD::BitsetD(const std::uint64_t mask, const std::int32_t size)
    : size_(size), chunks_(chunk_count())
{
    if (size <= 0) { throw std::invalid_argument("Size cannot be 0 or below 0"); }
    chunks_[0] = static_cast<std::uint32_t>(mask);
    if (chunk_count() >= 2) {
        chunks_[1] = static_cast<std::uint32_t>(mask >> 32);
    }
    trim_unused_bits();
}

bool BitsetD::get(const std::uint32_t idx) const {
    if (idx >= static_cast<std::uint32_t>(size_)) { throw std::out_of_range("Index " + std::to_string(idx) + " is out of bounds"); }

    return chunks_[idx / chunks_bit_size] & 1U << (idx % chunks_bit_size);
}

void BitsetD::set(const std::uint32_t idx, const bool val) {
    if (idx >= static_cast<std::uint32_t>(size_)) { throw std::out_of_range("Index " + std::to_string(idx) + " is out of bounds"); }

    const size_t chunk_idx = idx / chunks_bit_size;
    const size_t bit_pos = idx % chunks_bit_size;
    const std::uint32_t a = 1U << bit_pos;
    if (val) { chunks_[chunk_idx] |= a; }
    else { chunks_[chunk_idx] &= ~a; }
}


BitsetD& BitsetD::invert() {
    for (std::uint32_t& chunk : chunks_) { chunk = ~chunk; }
    trim_unused_bits();
    return *this;
}

void BitsetD::trim_unused_bits() noexcept {
    if (size_ % chunks_bit_size != 0) { chunks_.back() &= (1U << (size_ % chunks_bit_size)) - 1; }
}

void BitsetD::fill(const bool val) noexcept {
    auto a = 0xFFFFFFFF;
    if (!val) { a = 0x0; }
    std::fill(chunks_.begin(), chunks_.end(), a);
    trim_unused_bits();
}

void BitsetD::resize(const std::int32_t new_size, const bool val) {
    if (new_size <= 0) { throw std::invalid_argument("Size cannot be 0 or below 0"); }
    const size_t new_chunks_count = (new_size - 1) / chunks_bit_size + 1;
    if (new_size > size_) {
        if (val) {
            if (size_ % chunks_bit_size != 0) { chunks_.back() |= ~((1U << (size_ % chunks_bit_size)) - 1); }
            chunks_.resize(new_chunks_count, 0xFFFFFFFF);
        }
        else { chunks_.resize(new_chunks_count, 0x0); }
    }
    else { chunks_.resize(new_size); }

    size_ = new_size;

    trim_unused_bits();
}

BitsetD& BitsetD::operator<<=(const std::uint32_t shift) {
    if (shift <= 0) return *this;
    if (shift >= size_) { fill(false); }

    const std::uint32_t chunk_shift = shift / chunks_bit_size;
    const std::uint32_t bit_shift = shift % chunks_bit_size;

    if (chunk_shift) {
        for (int i = chunk_count() - 1; i >= 0; --i) {
            if (i >= chunk_shift) { chunks_[i] = chunks_[i - chunk_shift]; }
            else { chunks_[i] = 0x0; }
        }
    }

    if (bit_shift) {
        for (size_t i = chunk_count() - 1; i > 0; --i) {
            chunks_[i] = chunks_[i] << bit_shift | chunks_[i - 1] >> (chunks_bit_size - bit_shift);
        }
        chunks_[0] <<= bit_shift;
    }

    trim_unused_bits();
    return *this;
}

BitsetD& BitsetD::operator>>=(const std::uint32_t shift) {
    if (shift <= 0) return *this;
    if (shift >= size_) { fill(false); }

    const std::uint32_t chunk_shift = shift / chunks_bit_size;
    const std::uint32_t bit_shift = shift % chunks_bit_size;

    if (chunk_shift) {
        for (size_t i = 0; i < chunk_count(); ++i) {
            if (i + chunk_shift <= chunk_count()) { chunks_[i] = chunks_[i + chunk_shift]; }
            else { chunks_[i] = 0x0; }
        }
    }

    if (bit_shift) {
        for (size_t i = 0; i < chunk_count() - 1; ++i) {
            chunks_[i] = chunks_[i] >> bit_shift | chunks_[i + 1] << (chunks_bit_size - bit_shift);
        }
        chunks_.back() >>= bit_shift;
    }

    trim_unused_bits();
    return *this;
}

bool BitsetD::operator==(const BitsetD& rhs) const {
    if (size_ != rhs.size_) return false;

    for (size_t i = 0; i < chunk_count(); i++) { if (chunks_[i] != rhs.chunks_[i]) { return false; } }

    return true;
}

BitsetD& BitsetD::shift(const std::int32_t s) {
    if (size_ <= 1) return *this;

    std::int32_t norm_s = s % size_;
    if (norm_s == 0) return *this;
    if (norm_s < 0) norm_s += size_;

    BitsetD left_part = *this;
    left_part <<= norm_s;

    BitsetD right_part = *this;
    right_part >>= size_ - norm_s;

    for (size_t i = 0; i < chunk_count(); i++) {
        chunks_[i] = left_part.chunks_[i] | right_part.chunks_[i];
    }

    trim_unused_bits();
    return *this;
}

BitsetD& BitsetD::operator&=(const BitsetD& rhs) {
    if (size_ != rhs.size_) throw std::invalid_argument("Sizes are not equal");
    for (size_t i = 0; i < chunk_count(); i++) { chunks_[i] &= rhs.chunks_[i]; }

    return *this;
}

BitsetD& BitsetD::operator^=(const BitsetD& rhs) {
    if (size_ != rhs.size_) throw std::invalid_argument("Sizes are not equal");
    for (size_t i = 0; i < chunk_count(); i++) { chunks_[i] ^= rhs.chunks_[i]; }

    return *this;
}

BitsetD& BitsetD::operator|=(const BitsetD& rhs) {
    if (size_ != rhs.size_) throw std::invalid_argument("Sizes are not equal");
    for (size_t i = 0; i < chunk_count(); i++) { chunks_[i] |= rhs.chunks_[i]; }

    return *this;
}

std::string BitsetD::to_string(const BitsetD::StrFormat fmt, const int32_t len) const {
    if (size_ == 0) return "empty";

    switch (fmt) {
        case StrFormat::BinNoPreSep: {
            const int32_t bit_len = std::max(size_, len);
            std::string result(static_cast<size_t>(bit_len), '0');
            for (int32_t i = 0; i < size_; i++) {
                if (get(static_cast<uint32_t>(i))) result[bit_len - 1 - i] = '1';
            }
            return result;
        }
        case StrFormat::Bin: {
            const int32_t bit_len = std::max(size_, len);
            std::string bits(static_cast<size_t>(bit_len), '0');
            for (int32_t i = 0; i < size_; i++) {
                if (get(static_cast<uint32_t>(i))) bits[bit_len - 1 - i] = '1';
            }
            std::string result = "b";
            for (int32_t i = 0; i < bit_len; i++) {
                result += bits[i];
                const int32_t from_right = bit_len - 1 - i;
                if (from_right > 0 && from_right % 4 == 0) result += '\'';
            }
            return result;
        }
        case StrFormat::Oct: {
            const int32_t oct_digits = (size_ + 2) / 3;
            const int32_t oct_len = std::max(oct_digits, len);
            std::string result(static_cast<size_t>(oct_len), '0');
            for (int32_t i = 0; i < oct_digits; i++) {
                uint32_t v = 0;
                for (int32_t b = 0; b < 3; b++) {
                    const int32_t bit_idx = i * 3 + b;
                    if (bit_idx < size_ && get(static_cast<uint32_t>(bit_idx))) v |= 1U << b;
                }
                result[oct_len - 1 - i] = static_cast<char>('0' + v);
            }
            return result;
        }
        case StrFormat::Hex: {
            const int32_t hex_digits = (size_ + 3) / 4;
            const int32_t hex_len = std::max(hex_digits, len);
            std::string result(static_cast<size_t>(hex_len), '0');
            constexpr char hex_chars[] = "0123456789abcdef";
            for (int32_t i = 0; i < hex_digits; i++) {
                uint32_t v = 0;
                for (int32_t b = 0; b < 4; b++) {
                    const int32_t bit_idx = i * 4 + b;
                    if (bit_idx < size_ && get(static_cast<uint32_t>(bit_idx))) v |= 1U << b;
                }
                result[hex_len - 1 - i] = hex_chars[v];
            }
            return result;
        }
        default:
            return "empty";
    }
}
