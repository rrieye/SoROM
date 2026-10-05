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

int main() {
    BigInt zero;
    BigInt small(1024);
    BigInt hexNum("1a2b3c4d5e6f7a8b9c0d1e2f3");

    std::cout << "Zero: " << zero << "\n";
    std::cout << "Small: " << small << "\n";
    std::cout << "HexNum: " << hexNum << "\n";

    return 0;
}