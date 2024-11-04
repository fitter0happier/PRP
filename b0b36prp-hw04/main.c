#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum error_codes {
    ERROR_INPUT = 100
};

#define PRIME_BOUND 1000000

/**
 * @brief Returns an array of prime numbers indices in range [2, prime_bound) using
 * the "Sieve of Eratosthenes" algorithm
 * 
 * @param primes_indices array of booleans to indicate if current index is prime
 * @param prime_bound upper bound of primes up to which calculate them
 */
static void sieve(bool *primes_indices, const int prime_bound);

/**
 * @brief Prints prime factorization of the provided integer number
 * 
 * @param number a 64-bit unsigned integer greater than 1
 * @param primes_indices array of prime numbers indices
 */
void factorize(unsigned long number, const bool primes_indices[]);

int main(int argc, char *argv[]) {
    int ret_code = EXIT_SUCCESS;
    long number_to_factor;

    // Get all indices where prime numbers are located
    bool primes_indices[PRIME_BOUND];
    sieve(primes_indices, PRIME_BOUND);

    while (true) {
        if (scanf("%ld", &number_to_factor) != 1 || number_to_factor < 0) {
            ret_code = ERROR_INPUT;
            fprintf(stderr, "Error: Chybny vstup!\n");
            break;

        } else if (number_to_factor == 0) {
            break;

        } else {
            factorize(number_to_factor, primes_indices);
        }
    }

    return ret_code;
}


static void sieve(bool *primes_indices, const int prime_bound) {
    primes_indices[0] = false; // 0 and 1 are both not primes
    primes_indices[1] = false;

    for (int i = 2; i < prime_bound; ++i) {
        primes_indices[i] = true;
    }

    for (int i = 2; i < sqrt(prime_bound); ++i) {
        if (primes_indices[i]) {
            for (int j = i * i; j < prime_bound; j += i) {
                primes_indices[j] = false;
            }
        }
    }
}

void factorize(unsigned long number, const bool primes_indices[]) {
    printf("Prvociselny rozklad cisla %ld je:\n", number);

    if (number == 1) {
        printf("1\n");
        return;
    }

    int current_index = 0;
    int current_power = 0;

    while (number != 1) {
        if (primes_indices[current_index]) {
            int factor = current_index;

            while (number % factor == 0) {
                ++current_power;
                number /= factor;

            } 

            switch (current_power) {
            case 0:
                break;

            case 1:
                printf("%d%s", factor, (number == 1 ? "\n" : " x "));
                break;
            
            default:
                printf("%d^%d%s", factor, current_power, (number == 1 ? "\n" : " x "));
                break;
            }
        }

        current_power = 0;
        ++current_index;
    }
}
