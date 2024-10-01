#include <iostream>
#include <vector>
#include <cmath>
#include <openacc.h>

void sieveOfEratosthenes(int n) {
    // Step 1: Create a boolean array "prime[0..n]" and initialize all entries as true.
    std::vector<bool> prime(n + 1, true);

    // 0 and 1 are not prime numbers
    prime[0] = prime[1] = false;

    int sqrt_n = static_cast<int>(sqrt(n));

    // Step 2: Use OpenACC to parallelize the outer loop over primes
    #pragma acc data copy(prime[0:n+1])
    {
        for (int p = 2; p <= sqrt_n; p++) {
            if (prime[p]) {
                // Step 3: Parallelize the inner loop to mark multiples of p as false
                #pragma acc parallel loop
                for (int i = p * p; i <= n; i += p) {
                    prime[i] = false;
                }
            }
        }
    }

    // Step 4: Print all prime numbers
    std::cout << "Prime numbers up to " << n << " are: ";
    for (int i = 2; i <= n; i++) {
        if (prime[i]) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}

int main() {
    int n;
    std::cout << "Enter the limit: ";
    std::cin >> n;

    sieveOfEratosthenes(n);

    return 0;
}
