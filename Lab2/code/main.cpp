#include <iostream>
#include "BigInt.h"

using namespace std;

int main()
{
    BigInt a("15");
    BigInt b("20");
    BigInt n("30");
    
    BigInt resAdd = a.addMod(b, n);
    cout << "15 + 20 mod 30 = " << resAdd.toDecimal() << "\n";
    
    BigInt resSub1 = b.subMod(a, n);
    cout << "20 - 15 mod 30 = " << resSub1.toDecimal() << "\n";
    
    BigInt resSub2 = a.subMod(b, n);
    cout << "15 - 20 mod 30 = " << resSub2.toDecimal() << "\n";
    
    return 0;
}