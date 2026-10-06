#include "tests.h"
#include "BigInt.h"
#include <iostream>

using namespace std;

void runTests()
{
    cout << "--- Running Tests ---\n";
    
    BigInt a("1122334455667788");
    BigInt b("99AABBCCDDEEFF00");
    BigInt c("1234567890ABCDEF");

    BigInt left1 = (a + b) * c;
    BigInt right1 = (a * c) + (b * c);
    cout << "Test 1 [(a+b)*c == a*c + b*c]: ";
    if (left1 == right1) cout << "Passed!\n"; else cout << "Failed!\n";

    BigInt sumResult("0");
    for (int i = 0; i < 100; ++i)
    {
        sumResult = sumResult + a;
    }
    BigInt mulResult = a * 100;
    cout << "Test 2 [100*a == a+a+...+a]: ";
    if (sumResult == mulResult) cout << "Passed!\n"; else cout << "Failed!\n";

    cout << "Test 3 [a+b == b+a]: ";
    if ((a + b) == (b + a)) cout << "Passed!\n"; else cout << "Failed!\n";

    cout << "Test 4 [a*b == b*a]: ";
    if ((a * b) == (b * a)) cout << "Passed!\n"; else cout << "Failed!\n";

    BigInt divResult = a / c;
    BigInt modResult = a % c;
    BigInt restoredA = (c * divResult) + modResult;
    cout << "Test 5 [A == B*(A/B) + (A%B)]: ";
    if (a == restoredA) cout << "Passed!\n"; else cout << "Failed!\n";

    BigInt exponent("3");
    BigInt powerResult = a.power(exponent);
    BigInt manualPower = a * a * a;
    cout << "Test 6 [a^3 == a*a*a]: ";
    if (powerResult == manualPower) cout << "Passed!\n"; else cout << "Failed!\n";

    cout << "------------------------------\n\n";
}