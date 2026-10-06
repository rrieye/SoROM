#include <iostream>
#include <array>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

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
    friend std::ostream& operator<<(std::ostream& os, const BigInt& number);
};

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
            result = result * result;
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


void runTests()
{
    cout << "--- Running Tests ---\n";
    
    BigInt a("1122334455667788");
    BigInt b("99AABBCCDDEEFF00");
    BigInt c("1234567890ABCDEF");

    BigInt left1 = (a + b) * c;
    BigInt right1 = (a * c) + (b * c);
    cout << "Test 1 [(a+b)*c == a*c + b*c]: ";
    if (left1 == right1) cout << "Passed!\n"; else cout << "Failed!\n";

    BigInt sumResult("0");
    for (int i = 0; i < 100; ++i)
    {
        sumResult = sumResult + a;
    }
    BigInt mulResult = a * 100;
    cout << "Test 2 [100*a == a+a+...+a]: ";
    if (sumResult == mulResult) cout << "Passed!\n"; else cout << "Failed!\n";

    cout << "Test 3 [a+b == b+a]: ";
    if ((a + b) == (b + a)) cout << "Passed!\n"; else cout << "Failed!\n";

    cout << "Test 4 [a*b == b*a]: ";
    if ((a * b) == (b * a)) cout << "Passed!\n"; else cout << "Failed!\n";

    BigInt divResult = a / c;
    BigInt modResult = a % c;
    BigInt restoredA = (c * divResult) + modResult;
    cout << "Test 5 [A == B*(A/B) + (A%B)]: ";
    if (a == restoredA) cout << "Passed!\n"; else cout << "Failed!\n";

    BigInt exponent("3");
    BigInt powerResult = a.power(exponent);
    BigInt manualPower = a * a * a;
    cout << "Test 6 [a^3 == a*a*a]: ";
    if (powerResult == manualPower) cout << "Passed!\n"; else cout << "Failed!\n";

    cout << "------------------------------\n\n";
}


int main()
{
    runTests();

    BigInt a("AABBCCDD11223344");
    BigInt b("1111111111111111");
    BigInt c("100");
    BigInt d("200");
    BigInt testNum("F");

    cout << "A = " << a << "\n";
    cout << "B = " << b << "\n";
    cout << "A + B = " << (a + b) << "\n";
    cout << "A - B = " << (a - b) << "\n\n";

    cout << "c < d:  " << (c < d) << " (expected 1)\n";
    cout << "d == d: " << (d == d) << " (expected 1)\n";
    cout << "c >= d: " << (c >= d) << " (expected 0)\n\n";

    cout << "testNum = " << testNum << "\n";
    cout << "bitLength: " << testNum.bitLength() << " (expected 4)\n";

    BigInt shiftedHigh = testNum.shiftBitsHigh(4);
    cout << "testNum << 4: " << shiftedHigh << " (expected f0)\n";

    BigInt shiftedLow = shiftedHigh.shiftBitsLow(4);
    cout << "shiftedHigh >> 4: " << shiftedLow << " (expected f)\n";

    BigInt mul1("100000000");
    BigInt mul2("200000000");
    cout << "\n100000000 * 200000000 = " << (mul1 * mul2) << " (expected 20000000000000000)\n";
    
    BigInt mul3("FFFFFFFF");
    BigInt mul4("FFFFFFFF");
    cout << "FFFFFFFF * FFFFFFFF = " << (mul3 * mul4) << " (expected fffffffe00000001)\n";

    BigInt div1("100000000"); 
    BigInt div2("3");
    
    cout << "\n100000000 / 3 = " << (div1 / div2) << " (expected 55555555)\n";
    cout << "100000000 % 3 = " << (div1 % div2) << " (expected 1)\n";
    
    BigInt div3("AABBCCDD11223344");
    BigInt div4("1111111111111111");
    cout << "A / B = " << (div3 / div4) << " (expected a)\n";
    cout << "A % B = " << (div3 % div4) << " (expected 1122326677889a)\n";

    BigInt base("5");
    BigInt exponent("13");
    
    cout << "\n5 ^ 13 = " << base.power(exponent) << " (expected 1158e460913d)\n";

    return 0;
}