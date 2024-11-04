#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum error_codes {
    ERROR_INPUT = 100, 
    ERROR_RANGE = 101
};

enum const_values {
    STR_COUNT = 2,
    ROTATIONS_COUNT = 52,
    INIT_SIZE = 100
};

/**
 * @brief Reads a string from standard input and allocates memory for it
 * 
 * @return char* - pointer to the read string, or NULL if allocation fails
 */
char* read_string();

/**
 * @brief Enlarges the string by reallocating it with double the current size
 * 
 * @param str - the original string
 * @param curr_size - the current size of the string
 * @return char* - the reallocated string, or NULL if reallocation fails
 */
char* enlarge_string(char *str, int *curr_size);

/**
 * @brief Decyphers a cyphered string by comparing it with an overheard string
 * 
 * @param cyphered_str - the cyphered string
 * @param overheard_str - the overheard string to compare against
 * @param compare - the comparison function to use (Hamming or Levenshtein)
 */
void decypher(char *cyphered_str, const char *overheard_str, int (*compare)(const char *first_str, const char *second_str));

/**
 * @brief Compares two strings using the Hamming distance metric
 * 
 * @param first_str - the first string
 * @param second_str - the second string
 * @return int - the number of positions at which the corresponding characters are similar
 */
int compare_hamming(const char *first_str, const char *second_str);

/**
 * @brief Compares two strings using the Levenshtein distance metric
 * 
 * @param first_str - the first string
 * @param second_str - the second string
 * @return int - the Levenshtein distance between the two strings
 */
int compare_levenstein(const char *first_str, const char *second_str);

/**
 * @brief Shifts each character in the string by the given offset
 * 
 * @param str - the string to be shifted
 * @param offset - the number of rotations to shift each character
 * @param len - the length of the string
 */
void shift(char *str, int offset, int len);

/**
 * @brief Rotates a character to the next character in the alphabet. Wraps 'z' to 'A' and 'Z' to 'a'
 * 
 * @param original - the original character
 * @return char - the rotated character
 */
char rotate(char original);

/**
 * @brief Frees memory allocated for two strings
 * 
 * @param first_str - pointer to first string
 * @param second_str - pointer to second string
 */
void handle_free(char *first_str, char *second_str);

/**
 * @brief Prints error to the standard error output depending on the code
 * 
 * @param code - error code
 */
void handle_error(int code);

int main(int argc, char *argv[]) {
    int ret_code = EXIT_SUCCESS;
    char *strings[STR_COUNT] = { NULL, NULL };

    for (int i = 0; i < STR_COUNT; ++i) {
        char *str = read_string();;
        strings[i] = str;
    }

    if (strings[0] == NULL || strings[1] == NULL) {
        ret_code = ERROR_INPUT;

    } else if (strlen(strings[0]) != strlen(strings[1]) && argc != 2) {
        ret_code = ERROR_RANGE;

    } else {
        int (*compare_func)(const char *first_str, const char *second_str);
        compare_func = argc == 2 ? compare_levenstein : compare_hamming;
        decypher(strings[0], strings[1], compare_func);
        printf("%s\n", strings[0]);
    }

    handle_free(strings[0], strings[1]);
    handle_error(ret_code);
    return ret_code;
}

char* enlarge_string(char *str, int *curr_size) {
    char *temp = (char *) realloc(str, (size_t) (*curr_size * 2 + 1));

    if (temp == NULL) {
        free(str);
        str = NULL;
        
    } else {
        str = temp;
        *curr_size *= 2;
    }

    return str;
}

char* read_string() {
    char c;
    char *str = (char *) malloc(INIT_SIZE + 1);

    if (str == NULL) { // allocation failed
        return NULL;
    }

    int str_size = INIT_SIZE;
    int char_count = 0;

    while ((c = getchar()) != '\n' && c != EOF) {
        if (c < 'A' || (c > 'Z' && c < 'a') || c > 'z') {
            free(str);
            str = NULL;
            break;
        }

        str[char_count++] = c;

        if (char_count == str_size) { // the string is full
            str = enlarge_string(str, &str_size);

            if (str == NULL) { // reallocation failed
                break;
            }
        }
    }

    if (str != NULL) {
        str[char_count] = '\0';
    }

    return str;
}

char rotate(char original) {
    char rotated;

    switch (original) {
        case 'z':
            original = 'A';
            break;
        case 'Z':
            original = 'a';
            break;
        default:
            ++original;
            break;
    } 

    rotated = original;
    return rotated;
}

int compare_hamming(const char *first_str, const char *second_str) {
    unsigned int len = strlen(first_str);
    int overlap = 0;

    for (int i = 0; i < len; ++i) {
        if (first_str[i] == second_str[i]) {
            ++overlap;
        }
    }

    return overlap;
}

int compare_levenstein(const char *first_str, const char *second_str) {
    unsigned int l1 = strlen(first_str) + 1;
    unsigned int l2 = strlen(second_str) + 1;

    int (*matrix)[l2] = malloc(sizeof(int[l1][l2]));
    int sub_cost;

    for (int i = 0; i < l1; ++i) {
        for (int j = 0; j < l2; ++j) {
            matrix[i][j] = 0;
        }
    }
 
    for (int i = 1; i < l1; ++i) {
        matrix[i][0] = i;
    }
 
    for (int j = 1; j < l2; ++j) {
        matrix[0][j] = j;
    }
 
    for (int j = 1; j < l2; ++j) {
        for (int i = 1; i < l1; ++i) {
            if (first_str[i - 1] == second_str[j - 1]) {
                sub_cost = 0;

            } else {
                sub_cost = 1;
            }
 
            if (matrix[i - 1][j] + 1 < matrix[i][j - 1] + 1 
                && matrix[i - 1][j] + 1 < matrix[i - 1][j - 1] + sub_cost) {
                matrix[i][j] = matrix[i - 1][j] + 1;

            }  else if (matrix[i][j - 1] + 1 < matrix[i - 1][j - 1] + sub_cost) {
                matrix[i][j] = matrix[i][j - 1] + 1;

            } else {
                matrix[i][j] = (matrix[i - 1][j - 1] + sub_cost);
            }
        }
    }
    
    int lev_dist = matrix[l1 - 1][l2 - 1];
    free(matrix);

    return lev_dist;
}

void shift(char *str, int offset, int len) {
    for (int i = 0; i < offset; ++i) {
        for (int j = 0; j < len; ++j) {
            str[j] = rotate(str[j]);
        }
    }
}

void decypher(char *cyphered_str, const char *overheard_str, int (*compare)(const char *first_str, const char *second_str)) {
    unsigned int len = strlen(cyphered_str);
    int best_res = compare == compare_hamming ? 0 : len;
    int offset = 0;

    for (int i = 0; i < ROTATIONS_COUNT; ++i) {
        for (int j = 0; j < len; ++j) {
            cyphered_str[j] = rotate(cyphered_str[j]);
        }

        int curr_res = (*compare)(cyphered_str, overheard_str);

        if ((compare == compare_hamming && curr_res > best_res) || 
            (compare == compare_levenstein && curr_res < best_res)) 
        {
            best_res = curr_res;
            offset = i + 1;

        } 
    }

    shift(cyphered_str, offset, len);
}

void handle_free(char *first_str, char *second_str) {
    if (first_str != NULL) {
        free(first_str);
    }

    if (second_str != NULL) {
        free(second_str);
    }
}

void handle_error(int code) {
    switch (code) {
        case ERROR_INPUT:
            fprintf(stderr, "Error: Chybny vstup!\n");
            break;
        case ERROR_RANGE:
            fprintf(stderr, "Error: Chybna delka vstupu!\n");
            break;
        default:
            break;
    }
}
