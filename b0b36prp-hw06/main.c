#include <stdio.h>
#include <stdlib.h>

enum error_codes {
    ERROR_INPUT = 100
};

#define MAX_AMOUNT 100

// Structure representing a matrix with rows, columns, an operation, and values
typedef struct {
    unsigned int rows;      // Number of rows in the matrix
    unsigned int cols;      // Number of columns in the matrix
    char op;       // Operation associated with the matrix ('+', '-', '*')
    int **values;  // Pointer to the 2D array of matrix values
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
 * @param mat - pointer to the matrix to be read
 * @return int - status code (EXIT_SUCCESS or EXIT_FAILURE)
 */
int read_mat(matrix **mat);

/**
 * @brief Reads an operation character from standard input.
 * 
 * @param op - pointer to the operation character
 * @return int - status code (EXIT_SUCCESS or EXIT_FAILURE)
 */
int read_op(char *op);

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
 * @brief Allocates memory for a matrix with the given rows and columns.
 * 
 * @param rows - number of rows
 * @param cols - number of columns
 * @return matrix* - pointer to the allocated matrix, or NULL if allocation fails
 */
matrix *allocate_matrix(unsigned int rows, unsigned int cols);

/**
 * @brief Adds or subtracts the second matrix from the first matrix based on the operation.
 * 
 * @param first_matrix - pointer to the first matrix (result is stored here)
 * @param second_matrix - pointer to the second matrix
 * @param op - the operation character ('+' or '-')
 */
void mat_add(matrix *first_matrix, const matrix *second_matrix, char op);

/**
 * @brief Multiplies two matrices and returns the result.
 * 
 * @param first_matrix - pointer to the first matrix
 * @param second_matrix - pointer to the second matrix
 * @return matrix* - pointer to the resulting matrix, or NULL if allocation fails
 */
matrix *mat_mul(const matrix *first_matrix, const matrix *second_matrix);

/**
 * @brief Resolves a list of matrices and performs the operations on them.
 * 
 * @param list - array of pointers to matrices
 * @param res - pointer to the resulting matrix
 * @param mat_count - number of matrices in the list
 * @return int - status code (EXIT_SUCCESS or EXIT_FAILURE)
 */
int resolve_list(matrix *list[MAX_AMOUNT], matrix **res, int mat_count);

/**
 * @brief Frees the memory allocated for a matrix.
 * 
 * @param mat - pointer to the matrix to be freed
 */
void handle_free(matrix **mat);

int main(int argc, char *argv[]) {
    int ret_code = EXIT_SUCCESS;
    
    char op = 0;

    matrix *mat = NULL;
    matrix *read_matrices[MAX_AMOUNT] = { NULL };
    int curr_count = 0;

    while (op != EOF) {
        if (read_mat(&mat) != EXIT_SUCCESS) {
            ret_code = ERROR_INPUT;
            break;
        }

        if (read_op(&op) != EXIT_SUCCESS) {
            ret_code = ERROR_INPUT;
            break;
        }

        mat->op = op;

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
            handle_free(&total);

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

    printf("%u %u\n", rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d%c", mat->values[i][j], j == cols - 1 ? '\n' : ' ');
        }
    }
}

int read_mat(matrix **mat) {
    int ret_code = EXIT_SUCCESS;

    unsigned int rows;
    unsigned int cols;

    if (scanf("%u %u", &rows, &cols) != 2) {
        ret_code = EXIT_FAILURE;
        return ret_code;
    }

    (*mat) = allocate_matrix(rows, cols); 

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (scanf("%d", &(*mat)->values[i][j]) != 1) {
                handle_free(mat);
                ret_code = EXIT_FAILURE;
                goto end_loop;
            }
        }
    }

    end_loop:
    return ret_code;
}

int read_op(char *op) {
    int ret_code = EXIT_SUCCESS;

    char read_char;
    if (scanf("\n%c", &read_char) != 1) {
        (*op) = EOF;

    } else if (read_char != '+' && read_char != '-' && read_char != '*') {
        ret_code = EXIT_FAILURE;

    } else {
        (*op) = read_char;
    }

    return ret_code;
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

void mat_add(matrix *first_matrix, const matrix *second_matrix, char op) {
    int rows = first_matrix->rows;
    int cols = first_matrix->cols;
    int mult = op == '+' ? 1 : -1;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            first_matrix->values[i][j] += mult * second_matrix->values[i][j];
        }
    }
}

matrix *mat_mul(const matrix *first_matrix, const matrix *second_matrix) {
    unsigned int rows = first_matrix->rows;
    unsigned int cols = second_matrix->cols;
    matrix *res = allocate_matrix(rows, cols);

    if (res != NULL) {
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
    }
     
    return res;
}

matrix *allocate_matrix(unsigned int rows, unsigned int cols) {
    matrix *mat = (matrix *) malloc(sizeof(matrix) + sizeof(int) * rows * cols);
    mat->rows = rows;
    mat->cols = cols;

    if (mat != NULL) {
        int **stored_values = (int **) malloc(sizeof(int *) * rows);

        if (stored_values == NULL) {
            handle_free(&mat);
            mat = NULL;
            goto end_alloc;
        }

        mat->values = stored_values;

        for (int i = 0; i < rows; ++i) {
            stored_values[i] = (int *) calloc(cols, sizeof(int));

            if (stored_values[i] == NULL) {
                handle_free(&mat);
                mat = NULL;
                goto end_alloc;
            }
        }
    }   

    end_alloc:
    return mat;
}

int resolve_list(matrix *list[MAX_AMOUNT], matrix **res, int mat_count) {
    int ret_code = EXIT_SUCCESS;
    matrix *mats_to_sum[MAX_AMOUNT] = { NULL };
    int curr_count = 0;

    for (int i = 0; i < mat_count; ++i) {
        if (list[i]->op == '*') {
            matrix *res = NULL;
            if (handle_sizes(list[i], list[i + 1], '*') == EXIT_SUCCESS && 
                ((res = mat_mul(list[i], list[i + 1])) != NULL)) 
            {
                res->op = list[i + 1]->op;
                if (res->op != '*') {
                    ++i;
                    mats_to_sum[curr_count++] = res;

                } else {
                    handle_free(&list[i + 1]);
                    list[i + 1] = res;
                }

            } else {
                ret_code = EXIT_FAILURE;
                goto end_resolve;
            }

        } else {
            mats_to_sum[curr_count++] = list[i];
            list[i] = NULL;
        }
    }
    
    matrix *total = mats_to_sum[0];
    for (int i = 1; i < curr_count; ++i) {
        if (handle_sizes(total, mats_to_sum[i], '+') != EXIT_SUCCESS) {
            ret_code = EXIT_FAILURE;
            goto end_resolve;
        }

        mat_add(total, mats_to_sum[i], total->op);
        total->op = mats_to_sum[i]->op;
    }

    end_resolve:
    *res = mats_to_sum[0];

    for (int i = 1; i < curr_count; ++i) {
        handle_free(&mats_to_sum[i]);
    }
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
