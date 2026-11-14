#ifndef FIELD_ELEMENT_251_H
#define FIELD_ELEMENT_251_H

#include <iostream>
#include <string>
#include <array>
#include <cstdint>

class FieldElement251
{
public:
    static constexpr size_t M = 251;
    static constexpr size_t NUM_WORDS = 4;
    
    std::array<uint64_t, NUM_WORDS> words;

    FieldElement251();
    explicit FieldElement251(uint64_t val);
    explicit FieldElement251(const std::array<uint64_t, NUM_WORDS>& w);
    explicit FieldElement251(const std::string& hex_or_bin_str, bool is_binary = false);

    static FieldElement251 zero();
    static FieldElement251 one();

    bool isZero() const;
    bool isOne() const;
    bool operator==(const FieldElement251& other) const;
    bool operator!=(const FieldElement251& other) const;

    FieldElement251 operator+(const FieldElement251& other) const;
    FieldElement251 operator-(const FieldElement251& other) const;
    FieldElement251& operator+=(const FieldElement251& other);
    FieldElement251& operator-=(const FieldElement251& other);

    FieldElement251 operator*(const FieldElement251& other) const;
    FieldElement251& operator*=(const FieldElement251& other);

    FieldElement251 square() const;
    FieldElement251 power(const FieldElement251& exponent) const;
    FieldElement251 power(uint64_t exponent) const;
    FieldElement251 invert() const;
    uint8_t trace() const;

    int getBit(size_t index) const;
    void setBit(size_t index, int value);
    void maskUnusedBits();

    std::string toBinaryString() const;
    std::string toHexString() const;
    static FieldElement251 fromBinaryString(const std::string& str);
    static FieldElement251 fromHexString(const std::string& str);

    friend std::ostream& operator<<(std::ostream& os, const FieldElement251& elem);

private:
    static void reducePoly500(std::array<uint64_t, 8>& wide_poly, FieldElement251& result);
};

#endif