#include "benchmarks.h"
#include "BigInt.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace std::chrono;

void runBenchmarks()
{
    BigInt a("7A3B4C5D6E7F8A9B0C1D2E3F4A5B6C7D8E9F0A1B2C3D4E5F6A7B8C9D0E1F2A3B");
    BigInt b("1A2B3C4D5E6F7A8B9C0D1E2F3A4B5C6D7E8F9A0B1C2D3E4F5A6B7C8D9E0F1A2B");
    BigInt n("E3F4A5B6C7D8E9F0A1B2C3D4E5F6A7B8C9D0E1F2A3B4C5D6E7F8A9B0C1D2E3F3");
    BigInt exp("10001");
    BigInt mu = n.getBarrettMu();
    BigInt p("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    int itersFast = 10000;
    int itersMed = 1000;
    int itersSlow = 50;

    ofstream csvFile("benchmark_results.csv");
    csvFile << "Operation,Time_ns\n";

    cout << "\n--- Benchmarks (Average Time in ns) ---\n";
    cout << left << setw(25) << "Operation" << "Time (ns)\n";
    cout << "---------------------------------------\n";

    auto start = steady_clock::now();
    for (int i = 0; i < itersMed; ++i)
    {
        a.gcd(b);
    }
    auto end = steady_clock::now();
    long long t_gcd = duration_cast<nanoseconds>(end - start).count() / itersMed;
    cout << left << setw(25) << "gcd" << t_gcd << "\n";
    csvFile << "gcd," << t_gcd << "\n";

    start = steady_clock::now();
    for (int i = 0; i < itersMed; ++i)
    {
        a.lcm(b);
    }
    end = steady_clock::now();
    long long t_lcm = duration_cast<nanoseconds>(end - start).count() / itersMed;
    cout << left << setw(25) << "lcm" << t_lcm << "\n";
    csvFile << "lcm," << t_lcm << "\n";

    start = steady_clock::now();
    for (int i = 0; i < itersFast; ++i)
    {
        a.addMod(b, n);
    }
    end = steady_clock::now();
    long long t_add = duration_cast<nanoseconds>(end - start).count() / itersFast;
    cout << left << setw(25) << "addMod" << t_add << "\n";
    csvFile << "addMod," << t_add << "\n";

    start = steady_clock::now();
    for (int i = 0; i < itersFast; ++i)
    {
        a.subMod(b, n);
    }
    end = steady_clock::now();
    long long t_sub = duration_cast<nanoseconds>(end - start).count() / itersFast;
    cout << left << setw(25) << "subMod" << t_sub << "\n";
    csvFile << "subMod," << t_sub << "\n";

    start = steady_clock::now();
    for (int i = 0; i < itersFast; ++i)
    {
        a.mulMod(b, n, mu);
    }
    end = steady_clock::now();
    long long t_mul = duration_cast<nanoseconds>(end - start).count() / itersFast;
    cout << left << setw(25) << "mulMod" << t_mul << "\n";
    csvFile << "mulMod," << t_mul << "\n";

    start = steady_clock::now();
    for (int i = 0; i < itersFast; ++i)
    {
        a.squareMod(n, mu);
    }
    end = steady_clock::now();
    long long t_sqr = duration_cast<nanoseconds>(end - start).count() / itersFast;
    cout << left << setw(25) << "squareMod" << t_sqr << "\n";
    csvFile << "squareMod," << t_sqr << "\n";

    start = steady_clock::now();
    for (int i = 0; i < itersMed; ++i)
    {
        a.powerMod(exp, n, mu);
    }
    end = steady_clock::now();
    long long t_pow = duration_cast<nanoseconds>(end - start).count() / itersMed;
    cout << left << setw(25) << "powerMod" << t_pow << "\n";
    csvFile << "powerMod," << t_pow << "\n";

    start = steady_clock::now();
    for (int i = 0; i < itersSlow; ++i)
    {
        p.isProbablePrime(20);
    }
    end = steady_clock::now();
    long long t_mr = duration_cast<nanoseconds>(end - start).count() / itersSlow;
    cout << left << setw(25) << "Miller-Rabin (k=20)" << t_mr << "\n";
    csvFile << "MillerRabin," << t_mr << "\n";

    cout << "---------------------------------------\n";
    csvFile.close();
}