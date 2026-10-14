#include <iostream>
#include "BigInt.h"

using namespace std;

int main()
{
    BigInt base("7A3B4C5D6E7F8A9B0C1D2E3F4A5B6C7D8E9F0A1B2C3D4E5F6A7B8C9D0E1F2A3B");
    BigInt exp("1A2B3C4D5E6F7A8B9C0D1E2F3A4B5C6D7E8F9A0B1C2D3E4F5A6B7C8D9E0F1A2B");
    BigInt n("E3F4A5B6C7D8E9F0A1B2C3D4E5F6A7B8C9D0E1F2A3B4C5D6E7F8A9B0C1D2E3F3");
    
    BigInt mu = n.getBarrettMu();
    
    BigInt resPow = base.powerMod(exp, n, mu);
    
    cout << "Base = " << base.toHex() << "\n";
    cout << "Exp  = " << exp.toHex() << "\n";
    cout << "Mod  = " << n.toHex() << "\n\n";
    cout << "Base^Exp mod Mod = " << resPow.toHex() << "\n";
    
    return 0;
}