#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

#define MIN_VAL -10000
#define MAX_VAL 10000

#define ERR_RANGE 100
#define ERR_RES 101

/**
 * @brief Tries to read two numbers in the certain range
 * 
 * @param a A pointer to first number
 * @param b A pointer to second number
 * 
 * @return Boolean value indicating if read is successful
 */
bool read_nums(int *a, int *b);

/**
 * @brief  Checks if the number fits into the interval
 * 
 * This function takes an integer as input and returns true if 
 * the it fits into predefined inteval, false otherwise
 * 
 * @param num An integer to be checked
 * 
 * @return Boolean value indicating if the condition is true
 */
bool check_interval(const int num);

/**
 * @brief Performs operations on provided numbers and prints results
 * 
 * @param a First number
 * @param b Second number
 */
void print_info(const int a, const int b);

/* The main program */
int main(int argc, char *argv[]) {
	int ret_val = EXIT_SUCCESS;

	int a, b;

	if (!read_nums(&a, &b)) {
		fprintf(stderr, "Error: Vstup je mimo interval!\n");
		return ERR_RANGE;
	}

	if (b == 0) {
		ret_val = ERR_RES;
		fprintf(stderr, "Error: Nedefinovany vysledek!\n");
	}

	print_info(a, b);
	return ret_val;
}

bool read_nums(int *a, int *b) {
	scanf("%d %d", a, b);

	if (!(check_interval(*a) && check_interval(*b))) {
		return false;
	}

	return true;
}

bool check_interval(const int num) {
	if (num < MIN_VAL || num > MAX_VAL) {
		return false;
	}

	return true;
}

void print_info(const int a, const int b) {
	printf("Desitkova soustava: %d %d\n", a, b);
	printf("Sestnactkova soustava: %x %x\n", a, b);
	printf("Soucet: %d + %d = %d\n", a, b, a + b);
	printf("Rozdil: %d - %d = %d\n", a, b, a - b);
	printf("Soucin: %d * %d = %d\n", a, b, a * b);

	if (b == 0) {
		printf("Podil: %d / %d = NaN\n", a, b);

	} else {
		printf("Podil: %d / %d = %d\n", a, b, a / b);
	}

	printf("Prumer: %.1f\n", ((float) a + b) / 2);
}
