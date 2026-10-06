#include "benchmarks.h"
#include "BigInt.h"
#include <iostream>
#include <chrono>

using namespace std;

void runBenchmarks()
{
    cout << "--- Running Benchmarks ---\n";
    
    BigInt a("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    BigInt b("11111111111111111111111111111111");
    BigInt exp("10"); 
    
    const int ITERATIONS = 10000;
    volatile uint32_t dummy = 0; 
    
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        BigInt res = a + b;
        dummy = res.words[0];
    }
    auto end = std::chrono::steady_clock::now();
    long long timeAdd = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / ITERATIONS;
    
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        BigInt res = a - b;
        dummy = res.words[0];
    }
    end = std::chrono::steady_clock::now();
    long long timeSub = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / ITERATIONS;
    
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        BigInt res = a * b;
        dummy = res.words[0];
    }
    end = std::chrono::steady_clock::now();
    long long timeMul = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / ITERATIONS;
    
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        BigInt res = a / b;
        dummy = res.words[0];
    }
    end = std::chrono::steady_clock::now();
    long long timeDiv = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / ITERATIONS;

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        BigInt res = a % b;
        dummy = res.words[0];
    }
    end = std::chrono::steady_clock::now();
    long long timeMod = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / ITERATIONS;

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
    {
        BigInt res = a.power(exp);
        dummy = res.words[0];
    }
    end = std::chrono::steady_clock::now();
    long long timePow = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / ITERATIONS;
    
    cout << "Operation | Average Time (ns)\n";
    cout << "---------------------------\n";
    cout << "Addition  | " << timeAdd << "\n";
    cout << "Subtract  | " << timeSub << "\n";
    cout << "Multiply  | " << timeMul << "\n";
    cout << "Division  | " << timeDiv << "\n";
    cout << "Modulo    | " << timeMod << "\n";
    cout << "Power     | " << timePow << "\n";
    cout << "---------------------------\n\n";
}