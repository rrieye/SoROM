#include <iostream>
#include "BigInt.h"

using namespace std;

int main()
{
    cout << "--- Lab 2: Modular Arithmetic ---" << "\n";
    
    BigInt a("1A2B3C4D5E6F");
    BigInt b("F6E5D4C3B2A1");
    
    cout << "a = " << a.toHex() << "\n";
    cout << "b = " << b.toHex() << "\n";
    
    BigInt gcd_res = a.gcd(b);
    cout << "GCD(a, b) = " << gcd_res.toHex() << "\n";
    
    return 0;
}