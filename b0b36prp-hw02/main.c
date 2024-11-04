#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN_VAL -10000
#define MAX_VAL 10000

#define ERR_RANGE 100

/**
 * @brief  Checks if the number fits into the interval.
 * 
 * This function takes an integer as input and returns true if 
 * the it fits into predefined inteval, false otherwise.
 * 
 * @param num An integer to be checked.
 * 
 * @return Boolean value indicating if the condition is true.
 */
bool check_interval(int num);

/**
 * @brief  Converts the floating point number to percentage.
 * 
 * This function takes a float as input and returns its percentage representation by multiplying by 100
 * 
 * @param num A float to be converted.
 * 
 * @return Float representing percentage.
 */
float convert_to_percent(float num);

/* The main program */
int main(int argc, char *argv[]) {
	unsigned int neg_count = 0, pos_count = 0;
	unsigned int odd_count = 0, even_count = 0;
	unsigned int total_count = 0;

	int max = MIN_VAL;
	int min = MAX_VAL;
	int sum = 0;

	int curr_num;
	while (scanf("%d", &curr_num) != EOF) {
		if (!check_interval(curr_num)) {
			printf("\nError: Vstup je mimo interval!\n");
			return ERR_RANGE;
		}

		if (total_count == 0) {
			printf("%d", curr_num);

		} else {
			printf(", %d", curr_num);
		}

		if (curr_num < 0) {
			++neg_count;
		} 

		if (curr_num > 0) {
			++pos_count;
		}

		(curr_num % 2 != 0) ? ++odd_count : ++even_count;

		++total_count;

		if (curr_num < min) {
			min = curr_num;
		} 
		
		if (curr_num > max) {
			max = curr_num;
		}

		sum += curr_num;
	}

	printf("\n");
	printf("Pocet cisel: %d\n", total_count);
	printf("Pocet kladnych: %d\n", pos_count);
	printf("Pocet zapornych: %d\n", neg_count);
	printf("Procento kladnych: %.2f\n", convert_to_percent((float) pos_count / total_count));
	printf("Procento zapornych: %.2f\n", convert_to_percent((float) neg_count / total_count));
	printf("Pocet sudych: %d\n", even_count);
	printf("Pocet lichych: %d\n", odd_count);
	printf("Procento sudych: %.2f\n", convert_to_percent((float) even_count / total_count));
	printf("Procento lichych: %.2f\n", convert_to_percent((float) odd_count / total_count));
	printf("Prumer: %.2f\n", (float) sum / total_count);
	printf("Maximum: %d\n", max);
	printf("Minimum: %d\n", min);

	return EXIT_SUCCESS;
}

bool check_interval(int num) {
	if (num < MIN_VAL || num > MAX_VAL) {
		return false;
	}

	return true;
}

float convert_to_percent(float num) {
	return num * 100;
}
