#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <omp.h>

using namespace std;

bool isPrime(unsigned long long num)
{
    if (num <= 1)
        return false;
    if (num == 2)
        return true;
    if (num % 2 == 0)
        return false;
    for (unsigned long long i = 3; i * i <= num; i += 2)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

// Function to find perfect numbers
vector<unsigned long long> findPerfectNumbers(int n)
{
    vector<unsigned long long> perfectNumbers;
    perfectNumbers.reserve(n); 
    int count = 0;

#pragma omp parallel
    {
        vector<unsigned long long> local_perfectNumbers;
        local_perfectNumbers.reserve(n); 

#pragma omp for schedule(dynamic)
        for (int p = 2; p <= 62; ++p)
        {
            if (count >= n) 
                continue;

            unsigned long long mersenne = (1ULL << p) - 1; // Calculate Mersenne number
            if (isPrime(mersenne))
            {
                unsigned long long perfectNumber = (1ULL << (p - 1)) * mersenne; // Calculate perfect number
                local_perfectNumbers.push_back(perfectNumber);

#pragma omp atomic
                ++count; // Safely increment count across threads
            }
        }

#pragma omp critical
        {
            perfectNumbers.insert(perfectNumbers.end(), local_perfectNumbers.begin(), local_perfectNumbers.end());
        }
    }

    sort(perfectNumbers.begin(), perfectNumbers.end());

    if (perfectNumbers.size() > n)
    {
        perfectNumbers.resize(n);
    }

    return perfectNumbers;
}

int main()
{
    int n = 8;                                                         // Number of perfect numbers to find
    double start_time = omp_get_wtime();                               // Start timer
    vector<unsigned long long> perfectNumbers = findPerfectNumbers(n); // Find perfect numbers
    double end_time = omp_get_wtime();                                 // End timer

    cout << "Parallel Execution time: " << (end_time - start_time) << " seconds" << endl;
    cout << "The first " << n << " perfect numbers are:\n";
    for (unsigned long long num : perfectNumbers)
    {
        cout << num << endl; // Output perfect numbers
    }

    return 0;
}
