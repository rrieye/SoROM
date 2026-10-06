#include <iostream>
#include "BigInt.h"
#include "tests.h"
#include "benchmarks.h"

using namespace std;

int main()
{
    runTests();
    runBenchmarks();

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