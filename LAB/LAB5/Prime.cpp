#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;

bool isPrime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) return false;
    }
    return true;
}

int main() {
    const int limit = 200; 
    int count = 0; 
    int sum = 0; 

    
    #pragma omp parallel
    {
        int localCount = 0; 
        int localSum = 0; 

        // Check odd numbers less than the limit
        #pragma omp for
        for (int i = 3; i < limit; i += 2) {
            if (isPrime(i)) {
                // Check the previous odd number
                if (isPrime(i - 2)) {
                    localCount++; // Found a pair of consecutive odd primes
                    localSum += (i + (i - 2)); // Sum of the consecutive odd primes
                }
            }
        }

        // Use critical section to update the global count and sum
        #pragma omp critical
        {
            count += localCount;
            sum += localSum;
        }
    }

    // Output the results
    cout << "Number of times two consecutive odd integers are both prime: " << count << endl;
    cout << "Sum of each two consecutive odd integers that are both prime: " << sum << endl;

    return 0;
}
