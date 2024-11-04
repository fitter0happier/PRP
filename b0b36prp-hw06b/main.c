#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum error_codes {
    ERROR_INPUT = 100
};

#define MAX_AMOUNT 26

// Structure representing a matrix with rows, columns, an operation, and values
typedef struct {
    char name;              // Name of the matrix
    unsigned int rows;      // Number of rows in the matrix
    unsigned int cols;      // Number of columns in the matrix
    int **values;           // Pointer to the 2D array of matrix values
} matrix;

/**
 * @brief Prints the matrix to standard output.
 * 
 * @param mat - pointer to the matrix to be printed
 */
void print_matrix(matrix *mat);

/**
 * @brief Reads a matrix from standard input.
 * 
 * @param name - letter representing the matrix
 * @return matrix* - read matrix
 */
matrix* read_mat(const char name);

/**
 * @brief Handles size compatibility for matrix operations.
 * 
 * @param first_mat - pointer to the first matrix
 * @param second_mat - pointer to the second matrix
 * @param op - the operation character ('+', '-', '*')
 * @return int - status code (EXIT_SUCCESS or EXIT_FAILURE)
 */
int handle_sizes(const matrix *first_mat, const matrix *second_mat, const char op);

/**
 * @brief Adds or subtracts the second matrix from the first matrix based on the operation.
 * 
 * @param first_matrix - pointer to the first matrix (result is stored here)
 * @param second_matrix - pointer to the second matrix
 * @param op - the operation character ('+' or '-')
 * @return matrix* - pointer to the resulting matrix, or NULL if allocation fails
 */
matrix* mat_add(matrix *first_matrix, const matrix *second_matrix, char op);

/**
 * @brief Multiplies two matrices and returns the result.
 * 
 * @param first_matrix - pointer to the first matrix
 * @param second_matrix - pointer to the second matrix
 * @return matrix* - pointer to the resulting matrix, or NULL if allocation fails
 */
matrix* mat_mul(const matrix *first_matrix, const matrix *second_matrix);

/**
 * @brief Resolves a list of matrices and performs the operations on them.
 * 
 * @param list - array of pointers to matrices
 * @param res - pointer to the resulting matrix
 * @param mat_count - amount of matrices read as init length of list
 * @return int - status code (EXIT_SUCCESS or EXIT_FAILURE)
 */
int resolve_list(matrix *list[MAX_AMOUNT], matrix **res, const int mat_count);

/**
 * @brief Frees the memory allocated for a matrix.
 * 
 * @param mat - pointer to the matrix to be freed
 */
void handle_free(matrix **mat);

int main(int argc, char *argv[]) {
    int ret_code = EXIT_SUCCESS;
    char c;

    matrix *mat = NULL;
    matrix *read_matrices[MAX_AMOUNT] = { NULL };
    int curr_count = 0;

    // Read matrices
    while ((c = getchar()) != '\n') {
        mat = read_mat(c);
        read_matrices[curr_count] = mat;
        ++curr_count;
    }

    if (ret_code == ERROR_INPUT) {
        fprintf(stderr, "Error: Chybny vstup!\n");

    } else {
        matrix *total = NULL;
        if (resolve_list(read_matrices, &total, curr_count) != EXIT_SUCCESS) {
            ret_code = ERROR_INPUT;
            fprintf(stderr, "Error: Chybny vstup!\n");

        } else {
            print_matrix(total);
            handle_free(&total);
        }
    }

    for (int i = 0; i < curr_count; ++i) {
        handle_free(&read_matrices[i]);
    }

    return ret_code;   
}

void print_matrix(matrix *mat) {
    unsigned int rows = mat->rows;
    unsigned int cols = mat->cols;

    printf("[");

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d", mat->values[i][j]);

            if (j != cols - 1) {
                printf(" ");
            }
        }

        if (i != (rows - 1)) {
            printf("; ");
        }
    }

    printf("]\n");
}

matrix* read_mat(const char name) {
    unsigned int rows = 0;
    unsigned int cols = 0;

    char c;

    int curr_row_count = 1;
    int curr_col_count = 1;

    int **stored_values = (int **)malloc(sizeof(int *));
    int *row = NULL;
    
    // Skip "="
    getchar();

    int i = 0;

    while ((c = getchar()) != '\n') {
        if (c == '[' || c == ' ') {
            row = (int *)malloc(sizeof(int) * curr_col_count);
            continue;
        }

        int sign = 1;
        int value = 0;

        if (c == '-') {
            sign = -1;
            c = getchar();
        }

        while (c != ' ' && c != ';' && c != ']') {
            value = value * 10 + (c - '0');
            c = getchar();
        }

        if (rows == curr_row_count) {
            curr_row_count *= 2;
            stored_values = (int **)realloc(stored_values, sizeof(int *) * curr_row_count);
        } 
        
        if (i == curr_col_count) {
            curr_col_count *= 2;
            row = (int *)realloc(row, sizeof(int) * curr_col_count);
        } 

        row[i++] = sign * value;

        if (c == ';' || c == ']') {
            stored_values[rows] = row;
            rows += 1;

            if (cols == 0) {
                cols = i;
            }
            
            i = 0;
        }
    }

    matrix *mat = (matrix *)malloc(sizeof(matrix));
    mat->name = name;
    mat->rows = rows;
    mat->cols = cols;
    mat->values = stored_values;

    return mat;
}

int handle_sizes(const matrix *first_mat, const matrix *second_mat, const char op) {
    int ret_code = EXIT_SUCCESS;

    if ((op == '+' || op == '-') && 
        (first_mat->rows != second_mat->rows || first_mat->cols != second_mat->cols))
    {
        ret_code = EXIT_FAILURE;

    } else if (op == '*' && first_mat->cols != second_mat->rows) {
        ret_code = EXIT_FAILURE;
    }

    return ret_code;
}

matrix* mat_add(matrix *first_matrix, const matrix *second_matrix, char op) {
    int rows = first_matrix->rows;
    int cols = first_matrix->cols;
    int mult = op == '+' ? 1 : -1;

    matrix *res = (matrix *)malloc(sizeof(matrix));
    int **stored_values = (int **)malloc(sizeof(int *) * rows);

    for (int i = 0; i < rows; ++i) {
        stored_values[i] = (int *)malloc(sizeof(int) * cols);
    }

    res->rows = rows;
    res->cols = cols;
    res->values = stored_values;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            res->values[i][j] = first_matrix->values[i][j];
            res->values[i][j] += (mult * second_matrix->values[i][j]);
        }
    }

    return res;
}

matrix* mat_mul(const matrix *first_matrix, const matrix *second_matrix) {
    unsigned int rows = first_matrix->rows;
    unsigned int cols = second_matrix->cols;

    matrix *res = (matrix *)malloc(sizeof(matrix));
    int **stored_values = (int **)malloc(sizeof(int *) * rows);

    for (int i = 0; i < rows; ++i) {
        stored_values[i] = (int *)malloc(sizeof(int) * cols);
    }

    res->rows = rows;
    res->cols = cols;
    res->values = stored_values;

    int second_matrix_rows = second_matrix->rows;
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            res->values[i][j] = 0;
        }
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < second_matrix_rows; ++k) {
                int mult = first_matrix->values[i][k] * second_matrix->values[k][j];
                res->values[i][j] += mult;
            }
        }
    }
     
    return res;
}

int resolve_list(matrix *list[MAX_AMOUNT], matrix **res, const int mat_count) {
    int ret_code = EXIT_SUCCESS;

    char name;
    char op;

    matrix **matrices_to_add = (matrix **)malloc(sizeof(matrix *) * mat_count);
    char *ops_to_add = (char *)malloc(mat_count);
    int add_list_length = mat_count;
    int curr_count_to_add = 0;

    matrix **matrices_to_free = (matrix **)malloc(sizeof(matrix *) * mat_count);
    int free_list_length = mat_count;
    int curr_count_to_free = 0;

    matrix *curr_mat = NULL;


    while (true) {
        name = getchar();
        op = getchar();

        for (int i = 0; i < mat_count; ++i) {
            if (name == list[i]->name) {
                curr_mat = list[i];
            }
        }

        while (op == '*') {
            name = getchar();

            matrix *second_mat = NULL;
            for (int i = 0; i < mat_count; ++i) {
                if (name == list[i]->name) {
                    second_mat = list[i];
                }
            }

            if (handle_sizes(curr_mat, second_mat, '*') != EXIT_SUCCESS) {
                ret_code = EXIT_FAILURE;
                goto end_resolve;
            }

            if (curr_count_to_free == free_list_length) {
                free_list_length *= 2;

                matrices_to_free = (matrix **)realloc(
                    matrices_to_free, 
                    sizeof(matrix *) * free_list_length
                );
            }

            matrix *res = mat_mul(curr_mat, second_mat);
            matrices_to_free[curr_count_to_free++] = res;

            curr_mat = res;

            op = getchar();
        }

        if (curr_count_to_add == add_list_length) {
            add_list_length *= 2;

            matrices_to_add = (matrix **)realloc(
                matrices_to_add, 
                sizeof(matrix *) * add_list_length
            );

            ops_to_add = (char *)realloc(ops_to_add, add_list_length);
        }

        matrices_to_add[curr_count_to_add] = curr_mat;

        if (op == '\n') {
            break;

        } else {
            ops_to_add[curr_count_to_add++] = op;
        }
    }
    
    matrix *total = matrices_to_add[0];
    for (int i = 0; i < curr_count_to_add; ++i) {
        if (handle_sizes(total, matrices_to_add[i + 1], '+') != EXIT_SUCCESS) {
            ret_code = EXIT_FAILURE;
            goto end_resolve;
        }

        if (curr_count_to_free == free_list_length) {
            free_list_length *= 2;

            matrices_to_free = (matrix **)realloc(
                matrices_to_free, 
                sizeof(matrix *) * free_list_length
            );
        }

        total = mat_add(total, matrices_to_add[i + 1], ops_to_add[i]);
        matrices_to_free[curr_count_to_free++] = total;
    }

    *res = total;

    end_resolve:
    for (int i = 0; i < curr_count_to_add; ++i) {
        matrices_to_add[i] = NULL;
    }
    free(matrices_to_add);

    for (int i = 0; i < curr_count_to_free - 1; ++i) {
        handle_free(&matrices_to_free[i]);
    }

    if (ret_code == EXIT_FAILURE) {
        handle_free(&matrices_to_free[curr_count_to_free - 1]);
    }

    free(matrices_to_free);
    free(ops_to_add);

    return ret_code;
}

void handle_free(matrix **mat) {
    if (*mat == NULL) {
        return;
    }

    if ((*mat)->values == NULL) {
        free(*mat);
        return;
    }

    int rows = (*mat)->rows;

    for (int i = 0; i < rows; ++i) {
        if ((*mat)->values[i] != NULL) {
            free((*mat)->values[i]);
            (*mat)->values[i] = NULL;
        }
    }

    free((*mat)->values);
    free(*mat);
    *mat = NULL;
}

