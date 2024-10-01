#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>

struct ThreadArgs {
    std::vector<bool>* prime;
    int p;
    int n;
};

// Thread function to mark multiples of a prime number in a given range
void* markMultiples(void* args) {
    ThreadArgs* threadArgs = static_cast<ThreadArgs*>(args);
    std::vector<bool>& prime = *(threadArgs->prime);
    int p = threadArgs->p;
    int n = threadArgs->n;

    // Mark multiples of p as false starting from p^2
    for (int i = p * p; i <= n; i += p) {
        prime[i] = false;
    }

    return nullptr;
}

void sieveOfEratosthenes(int n, int numThreads) {
    // Create a boolean array "prime[0..n]" and initialize all entries as true.
    std::vector<bool> prime(n + 1, true);

    // 0 and 1 are not primes
    prime[0] = prime[1] = false;

    int sqrt_n = static_cast<int>(sqrt(n));

    // Create an array of pthreads
    std::vector<pthread_t> threads(numThreads);
    std::vector<ThreadArgs> threadArgs(numThreads);

    // Loop through numbers up to sqrt(n)
    for (int p = 2; p <= sqrt_n; p++) {
        if (prime[p]) {
            // Parallelize the marking of multiples for the current prime `p`
            for (int t = 0; t < numThreads; t++) {
                threadArgs[t].prime = &prime;
                threadArgs[t].p = p;
                threadArgs[t].n = n;

                // Create a thread to mark multiples of p
                pthread_create(&threads[t], nullptr, markMultiples, &threadArgs[t]);
            }

            // Join the threads
            for (int t = 0; t < numThreads; t++) {
                pthread_join(threads[t], nullptr);
            }
        }
    }

    // Print all prime numbers
    std::cout << "Prime numbers up to " << n << " are: ";
    for (int i = 2; i <= n; i++) {
        if (prime[i]) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}

int main() {
    int n, numThreads;

    std::cout << "Enter the limit: ";
    std::cin >> n;

    std::cout << "Enter the number of threads: ";
    std::cin >> numThreads;

    sieveOfEratosthenes(n, numThreads);

    return 0;
}
