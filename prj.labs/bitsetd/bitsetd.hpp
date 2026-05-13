#ifndef PROSTOMOLOTOV_A_S_25_17_BITSETD_H
#define PROSTOMOLOTOV_A_S_25_17_BITSETD_H

#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

class BitsetD {
public:
    class BitR {
        friend class BitsetD;
    public:
        ~BitR() = default;
        operator bool() const noexcept { return val_; }
    private:
        BitR() = delete;
        BitR(const BitR&) = delete;
        BitR(BitR&&) = delete;
        BitR(const BitsetD& bs, const std::uint32_t idx) : val_(bs.get(idx)) {}
        BitR& operator=(const BitR&) = delete;
        BitR& operator=(BitR&&) = delete;
        bool val_ = false;
    };

    class BitW {
        friend class BitsetD;
    public:
        ~BitW() = default;
        operator bool() const noexcept { return bs_.get(idx_); }
        void operator=(const bool val) noexcept { bs_.set(idx_, val); }
        void operator=(const BitW& rhs) noexcept { operator=(rhs.operator bool()); }
        void operator=(BitW&& rhs) noexcept { operator=(rhs.operator bool()); }
    private:
        BitW(BitsetD& bs, const std::uint32_t idx) : bs_(bs), idx_(idx) {}
        BitW() = delete;
        BitW(const BitW&) = delete;
        BitW(BitW&&) = delete;
        BitsetD& bs_;
        const std::uint32_t idx_ = 0;
    };

    BitsetD() = default;
    BitsetD(const BitsetD&) = default;
    BitsetD(BitsetD&&) = default;
    ~BitsetD() = default;

    BitsetD& operator=(const BitsetD& rhs) = default;
    BitsetD& operator=(BitsetD&& rhs) = default;

    explicit BitsetD(const std::int32_t size, const bool val = false);
    BitsetD(const std::uint64_t mask, const std::int32_t size);

    void resize(const std::int32_t new_size, const bool val = false);
    [[nodiscard]] explicit operator std::uint64_t() const;
    [[nodiscard]] explicit operator std::uint32_t() const { return chunks_[0]; }
    [[nodiscard]] std::int32_t size() const noexcept { return size_; }
    [[nodiscard]] bool get(const std::uint32_t idx) const;
    void set(const std::uint32_t idx, const bool val);
    void fill(const bool val) noexcept;

    BitW operator[](const std::uint32_t idx)       & { return BitW(*this, idx); }
    BitR operator[](const std::uint32_t idx) const & { return BitR(*this, idx); }

    BitsetD& invert();
    BitsetD& shift(const std::int32_t s);
    bool operator==(const BitsetD& rhs) const;

    BitsetD& operator<<=(const std::uint32_t shift);
    BitsetD& operator>>=(const std::uint32_t shift);
    BitsetD& operator&=(const BitsetD& rhs);
    BitsetD& operator|=(const BitsetD& rhs);
    BitsetD& operator^=(const BitsetD& rhs);

    enum class StrFormat : int32_t {
        Bin = 1,
        BinNoPreSep,
        Oct,
        Hex,
        Def = Bin,
        Default = Def
    };

    std::string to_string(const StrFormat fmt = StrFormat::Def, const int32_t len = 0) const;

    std::ostream& write_to(std::ostream& s) const { return s; }
    std::istream& read_from(std::istream& s) { return s; }

private:
    void trim_unused_bits() noexcept;

    static constexpr int32_t chunks_bit_size = 32;

    std::int32_t size_ = 0;
    std::vector<std::uint32_t> chunks_;

    [[nodiscard]] int32_t chunk_count() const {
        return (size_ + chunks_bit_size - 1) / chunks_bit_size;
    }
};

inline BitsetD::operator std::uint64_t() const {
    std::uint64_t first_two_chunks = 0x0;
    if (chunks_.size() >= 2) {
        first_two_chunks |= chunks_[1];
        first_two_chunks <<= 32;
    }
    first_two_chunks |= chunks_[0];
    return first_two_chunks;
}

inline BitsetD operator~(const BitsetD& val) noexcept {
    return BitsetD(val).invert();
}

inline BitsetD operator<<(const BitsetD& lhs, const std::int32_t shift) {
    return BitsetD(lhs).operator<<=(shift);
}

inline BitsetD operator>>(const BitsetD& lhs, const std::int32_t shift) {
    return BitsetD(lhs).operator>>=(shift);
}

inline BitsetD operator&(const BitsetD& lhs, const BitsetD& rhs) {
    return BitsetD(lhs).operator&=(rhs);
}

inline BitsetD operator|(const BitsetD& lhs, const BitsetD& rhs) {
    return BitsetD(lhs).operator|=(rhs);
}

inline BitsetD operator^(const BitsetD& lhs, const BitsetD& rhs) {
    return BitsetD(lhs).operator^=(rhs);
}

#endif //PROSTOMOLOTOV_A_S_25_17_BITSETD_H
