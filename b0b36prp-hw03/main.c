#include <stdio.h>
#include <stdlib.h>
 
enum error_codes {
    ERROR_INPUT = 100, 
    ERROR_RANGE = 101, 
    ERROR_NOT_ODD = 102, 
    ERROR_FENCE_SIZE = 103
};
 
enum const_values {
    MIN_VALUE = 3,
    MAX_VALUE = 69
};
 
/**
 * @brief Draws the house with given width and height 
 * 
 * @param width - width, must be an odd number in range [3,69]
 * @param height - height, must be a number in range [3,69]
 * @return int ret - return value
 */
int print_house(int width, int height);
 
/**
 * @brief Draws the house of given width/height, plus the fence of given
 * width/height for the optional task
 * 
 * @param width - width, must be an odd number in range [3,69] 
 * @param height - height, must be a number in range [3,69]
 * @param fence - fence width/height, must be a positive number strictly less than height
 * @return int ret - return value
 */
int print_house_opt(int width, int height, int fence);
 
/**
 * @brief Draws the roof of the house
 * 
 * @param width - width of the house
 */
void print_roof(int width);
 
/**
 * @brief - Draws the fence of given width/height
 * 
 * @param fence - fence width/height
 * @param skip - check whether or not to print an empty symbol before the fence
 */
 
void print_fence(int fence, int skip);
 
/**
 * @brief Tests if the width and height are within range/odd
 * 
 * @param width - width, must be an odd number in range [3,69]
 * @param height - height, must be a number in range [3,69]
 * @return int ret - return value 
 */
 
int test_input(int width, int height);

/**
 * @brief Tests if the width, height and fence are set according to the task
 * 
 * @param width - width, must be an odd number in range [3,69]
 * @param height - height, must be a number in range [3,69]
 * @param fence - height, must be a positive number less than height
 * @return int ret - return value 
 */
 
int test_input_opt(int width, int height, int fence);
 
/**
 * @brief Prints error to the standard error output depending on the code
 * 
 * @param code - error code
 */
void handle_error(int code);
 
int main(int argc, char *argv[]) {
    int ret_code = ERROR_INPUT;
    int width, height, fence;

    if (scanf("%d %d", &width, &height) == 2) {
        if (width == height) {
            if (scanf("%d", &fence) == 1) {
                ret_code = print_house_opt(width, height, fence);
            }

        } else {
            ret_code = print_house(width, height);
        }
    }

    handle_error(ret_code);
    return ret_code;
}

int print_house(int width, int height) {
	int ret;
	if ((ret = test_input(width, height)) != EXIT_SUCCESS) {
		return ret;
	}

	print_roof(width);

    for (int i = 0; i < width; ++i) {
        printf("X");
    }
    printf("\n");

    for (int i = 0; i < height - 2; ++i) {
        printf("X");

        for (int j = 0; j < width - 2; ++j) {
            printf(" ");
        }

        printf("X\n");
    }

    for (int i = 0; i < width; ++i) {
        printf("X");
    }
    printf("\n");

	return EXIT_SUCCESS;
}

int print_house_opt(int width, int height, int fence) {
	int ret;
	if ((ret = test_input_opt(width, height, fence)) != EXIT_SUCCESS) {
		return ret;
	}

	print_roof(width);

    for (int i = 0; i < width; ++i) {
        printf("X");
    }
    printf("\n");

    char curr = 'o';
    for (int i = 0; i < height - 2; ++i) {
        printf("X");

        for (int j = 0; j < width - 2; ++j) {
            putchar(curr);

            if (curr == 'o') {
                curr = '*';

            } else {
                curr = 'o';
            }
        }

        printf("X");

        if (i >= height - fence - 1) {
            int skip = 1;
            if ((i == height - fence - 1) || (i == height - 2)) {
                skip = 0;
            }

            print_fence(fence, skip);
        }

        printf("\n");

    }

    for (int i = 0; i < width; ++i) {
        printf("X");
    }

    print_fence(fence, 0);
    printf("\n");

	return EXIT_SUCCESS;
}

void print_roof(int width) {
	int counter = 1;
	for (int i = width / 2; i > 0; --i) {
		for (int j = i; j > 0; --j) {
			printf(" ");
		}
		printf("X");

		if (i != width / 2) {
			for (int j = 0; j < counter; ++j) {
				printf(" ");
			}
			printf("X");
			counter += 2;
		}

		printf("\n");
	}
}

void print_fence(int fence, int skip) {
    if (fence % 2 == 0) {
        if (skip) {
            for (int i = 0; i < fence; ++i) {
                i % 2 == 0 ? printf(" "): printf("|");
            }

        } else {
            for (int i = 0; i < fence; ++i) {
                i % 2 == 0 ? printf("-"): printf("|");
            }
        }

    } else {
        if (skip) {
            for (int i = 0; i < fence; ++i) {
                i % 2 == 0 ? printf("|"): printf(" ");
            }

        } else {
            for (int i = 0; i < fence; ++i) {
                i % 2 == 0 ? printf("|"): printf("-");
            }
        }
    }
}

int test_input(int width, int height) {
    if (width < MIN_VALUE || width > MAX_VALUE || height < MIN_VALUE || height > MAX_VALUE) {
        return ERROR_RANGE;

    } else if (width % 2 == 0 ) {
        return ERROR_NOT_ODD;
    }

    return EXIT_SUCCESS;
}

int test_input_opt(int width, int height, int fence) {
	int ret;
    if ((ret = test_input(width, height)) != EXIT_SUCCESS) {
        return ret;

    } else if (fence <= 0 || fence >= height) {
        return ERROR_FENCE_SIZE;
    }

    return EXIT_SUCCESS;
}

void handle_error(int code) {
    switch (code) {
        case ERROR_INPUT:
            fprintf(stderr, "Error: Chybny vstup!\n");
            break;
        case ERROR_RANGE:
            fprintf(stderr, "Error: Vstup mimo interval!\n");
            break;
        case ERROR_NOT_ODD:
            fprintf(stderr, "Error: Sirka neni liche cislo!\n");
            break;
        case ERROR_FENCE_SIZE:
            fprintf(stderr, "Error: Neplatna velikost plotu!\n");
    }
}
