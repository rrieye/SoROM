#include "benchmarks.h"
#include "FieldElement251.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <random>
#include <x86intrin.h>

using namespace std;

static uint64_t getRdtsc()
{
    return __rdtsc();
}

static FieldElement251 generateRandomElement(mt19937_64& rng)
{
    array<uint64_t, 4> w;
    for (size_t i = 0; i < 4; ++i)
    {
        w[i] = rng();
    }
    FieldElement251 elem(w);
    elem.maskUnusedBits();
    return elem;
}

void runFieldBenchmarks()
{
    mt19937_64 rng(1337);

    cout << "--- BENCHMARKS ---\n";

    const int iterations = 10000;
    vector<FieldElement251> va(iterations);
    vector<FieldElement251> vb(iterations);
    for (int i = 0; i < iterations; ++i)
    {
        va[i] = generateRandomElement(rng);
        vb[i] = generateRandomElement(rng);
    }

    uint64_t start_cycles = getRdtsc();
    auto start_time = chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        FieldElement251 res = va[i] + vb[i];
    }
    auto end_time = chrono::steady_clock::now();
    uint64_t end_cycles = getRdtsc();
    double time_add = chrono::duration<double, nano>(end_time - start_time).count() / iterations;
    uint64_t cycles_add = (end_cycles - start_cycles) / iterations;

    start_cycles = getRdtsc();
    start_time = chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        FieldElement251 res = va[i] * vb[i];
    }
    end_time = chrono::steady_clock::now();
    end_cycles = getRdtsc();
    double time_mul = chrono::duration<double, nano>(end_time - start_time).count() / iterations;
    uint64_t cycles_mul = (end_cycles - start_cycles) / iterations;

    start_cycles = getRdtsc();
    start_time = chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        FieldElement251 res = va[i].square();
    }
    end_time = chrono::steady_clock::now();
    end_cycles = getRdtsc();
    double time_sq = chrono::duration<double, nano>(end_time - start_time).count() / iterations;
    uint64_t cycles_sq = (end_cycles - start_cycles) / iterations;

    const int small_iters = 500;

    start_cycles = getRdtsc();
    start_time = chrono::steady_clock::now();
    for (int i = 0; i < small_iters; ++i)
    {
        FieldElement251 res = va[i].invert();
    }
    end_time = chrono::steady_clock::now();
    end_cycles = getRdtsc();
    double time_inv = chrono::duration<double, nano>(end_time - start_time).count() / small_iters;
    uint64_t cycles_inv = (end_cycles - start_cycles) / small_iters;

    start_cycles = getRdtsc();
    start_time = chrono::steady_clock::now();
    for (int i = 0; i < small_iters; ++i)
    {
        FieldElement251 res = va[i].power(vb[i]);
    }
    end_time = chrono::steady_clock::now();
    end_cycles = getRdtsc();
    double time_pow = chrono::duration<double, nano>(end_time - start_time).count() / small_iters;
    uint64_t cycles_pow = (end_cycles - start_cycles) / small_iters;

    start_cycles = getRdtsc();
    start_time = chrono::steady_clock::now();
    for (int i = 0; i < small_iters; ++i)
    {
        uint8_t res = va[i].trace();
    }
    end_time = chrono::steady_clock::now();
    end_cycles = getRdtsc();
    double time_tr = chrono::duration<double, nano>(end_time - start_time).count() / small_iters;
    uint64_t cycles_tr = (end_cycles - start_cycles) / small_iters;

    cout << left << setw(20) << "Operation" << setw(18) << "Time (ns)" << setw(18) << "CPU Cycles" << "\n";
    cout << "--------------------------------------------------------\n";
    cout << left << setw(20) << "Addition" << setw(18) << time_add << setw(18) << cycles_add << "\n";
    cout << left << setw(20) << "Multiplication" << setw(18) << time_mul << setw(18) << cycles_mul << "\n";
    cout << left << setw(20) << "Squaring" << setw(18) << time_sq << setw(18) << cycles_sq << "\n";
    cout << left << setw(20) << "Trace" << setw(18) << time_tr << setw(18) << cycles_tr << "\n";
    cout << left << setw(20) << "Inversion" << setw(18) << time_inv << setw(18) << cycles_inv << "\n";
    cout << left << setw(20) << "Power" << setw(18) << time_pow << setw(18) << cycles_pow << "\n\n";
}