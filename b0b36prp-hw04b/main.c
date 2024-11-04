#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum error_codes {
    ERROR_INPUT = 100
};

#define MAX_LEN 100
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
 * @brief Prints prime factorization of the provided big number
 * 
 * @param number a string containing number to factorize
 * @param primes_indices array of prime numbers indices
 */
void factorize(char *number, const bool primes_indices[]);

int main(int argc, char *argv[]) {
    int ret_code = EXIT_SUCCESS;
    char number_as_str[MAX_LEN + 1];

    // Get all indices where prime numbers are located
    bool primes_indices[PRIME_BOUND];
    sieve(primes_indices, PRIME_BOUND);

    while (true) {
        char c = 0;
        int num_index = 0;

        while ((c = getchar()) != '\n' && c != EOF) {
            if (c < '0' || c > '9') {
                ret_code = ERROR_INPUT;
                break;
            }

            number_as_str[num_index++] = c;
        }

        if (ret_code == ERROR_INPUT) {
            fprintf(stderr, "Error: Chybny vstup!\n");
            break;

        } else if (number_as_str[0] == '0') {
            break;

        } else {
            number_as_str[num_index] = '\0';
            factorize(number_as_str, primes_indices);
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

void factorize(char *number, const bool primes_indices[]) {
    printf("Prvociselny rozklad cisla %s je:\n", number);

    if (strlen(number) == 1 && number[0] == '1') {
        printf("1\n");
        return;
    }

    int current_index = 0;
    int current_power = 0;

    while (!(strlen(number) == 1 && number[0] == '1')) {
        if (primes_indices[current_index]) {
            int factor = current_index;

            while (true) {
                int num_len = strlen(number);
                int div_index = 0;
                int q_index = 0;

                int dividend = 0;
                char quotient[MAX_LEN - 1];

                while (div_index <= (num_len - 1) && dividend < factor) {
                    dividend = (dividend * 10) + (number[div_index++] - '0');
                }

                while (div_index <= (num_len - 1)) {
                    quotient[q_index++] = (dividend / factor) + '0';
                    dividend = (dividend % factor) * 10 + (number[div_index++] - '0');
                }

                quotient[q_index++] = (dividend / factor) + '0';
                dividend = (dividend % factor);

                if (dividend != 0) {
                    break;

                } else {
                    ++current_power;

                    for (int i = 0; i < q_index; ++i) {
                        number[i] = quotient[i];
                    }

                    number[q_index] = '\0';
                }
            } 

            switch (current_power) {
            case 0:
                break;

            case 1:
                printf("%d%s", factor, (strlen(number) == 1 && number[0] == '1' ? "\n" : " x "));
                break;
            
            default:
                printf("%d^%d%s", factor, current_power, (strlen(number) == 1 && number[0] == '1' ? "\n" : " x "));
                break;
            }
        }

        current_power = 0;
        ++current_index;
    }
}
