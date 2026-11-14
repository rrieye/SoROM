#include <iostream>
#include <string>
#include "tests.h"
#include "benchmarks.h"
#include "BigInt.h"

using namespace std;

int main()
{
    // runTests();
    // runBenchmarks(); 

    BigInt B("202171731bd25f23cb5723b759bce0c00632334c4e9917fc14be434bf9db4b8653c39df5c563ea9c5e314de587a68261a407ad8512ba79993b280022e269ed74f37bfe8e74bafb86682da9034d76c812556e60745617ca65ab62ea46746ef58b96c852f237bf18c53e30f21eacdc84fd42ebaad5c247490cb7d55e879a12707c");
    BigInt A("25");
    BigInt N("80642ec8fb4a036035a01b3cf673429799e40121791a67c1472cd8695df87ec163d665aad34a4a21182d50d71fa858344d0f054f189053f07736368edecbd043f666c204a310abb448ec0e8af0c9f263050dfe95ed4f77f559b94fab131543b73358ca123c0e47547b9af637991eb3ad0af8cf58ce6f17d1a83512d1845108b8");
    BigInt C("7234567890ABCDEF7234567890ABCDEF7234567890ABCDEF7234567890ABCDEF7234567890ABCDEF7234567890ABCDEF7234567890ABCDEF7234567890ABCDEF");
    
    BigInt mu = N.getBarrettMu();

    cout << "A + B =\n" << (A + B).toHex() << "\n\n";
    cout << "B - A =\n" << (B - A).toHex() << "\n\n";
    cout << "A * B =\n" << (A * B).toHex() << "\n\n";
    cout << "A / C =\n" << (A / C).toHex() << "\n\n";
    cout << "A mod C =\n" << (A % C).toHex() << "\n\n";

    cout << "(A + B) mod N =\n" << A.addMod(B, N).toHex() << "\n\n";
    cout << "(A - B) mod N =\n" << A.subMod(B, N).toHex() << "\n\n";
    cout << "(A * B) mod N (Barrett Reduction) =\n" << A.mulMod(B, N, mu).toHex() << "\n\n";
    cout << "(A ^ B) mod N =\n" << A.powerMod(B, N, mu).toHex() << "\n\n";

    cout << "MILLER-RABIN PRIMALITY TEST\n";
    cout << "Is number A probably prime? " << (A.isProbablePrime(20) ? "Yes" : "No") << "\n";
    cout << "Is number B probably prime? " << (B.isProbablePrime(20) ? "Yes" : "No") << "\n\n";

    return 0;
}