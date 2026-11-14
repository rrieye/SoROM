#include <iostream>
#include "BigInt.h"
#include "tests.h"
#include "benchmarks.h"

using namespace std;

int main()
{
    runTests();
    runBenchmarks();

    BigInt a("ada300c1d4c601c7e603aed263464ccad55b7e7ad795986e8e32e13b0a7d2c619af3f83e278ff80f98ce8fa3f0f0088ca49b53b6563f3f74969ce2906118bfba6162255e750dc922613cb7e31d959074be02a4411f15093f383fb2e61b7229512c5d5651054700b0e3d93b6e514b71115e9ec8ce584ed8da15587c5560eb99d6");
    BigInt b("2ce123c0661d00fd74343d807bc252f16c50ab0764be6797c1be67d82384903294620aafdd926b0c3c762313a12bc5e00f83179c9958d2c9b79f3597b3ca9c3f6dc97ac6d927ad440745326531a0680193e82b5cae5ac09deaff2ae5c62e392480755d124387b477e2a2d4b3cd872fd7acff13a05d4acc720a2b857dfe58bebd");
    BigInt c("a7b5037fb553ead1e7c2f200af68bc80c0abd5fc4d2e4a6db0aa8aa16a27d06f2fc7b6182fdcc3ea38b78031d2ac14a306f6c1f2c787745fa90d3e325ec06b8e");
    BigInt d("200");
    BigInt testNum("F");


    cout << "A = " << a << "\n";
    cout << "B = " << b << "\n";
    cout << "A + B = " << (a + b) << "\n";
    cout << "A - B = " << (a - b) << "\n\n";
    cout << "A * B = " << (a * b) << "\n\n";
    cout << "A / C = " << (a / c) << "\n\n";
    cout << "A / C = " << (a % c) << "\n\n";



    return 0;
}