#include <iostream>
#include "BigInt.h"

using namespace std;

int main()
{
    BigInt a("1A2B3C4D5E6F7A8B9C0D1E2F3A4B5C6D");
    BigInt b("FEDCBA9876543210FEDCBA9876543210");
    BigInt n("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE");
    
    BigInt mu = n.getBarrettMu();
    
    cout << "A = " << a.toHex() << "\n";
    cout << "B = " << b.toHex() << "\n";
    cout << "N = " << n.toHex() << "\n\n";
    
    BigInt resMul = a.mulMod(b, n, mu);
    cout << "A * B mod N = " << resMul.toHex() << "\n";
    
    BigInt resSq = a.squareMod(n, mu);
    cout << "A^2 mod N   = " << resSq.toHex() << "\n";
    
    return 0;
}