#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cstdint>

class BigInt
{
public:
    static constexpr size_t WORDS = 32;

    std::array<uint32_t, WORDS> words{};

    BigInt() = default;
    BigInt(uint32_t value);
    BigInt(const std::string& hexStr);

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;

    BigInt operator*(const BigInt& other) const;
    BigInt operator*(uint32_t other) const;
    
    BigInt square() const;

    BigInt operator/(const BigInt& other) const;
    BigInt operator%(const BigInt& other) const;

    BigInt power(const BigInt& exp) const;

    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;

    int bitLength() const;
    BigInt shiftBitsHigh(int shift) const;
    BigInt shiftBitsLow(int shift) const;

    std::string toHex() const;
    std::string toBinary() const;
    std::string toDecimal() const;
    friend std::ostream& operator<<(std::ostream& os, const BigInt& number);
};