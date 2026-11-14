#include "tests.h"
#include "FieldElement251.h"
#include <iostream>
#include <random>

using namespace std;

static FieldElement251 generateRandomElement(mt19937_64& rng)
{
    array<uint64_t, 4> w;
    for (size_t i = 0; i < 4; ++i)
    {
        w[i] = rng();
    }
    FieldElement251 elem(w);
    elem.maskUnusedBits();
    return elem;
}

void runFieldTests()
{
    mt19937_64 rng(42);

    FieldElement251 a = generateRandomElement(rng);
    FieldElement251 b = generateRandomElement(rng);
    FieldElement251 c = generateRandomElement(rng);
    FieldElement251 d = generateRandomElement(rng);

    cout << "--- OPERATIONS ---\n";
    cout << "Zero:       " << FieldElement251::zero().toHexString() << "\n";
    cout << "One:        " << FieldElement251::one().toHexString() << "\n";
    cout << "A + B:      " << (a + b).toHexString() << "\n";
    cout << "A * B:      " << (a * b).toHexString() << "\n";
    cout << "A^2:        " << a.square().toHexString() << "\n";
    cout << "Trace(A):   " << static_cast<int>(a.trace()) << "\n";

    FieldElement251 a_inv = a.invert();
    cout << "A^-1:       " << a_inv.toHexString() << "\n";
    cout << "A * A^-1:   " << (a * a_inv).toHexString() << "\n";

    FieldElement251 bin_test = FieldElement251::fromBinaryString(a.toBinaryString());
    cout << "Bin conv:   " << (bin_test == a ? "OK" : "FAIL") << "\n\n";

    cout << "--- IDENTITY CHECKS ---\n";

    FieldElement251 left_dist = (a + b) * c;
    FieldElement251 right_dist = (b * c) + (c * a);
    cout << "(a + b) * c == b * c + c * a: " << (left_dist == right_dist ? "PASSED" : "FAILED") << "\n";

    FieldElement251 left_assoc = (a * b) * c;
    FieldElement251 right_assoc = a * (b * c);
    cout << "(a * b) * c == a * (b * c):   " << (left_assoc == right_assoc ? "PASSED" : "FAILED") << "\n";

    cout << "a * b == b * a:               " << ((a * b) == (b * a) ? "PASSED" : "FAILED") << "\n";

    cout << "a + a == 0:                   " << ((a + a).isZero() ? "PASSED" : "FAILED") << "\n";

    cout << "a + 0 == a:                   " << ((a + FieldElement251::zero()) == a ? "PASSED" : "FAILED") << "\n";

    cout << "a * 1 == a:                   " << ((a * FieldElement251::one()) == a ? "PASSED" : "FAILED") << "\n";

    cout << "a * 0 == 0:                   " << ((a * FieldElement251::zero()).isZero() ? "PASSED" : "FAILED") << "\n";

    FieldElement251 inv_check = a * a.invert();
    cout << "a * a^-1 == 1:               " << (inv_check.isOne() ? "PASSED" : "FAILED") << "\n";

    cout << "(a^-1)^-1 == a:               " << (a.invert().invert() == a ? "PASSED" : "FAILED") << "\n";

    FieldElement251 sq_add_left = (a + b).square();
    FieldElement251 sq_add_right = a.square() + b.square();
    cout << "(a + b)^2 == a^2 + b^2:       " << (sq_add_left == sq_add_right ? "PASSED" : "FAILED") << "\n";

    FieldElement251 sq_check = a * a;
    cout << "a^2 == a * a:                 " << (a.square() == sq_check ? "PASSED" : "FAILED") << "\n";

    FieldElement251 tr_dist_left = a + b;
    uint8_t tr_left = tr_dist_left.trace();
    uint8_t tr_right = a.trace() ^ b.trace();
    cout << "Tr(a + b) == Tr(a) ^ Tr(b):   " << (tr_left == tr_right ? "PASSED" : "FAILED") << "\n";

    cout << "Tr(a^2) == Tr(a):             " << (a.square().trace() == a.trace() ? "PASSED" : "FAILED") << "\n";

    FieldElement251 max_exp;
    max_exp.words.fill(0xFFFFFFFFFFFFFFFFULL);
    max_exp.maskUnusedBits();
    FieldElement251 fermat_check = d.power(max_exp);
    cout << "d^(2^m - 1) == 1:             " << (fermat_check.isOne() ? "PASSED" : "FAILED") << "\n\n";
}