#include "FieldElement251.h"
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <wmmintrin.h>
#include <immintrin.h>

FieldElement251::FieldElement251()
{
    words.fill(0);
}

FieldElement251::FieldElement251(uint64_t val)
{
    words.fill(0);
    words[0] = val;
    maskUnusedBits();
}

FieldElement251::FieldElement251(const std::array<uint64_t, NUM_WORDS>& w)
{
    words = w;
    maskUnusedBits();
}

FieldElement251::FieldElement251(const std::string& hex_or_bin_str, bool is_binary)
{
    words.fill(0);
    if (is_binary)
    {
        *this = fromBinaryString(hex_or_bin_str);
    }
    else
    {
        *this = fromHexString(hex_or_bin_str);
    }
}

FieldElement251 FieldElement251::zero()
{
    return FieldElement251();
}

FieldElement251 FieldElement251::one()
{
    return FieldElement251(1ULL);
}

bool FieldElement251::isZero() const
{
    for (size_t i = 0; i < NUM_WORDS; ++i)
    {
        if (words[i] != 0)
        {
            return false;
        }
    }
    return true;
}

bool FieldElement251::isOne() const
{
    if (words[0] != 1)
    {
        return false;
    }
    for (size_t i = 1; i < NUM_WORDS; ++i)
    {
        if (words[i] != 0)
        {
            return false;
        }
    }
    return true;
}

bool FieldElement251::operator==(const FieldElement251& other) const
{
    return words == other.words;
}

bool FieldElement251::operator!=(const FieldElement251& other) const
{
    return words != other.words;
}

void FieldElement251::maskUnusedBits()
{
    words[3] &= 0x07FFFFFFFFFFFFFFULL;
}

int FieldElement251::getBit(size_t index) const
{
    if (index >= M)
    {
        return 0;
    }
    size_t word_idx = index / 64;
    size_t bit_idx = index % 64;
    return (words[word_idx] >> bit_idx) & 1ULL;
}

void FieldElement251::setBit(size_t index, int value)
{
    if (index >= M)
    {
        return;
    }
    size_t word_idx = index / 64;
    size_t bit_idx = index % 64;
    if (value)
    {
        words[word_idx] |= (1ULL << bit_idx);
    }
    else
    {
        words[word_idx] &= ~(1ULL << bit_idx);
    }
}

FieldElement251 FieldElement251::operator+(const FieldElement251& other) const
{
    FieldElement251 res;
    for (size_t i = 0; i < NUM_WORDS; ++i)
    {
        res.words[i] = words[i] ^ other.words[i];
    }
    return res;
}

FieldElement251 FieldElement251::operator-(const FieldElement251& other) const
{
    return *this + other;
}

FieldElement251& FieldElement251::operator+=(const FieldElement251& other)
{
    for (size_t i = 0; i < NUM_WORDS; ++i)
    {
        words[i] ^= other.words[i];
    }
    return *this;
}

FieldElement251& FieldElement251::operator-=(const FieldElement251& other)
{
    return *this += other;
}

void FieldElement251::reducePoly500(std::array<uint64_t, 8>& wide, FieldElement251& result)
{
    for (int i = 7; i >= 3; --i)
    {
        while (wide[i])
        {
            int bit_idx = 63 - __builtin_clzll(wide[i]);
            int abs_bit = i * 64 + bit_idx;
            
            if (abs_bit < 251)
            {
                break;
            }
            
            wide[i] ^= (1ULL << bit_idx);
            
            int target = abs_bit - 251;
            
            wide[target / 64] ^= (1ULL << (target % 64));
            
            int t1 = target + 1;
            wide[t1 / 64] ^= (1ULL << (t1 % 64));
            
            int t4 = target + 4;
            wide[t4 / 64] ^= (1ULL << (t4 % 64));
            
            int t14 = target + 14;
            wide[t14 / 64] ^= (1ULL << (t14 % 64));
        }
    }

    for (size_t i = 0; i < NUM_WORDS; ++i)
    {
        result.words[i] = wide[i];
    }
    result.maskUnusedBits();
}

FieldElement251 FieldElement251::operator*(const FieldElement251& other) const
{
    std::array<uint64_t, 8> wide;
    wide.fill(0);

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (words[i] == 0 || other.words[j] == 0)
            {
                continue;
            }
            
            __m128i a = _mm_set_epi64x(0, words[i]);
            __m128i b = _mm_set_epi64x(0, other.words[j]);
            __m128i prod = _mm_clmulepi64_si128(a, b, 0x00);
            
            uint64_t p[2];
            _mm_storeu_si128((__m128i*)p, prod);
            
            wide[i + j] ^= p[0];
            wide[i + j + 1] ^= p[1];
        }
    }

    FieldElement251 res;
    reducePoly500(wide, res);
    return res;
}

FieldElement251& FieldElement251::operator*=(const FieldElement251& other)
{
    *this = *this * other;
    return *this;
}

FieldElement251 FieldElement251::square() const
{
    std::array<uint64_t, 8> wide;
    wide.fill(0);

    for (size_t i = 0; i < NUM_WORDS; ++i)
    {
        wide[2 * i] = _pdep_u64(words[i], 0x5555555555555555ULL);
        wide[2 * i + 1] = _pdep_u64(words[i] >> 32, 0x5555555555555555ULL);
    }

    FieldElement251 res;
    reducePoly500(wide, res);
    return res;
}

FieldElement251 FieldElement251::power(uint64_t exponent) const
{
    FieldElement251 res = FieldElement251::one();
    FieldElement251 base = *this;

    while (exponent > 0)
    {
        if (exponent & 1ULL)
        {
            res *= base;
        }
        base = base.square();
        exponent >>= 1ULL;
    }
    return res;
}

FieldElement251 FieldElement251::power(const FieldElement251& exponent) const
{
    FieldElement251 res = FieldElement251::one();
    int highest_bit = -1;

    for (int i = static_cast<int>(M) - 1; i >= 0; --i)
    {
        if (exponent.getBit(i))
        {
            highest_bit = i;
            break;
        }
    }

    if (highest_bit == -1)
    {
        return res;
    }

    for (int i = highest_bit; i >= 0; --i)
    {
        res = res.square();
        if (exponent.getBit(i))
        {
            res *= *this;
        }
    }

    return res;
}

static FieldElement251 power2k(const FieldElement251& a, int k)
{
    FieldElement251 res = a;
    for (int i = 0; i < k; ++i)
    {
        res = res.square();
    }
    return res;
}

FieldElement251 FieldElement251::invert() const
{
    if (isZero())
    {
        return zero();
    }
    
    FieldElement251 S1 = *this;
    FieldElement251 S2 = S1 * power2k(S1, 1);
    FieldElement251 S3 = S2 * power2k(S1, 2);
    FieldElement251 S6 = S3 * power2k(S3, 3);
    FieldElement251 S12 = S6 * power2k(S6, 6);
    FieldElement251 S24 = S12 * power2k(S12, 12);
    FieldElement251 S25 = S24 * power2k(S1, 24);
    FieldElement251 S50 = S25 * power2k(S25, 25);
    FieldElement251 S100 = S50 * power2k(S50, 50);
    FieldElement251 S125 = S100 * power2k(S25, 100);
    FieldElement251 S250 = S125 * power2k(S125, 125);
    
    return S250.square();
}

uint8_t FieldElement251::trace() const
{
    FieldElement251 acc = *this;
    FieldElement251 cur = *this;
    for (size_t i = 1; i < M; ++i)
    {
        cur = cur.square();
        acc += cur;
    }
    return static_cast<uint8_t>(acc.words[0] & 1ULL);
}

std::string FieldElement251::toBinaryString() const
{
    std::string s;
    s.reserve(M);
    for (int i = static_cast<int>(M) - 1; i >= 0; --i)
    {
        s.push_back(getBit(i) ? '1' : '0');
    }
    return s;
}

std::string FieldElement251::toHexString() const
{
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    oss << std::setw(15) << words[3];
    for (int i = 2; i >= 0; --i)
    {
        oss << std::setw(16) << words[i];
    }
    return oss.str();
}

FieldElement251 FieldElement251::fromBinaryString(const std::string& str)
{
    FieldElement251 elem;
    size_t len = str.length();
    for (size_t i = 0; i < len && i < M; ++i)
    {
        char c = str[len - 1 - i];
        if (c == '1')
        {
            elem.setBit(i, 1);
        }
    }
    return elem;
}

FieldElement251 FieldElement251::fromHexString(const std::string& str)
{
    FieldElement251 elem;
    size_t len = str.length();
    size_t char_idx = 0;

    for (size_t i = 0; i < NUM_WORDS; ++i)
    {
        uint64_t w = 0;
        for (size_t h = 0; h < 16; ++h)
        {
            if (char_idx < len)
            {
                char c = str[len - 1 - char_idx];
                uint64_t nibble = 0;
                if (c >= '0' && c <= '9')
                {
                    nibble = c - '0';
                }
                else if (c >= 'a' && c <= 'f')
                {
                    nibble = c - 'a' + 10;
                }
                else if (c >= 'A' && c <= 'F')
                {
                    nibble = c - 'A' + 10;
                }
                w |= (nibble << (4 * h));
                char_idx++;
            }
        }
        elem.words[i] = w;
    }
    elem.maskUnusedBits();
    return elem;
}

std::ostream& operator<<(std::ostream& os, const FieldElement251& elem)
{
    os << elem.toHexString();
    return os;
}