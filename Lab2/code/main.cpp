#include <iostream>
#include <string>
#include "tests.h"
#include "benchmarks.h"
#include "BigInt.h"

using namespace std;

int main()
{
    runTests();
    // runBenchmarks(); 

    string strA = "1234567890ABCDEF";
    string strB = "FEDCBA0987654321";
    string strN = "FFFFFFFFFFFFFFFF";
    string strExp = "10001";
    
    BigInt a(strA);
    BigInt b(strB);
    BigInt n(strN);
    BigInt exp(strExp);
    
    BigInt mu = n.getBarrettMu();

    cout << "A = " << a.toHex() << "\n";
    cout << "B = " << b.toHex() << "\n";
    cout << "N = " << n.toHex() << "\n\n";

    // BigInt resGcd = a.gcd(b);
    // cout << "GCD(A, B) = " << resGcd.toHex() << "\n";

    // BigInt resLcm = a.lcm(b);
    // cout << "LCM(A, B) = " << resLcm.toHex() << "\n";

    // BigInt resAdd = a.addMod(b, n);
    // cout << "(A + B) mod N = " << resAdd.toHex() << "\n";

    // BigInt resSub = a.subMod(b, n);
    // cout << "(A - B) mod N = " << resSub.toHex() << "\n";

    // BigInt resMul = a.mulMod(b, n, mu);
    // cout << "(A * B) mod N = " << resMul.toHex() << "\n";

    // BigInt resSquare = a.squareMod(n, mu);
    // cout << "(A ^ 2) mod N = " << resSquare.toHex() << "\n";

    // BigInt resPow = a.powerMod(exp, n, mu);
    // cout << "(A ^ exp) mod N = " << resPow.toHex() << "\n";

    // bool isPrime = a.isProbablePrime(20);
    // cout << "Is A prime? " << (isPrime ? "Yes" : "No") << "\n";

    return 0;
}