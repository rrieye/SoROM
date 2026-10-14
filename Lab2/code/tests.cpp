#include "tests.h"
#include "BigInt.h"
#include <iostream>

using namespace std;

bool testGCD()
{
    BigInt a("30");
    BigInt b("12");
    BigInt expected("6");
    BigInt res = a.gcd(b);
    if (res == expected)
    {
        cout << "   [PASS] GCD\n";
        return true;
    }
    cout << "   [FAIL] GCD\n";
    return false;
}

bool testLCM()
{
    BigInt a("15");
    BigInt b("6");
    BigInt expected("2A");
    BigInt res = a.lcm(b);
    if (res == expected)
    {
        cout << "   [PASS] LCM\n";
        return true;
    }
    cout << "   [FAIL] LCM\n";
    return false;
}

bool testAddMod()
{
    BigInt a("F");
    BigInt b("14");
    BigInt n("1E");
    BigInt expected("5");
    BigInt res = a.addMod(b, n);
    if (res == expected)
    {
        cout << "   [PASS] addMod\n";
        return true;
    }
    cout << "   [FAIL] addMod\n";
    return false;
}

bool testSubMod()
{
    BigInt a("F");
    BigInt b("14");
    BigInt n("1E");
    BigInt expected("19");
    BigInt res = a.subMod(b, n);
    if (res == expected)
    {
        cout << "   [PASS] subMod\n";
        return true;
    }
    cout << "   [FAIL] subMod\n";
    return false;
}

bool testMulMod()
{
    BigInt a("A");
    BigInt b("B");
    BigInt n("C");
    BigInt mu = n.getBarrettMu();
    BigInt expected("2");
    BigInt res = a.mulMod(b, n, mu);
    if (res == expected)
    {
        cout << "   [PASS] mulMod\n";
        return true;
    }
    cout << "   [FAIL] mulMod\n";
    return false;
}

bool testSquareMod()
{
    BigInt a("A");
    BigInt n("C");
    BigInt mu = n.getBarrettMu();
    BigInt expected("4");
    BigInt res = a.squareMod(n, mu);
    if (res == expected)
    {
        cout << "   [PASS] squareMod\n";
        return true;
    }
    cout << "   [FAIL] squareMod\n";
    return false;
}

bool testIdentity1()
{
    BigInt a("1A2B3C4D");
    BigInt b("98765432");
    BigInt c("FEDCBA98");
    BigInt n("FFFFFFFF");
    BigInt mu = n.getBarrettMu();
    BigInt a_plus_b = a.addMod(b, n);
    BigInt lhs1 = a_plus_b.mulMod(c, n, mu);
    BigInt lhs2 = c.mulMod(a_plus_b, n, mu);
    BigInt ac = a.mulMod(c, n, mu);
    BigInt bc = b.mulMod(c, n, mu);
    BigInt rhs = ac.addMod(bc, n);
    if (lhs1 == lhs2 && lhs1 == rhs)
    {
        cout << "   [PASS] Identity 1\n";
        return true;
    }
    cout << "   [FAIL] Identity 1\n";
    return false;
}

bool testIdentity2()
{
    BigInt a("1A2B3C");
    BigInt m("FFFFFFFF");
    BigInt mu = m.getBarrettMu();
    int k = 150;
    BigInt k_big(k);
    BigInt lhs = k_big.mulMod(a, m, mu);
    BigInt rhs("0");
    for (int i = 0; i < k; ++i)
    {
        rhs = rhs.addMod(a, m);
    }
    if (lhs == rhs)
    {
        cout << "   [PASS] Identity 2\n";
        return true;
    }
    cout << "   [FAIL] Identity 2\n";
    return false;
}

bool testIdentity3()
{
    BigInt p("7FFFFFFF"); 
    BigInt phi_p("7FFFFFFE");
    BigInt mu_p = p.getBarrettMu();
    BigInt a1("12345678"); 
    BigInt res1 = a1.powerMod(phi_p, p, mu_p);
    BigInt n3("CFD41B91");
    BigInt phi_n3("8A8D67B6");
    BigInt mu_n3 = n3.getBarrettMu();
    BigInt a2("5");
    BigInt res2 = a2.powerMod(phi_n3, n3, mu_n3);
    BigInt one("1");
    if (res1 == one && res2 == one)
    {
        cout << "   [PASS] Identity 3\n";
        return true;
    }
    cout << "   [FAIL] Identity 3\n";
    return false;
}

void runTests()
{
    cout << "--- Running Tests ---\n";
    bool allPassed = true;
    
    if (!testGCD())
    {
        allPassed = false;
    }
    if (!testLCM())
    {
        allPassed = false;
    }
    if (!testAddMod())
    {
        allPassed = false;
    }
    if (!testSubMod())
    {
        allPassed = false;
    }
    if (!testMulMod())
    {
        allPassed = false;
    }
    if (!testSquareMod())
    {
        allPassed = false;
    }
    if (!testIdentity1())
    {
        allPassed = false;
    }
    if (!testIdentity2())
    {
        allPassed = false;
    }
    if (!testIdentity3())
    {
        allPassed = false;
    }
    
    cout << "----------------------\n";
    if (allPassed)
    {
        cout << "All tests ran correctly!\n\n";
    }
    else
    {
        cout << "SOME TESTS FAILED!\n\n";
    }
}