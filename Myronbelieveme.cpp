#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() 
{
    int m = 4;
    int mod = (1 << m) - 1;

    volatile int dummy = 0; 
    const int it = 10000000;

    cout << "Formula equivalence proof:\n";
    for (int i = 0; i < m; i++) 
    {
        for (int j = 0; j < m; j++ ) 
        {
            int exp_lecture = ((1 << i) + (1 << j)) % mod;

            int diff = (j - i + m) % m; 
            int exp_base = ((1 << 0) + (1 << diff)) % mod;
            int exp_shifted = (exp_base * (1 << i)) % mod;

            cout << "i=" << i << ", j=" << j 
                 << " | lec: beta^" << left << setw(2) << exp_lecture 
                 << " | shifted: beta^" << left << setw(2) << exp_shifted 
                 << (exp_lecture == exp_shifted ? "  [Y]" : "  [N]") << "\n";
        }
    }

    cout << "\n--- Starting test (" << it << " iterations) ---\n";

    auto start_lec = steady_clock::now();
    for (int k = 0; k < it; k++) 
    {
        for (int i = 0; i < m; i++) 
        {
            for (int j = 0; j < m; j++) 
            {
                dummy = ((1 << i) + (1 << j)) % mod;
            }
        }
    }
    auto end_lec = steady_clock::now();
    auto duration_lec = duration_cast<milliseconds>(end_lec - start_lec).count();

    auto start_shift = steady_clock::now();
    for (int k = 0; k < it; k++) 
    {
        for (int i = 0; i < m; i++) 
        {
            for (int j = 0; j < m; j++) 
            {
                int diff = (j - i + m) % m; 
                int exp_base = ((1 << 0) + (1 << diff)) % mod;
                dummy = (exp_base * (1 << i)) % mod;
            }
        }
    }
    auto end_shift = steady_clock::now();
    auto duration_shift = duration_cast<milliseconds>(end_shift - start_shift).count();

    auto start_bitwise = steady_clock::now();
    int a = 7;
    int b = 10;
    for (int k = 0; k < it; k++) 
    {
        for (int i = 0; i < m; i++) 
        {
            int a_rot = ((a << i) | (a >> (m - i))) & mod;
            int b_rot = ((b << i) | (b >> (m - i))) & mod;
            dummy = a_rot ^ b_rot; 
        }
    }
    auto end_bitwise = steady_clock::now();
    auto duration_bitwise = duration_cast<milliseconds>(end_bitwise - start_bitwise).count();

    cout << "1. Lecture index generation time: " << duration_lec << " ms\n";
    cout << "2. Shifted index generation time: " << duration_shift << " ms\n";
    cout << "3. Actual Bitwise multiplication time (Shift + XOR): " << duration_bitwise << " ms\n";

    return 0;
}