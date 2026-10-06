#include "BigInt.h"
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

BigInt::BigInt(uint32_t value)
{
    words[0] = value;
}

BigInt::BigInt(const std::string& hexStr)
{
    int len = hexStr.length();
    int wordIndex = 0;

    for (int i = len; i > 0; i -= 8)
    {
        if (wordIndex >= WORDS) break;

        int start = std::max(0, i - 8);
        int count = i - start;
        std::string chunk = hexStr.substr(start, count);

        words[wordIndex] = std::stoul(chunk, nullptr, 16);
        wordIndex++;
    }
}

BigInt BigInt::operator+(const BigInt& other) const
{
    BigInt result;
    uint32_t carry = 0;

    for (size_t i = 0; i < WORDS; ++i)
    {
        uint64_t temp = static_cast<uint64_t>(this->words[i]) + other.words[i] + carry;
        result.words[i] = static_cast<uint32_t>(temp & 0xFFFFFFFF);
        carry = static_cast<uint32_t>(temp >> 32);
    }
    return result;
}

BigInt BigInt::operator-(const BigInt& other) const
{
    BigInt result;
    uint32_t borrow = 0;

    for (size_t i = 0; i < WORDS; ++i)
    {
        int64_t temp = static_cast<int64_t>(this->words[i]) - other.words[i] - borrow;

        if (temp >= 0)
        {
            result.words[i] = static_cast<uint32_t>(temp);
            borrow = 0;
        }
        else
        {
            result.words[i] = static_cast<uint32_t>((1ULL << 32) + temp);
            borrow = 1;
        }
    }

    if (borrow == 1)
    {
        throw std::invalid_argument("Error: subtraction result is negative");
    }

    return result;
}

BigInt BigInt::operator*(uint32_t other) const
{
    BigInt result;
    uint32_t carry = 0;
    
    for (size_t i = 0; i < WORDS; ++i)
    {
        uint64_t temp = static_cast<uint64_t>(this->words[i]) * other + carry;
        result.words[i] = static_cast<uint32_t>(temp & 0xFFFFFFFF);
        carry = static_cast<uint32_t>(temp >> 32);
    }
    return result;
}

BigInt BigInt::square() const
{
    return (*this) * (*this);
}

BigInt BigInt::operator*(const BigInt& other) const
{
    BigInt result;
    
    for (size_t i = 0; i < WORDS; ++i)
    {
        if (this->words[i] == 0) continue;

        uint32_t carry = 0; 
        for (size_t j = 0; j < WORDS - i; ++j)
        {
            uint64_t temp = static_cast<uint64_t>(result.words[i + j]) + 
                            static_cast<uint64_t>(this->words[i]) * other.words[j] + carry;
                            
            result.words[i + j] = static_cast<uint32_t>(temp & 0xFFFFFFFF);
            carry = static_cast<uint32_t>(temp >> 32);
        }
    }
    return result;
}

BigInt BigInt::operator/(const BigInt& other) const
{
    if (other == BigInt(0))
    {
        throw std::invalid_argument("Error: division by zero");
    }

    BigInt q;
    BigInt r = *this;
    int k = other.bitLength();

    while (r >= other)
    {
        int t = r.bitLength();
        BigInt c = other.shiftBitsHigh(t - k);

        if (r < c)
        {
            t = t - 1;
            c = other.shiftBitsHigh(t - k);
        }

        r = r - c;
        BigInt powerOfTwo(1);
        q = q + powerOfTwo.shiftBitsHigh(t - k);
    }

    return q;
}

BigInt BigInt::operator%(const BigInt& other) const
{
    if (other == BigInt(0))
    {
        throw std::invalid_argument("Error: division by zero");
    }

    BigInt r = *this;
    int k = other.bitLength();

    while (r >= other)
    {
        int t = r.bitLength();
        BigInt c = other.shiftBitsHigh(t - k);

        if (r < c)
        {
            t = t - 1;
            c = other.shiftBitsHigh(t - k);
        }

        r = r - c;
    }

    return r;
}

BigInt BigInt::power(const BigInt& exp) const
{
    BigInt result(1);
    int bits = exp.bitLength();
    
    for (int i = bits - 1; i >= 0; --i)
    {
        uint32_t bit = (exp.words[i / 32] >> (i % 32)) & 1;
        
        if (bit == 1)
        {
            result = result * (*this);
        }
        if (i > 0)
        {
            result = result.square();
        }
    }
    return result;
}

bool BigInt::operator<(const BigInt& other) const
{
    int i = WORDS - 1;
    while (i >= 0 && this->words[i] == other.words[i])
    {
        i--;
    }
    if (i == -1) return false;
    return this->words[i] < other.words[i];
}

bool BigInt::operator>(const BigInt& other) const
{
    int i = WORDS - 1;
    while (i >= 0 && this->words[i] == other.words[i])
    {
        i--;
    }
    if (i == -1) return false;
    return this->words[i] > other.words[i];
}

bool BigInt::operator<=(const BigInt& other) const
{
    return *this < other || *this == other;
}

bool BigInt::operator>=(const BigInt& other) const
{
    return *this > other || *this == other;
}

bool BigInt::operator==(const BigInt& other) const
{
    int i = WORDS - 1;
    while (i >= 0 && this->words[i] == other.words[i])
    {
        i--;
    }
    return i == -1;
}

bool BigInt::operator!=(const BigInt& other) const
{
    return !(*this == other);
}

int BigInt::bitLength() const
{
    for (int i = WORDS - 1; i >= 0; --i)
    {
        if (words[i] != 0)
        {
            int length = i * 32;
            uint32_t temp = words[i];
            while (temp > 0)
            {
                length++;
                temp >>= 1;
            }
            return length;
        }
    }
    return 0;
}

BigInt BigInt::shiftBitsHigh(int shift) const
{
    BigInt result;
    int wordShift = shift / 32;
    int bitShift = shift % 32;

    if (wordShift >= WORDS) return result;

    uint32_t carry = 0;
    for (size_t i = 0; i < WORDS - wordShift; ++i)
    {
        uint32_t current = this->words[i];
        result.words[i + wordShift] = (bitShift == 0 ? current : (current << bitShift)) | carry;
        carry = (bitShift == 0 ? 0 : (current >> (32 - bitShift)));
    }
    return result;
}

BigInt BigInt::shiftBitsLow(int shift) const
{
    BigInt result;
    int wordShift = shift / 32;
    int bitShift = shift % 32;

    if (wordShift >= WORDS) return result;

    uint32_t carry = 0;
    for (int i = WORDS - 1; i >= wordShift; --i)
    {
        uint32_t current = this->words[i];
        result.words[i - wordShift] = (bitShift == 0 ? current : (current >> bitShift)) | carry;
        carry = (bitShift == 0 ? 0 : (current << (32 - bitShift)));
    }
    return result;
}

std::string BigInt::toHex() const
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    bool leadingZeros = true;

    for (int i = WORDS - 1; i >= 0; --i)
    {
        if (words[i] != 0 || !leadingZeros)
        {
            if (leadingZeros)
            {
                ss << words[i];
                leadingZeros = false;
            }
            else
            {
                ss << std::setw(8) << words[i];
            }
        }
    }

    if (leadingZeros) return "0";

    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const BigInt& number)
{
    os << number.toHex();
    return os;
}

std::string BigInt::toBinary() const
{
    int len = bitLength();
    if (len == 0) return "0";

    std::string result = "";
    for (int i = len - 1; i >= 0; --i)
    {
        uint32_t bit = (words[i / 32] >> (i % 32)) & 1;
        result += (bit == 1 ? '1' : '0');
    }
    return result;
}

std::string BigInt::toDecimal() const
{
    if (*this == BigInt(0)) return "0";

    BigInt temp = *this;
    BigInt ten("A"); 
    std::string result = "";

    while (temp > BigInt(0))
    {
        BigInt rem = temp % ten;
        result += std::to_string(rem.words[0]);
        temp = temp / ten;
    }
    
    std::reverse(result.begin(), result.end());
    return result;
}