#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define COLOR_START "\033[01;31m\033[K"
#define COLOR_END "\033[m\033[K"

/* Searches a provided pattern in file and prints out lines if found */
void search(FILE *file, const char *pattern, bool coloring, bool *found);

/* Searches a provided regex pattern in file and prints out lines if found */
void search_with_regex(FILE *file, const char *pattern, bool *found);

/* Compares two strings, true if strings are identical, false otherwise */
bool string_compare(const char* first_str, const char* second_str);

int main(int argc, char *argv[]) {
    FILE *file = NULL;
    char *pattern = NULL;
    bool regex = false;
    bool coloring = false;

    switch (argc) {
        case 1:
            fprintf(stderr, "Provide command line arguments.\n");
            return EXIT_FAILURE;

        case 2:
            file = stdin;
            pattern = argv[1];
            break;

        case 3:
            if (string_compare(argv[1], "-E")) {
                regex = true;
                file = stdin;
                pattern = argv[2];

            } else if (string_compare(argv[1], "--color=always")) {
                coloring = true;
                file = stdin;
                pattern = argv[2];

            } else {
                file = fopen(argv[2], "r");

                if (file == NULL) {
                    fprintf(stderr, "Could not open file.\n");
                    return errno;
                }

                pattern = argv[1];
            }

            break;

        case 4:
            file = fopen(argv[3], "r");

            if (file == NULL) {
                fprintf(stderr, "Could not open file.\n");
                return errno;
            }

            if (string_compare(argv[1], "-E")) {
                regex = true;

            } else if (string_compare(argv[1], "--color=always")) {
                coloring = true;
            }

            pattern = argv[2];
            break;

        default:
            fprintf(stderr, "Too many command line arguments.\n");
            return EXIT_FAILURE;
    }

    bool found = false;

    char pattern_buffer[BUFFER_SIZE];
    int i = 0;
    while (pattern[i] != '\0') {
        pattern_buffer[i] = pattern[i];
        i += 1;
    }
    pattern_buffer[i] = '\0';
    pattern = pattern_buffer;
    
    if (regex) {
        search_with_regex(file, pattern, &found);

    } else {
        search(file, pattern, coloring, &found);
    }

    if (file != stdin && fclose(file) != EXIT_SUCCESS) {
        fprintf(stderr, "Could not close file.\n");
        return errno;
    }

    int ret = found ? EXIT_SUCCESS : EXIT_FAILURE;
    return ret;
}

bool string_compare(const char* first_str, const char* second_str) {
    bool res = true;
    int curr_index = 0;

    while (first_str[curr_index] != '\0' || second_str[curr_index] != '\0') {
        if (first_str[curr_index] != second_str[curr_index]) {
            res = false;
            break;
        }

        ++curr_index;
    }

    if (first_str[curr_index] != second_str[curr_index]) {
        res = false;
    }

    return res;
}

void search(FILE *file, const char *pattern, bool coloring, bool *found) {
    char buffer[BUFFER_SIZE];
    
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        char c = 0;

        bool in_pattern = false;
        bool pattern_found = false;
        int pattern_index = 0;

        int color_indices[BUFFER_SIZE] = { 0 };
        int color_index = 0;

        for (int i = 0; buffer[i] != '\0'; ++i) {   
            c = buffer[i];

            if (!in_pattern) {
                if (c == pattern[pattern_index]) {
                    in_pattern = true;
                    ++pattern_index;
                    color_indices[color_index++] = i;

                    // Single - letter pattern
                    if (pattern[pattern_index] == '\0') { 
                        color_indices[color_index++] = i;
                        pattern_found = true;
                        in_pattern = false;
                        pattern_index = 0;
                    }
                }

            } else if (in_pattern && c == pattern[pattern_index]) {
                ++pattern_index;

                if (pattern[pattern_index] == '\0') {
                    color_indices[color_index++] = i;
                    pattern_found = true;
                    in_pattern = false;
                    pattern_index = 0;
                }

            } else if (in_pattern && c != pattern[pattern_index]) {
                pattern_index = 0;
                in_pattern = false;
                
                // If the wrong letter is also the first letter of pattern
                if (c == pattern[pattern_index]) {
                    in_pattern = true;
                    ++pattern_index;
                    color_indices[color_index++] = i;
                }
            } 
        }

        if (pattern_found) {
            *found = true;
            int color_index = 0;

            for (int i = 0; buffer[i] != '\0'; ++i) {
                if (coloring) {
                    if (color_indices[color_index] == i) {
                        // Single - letter pattern
                        if (color_indices[color_index] == color_indices[color_index + 1]) {
                            printf(COLOR_START);
                            printf("%c", buffer[i]);
                            printf(COLOR_END);

                            color_index += 2;
                        } 

                        // Otherwise
                        else {
                            if (color_index % 2 == 0) {
                                printf(COLOR_START);
                                printf("%c", buffer[i]); 

                            } else {
                                printf("%c", buffer[i]); 
                                printf(COLOR_END);
                            }

                            ++color_index;
                        }

                    } else {
                       printf("%c", buffer[i]); 
                    }

                } else {
                    printf("%c", buffer[i]);
                }
            }
        }

        pattern_found = false;
    }
}

void search_with_regex(FILE *file, const char *pattern, bool *found) {
    char regex = 0;
    char regex_char = 0;
    int regex_position = 0;

    char string_without_regex[BUFFER_SIZE];
    int index = 0;

    for (int i = 0; pattern[i] != '\0'; ++i) {
        if (pattern[i] == '*' 
            || pattern[i] == '?' 
            || pattern[i] == '+') 
        {
            regex = pattern[i];
            regex_char = pattern[i - 1];
            regex_position = i - 1;
        } else {
            string_without_regex[index++] = pattern[i];
        }
    }

    string_without_regex[index] = '\0';
    char buffer[BUFFER_SIZE];

    bool in_pattern = false;
    bool pattern_found = false;
    int pattern_index = 0;

    char c = 0;

    while(fgets(buffer, BUFFER_SIZE, file) != NULL) {
        if (regex == '?') {
            for (int i = 0; buffer[i] != '\0'; ++i) {
                c = buffer[i];

                if (!in_pattern) {
                    if (c == string_without_regex[pattern_index]) {
                        in_pattern = true;
                        ++pattern_index;

                    } else if (pattern_index == regex_position) {
                        ++pattern_index;
                        --i;
                    }

                } else if (in_pattern && c == string_without_regex[pattern_index]) {
                    ++pattern_index;

                } else if (in_pattern && c != string_without_regex[pattern_index]) {
                    if (pattern_index == regex_position) {
                        ++pattern_index;
                        --i;

                    } else {
                        pattern_index = 0;
                        in_pattern = false;
                    }
                }

                if (string_without_regex[pattern_index] == '\0') {
                    pattern_index = 0;
                    pattern_found = true;
                    break;
                }
            }
        }

        else if (regex == '*') {
            for (int i = 0; buffer[i] != '\0'; ++i) {
                c = buffer[i];

                if (!in_pattern) {
                    if (c == string_without_regex[pattern_index]) {
                        if (pattern_index == regex_position) {
                            while (buffer[i + 1] == regex_char) {
                                ++i;
                            }
                        }

                        in_pattern = true;
                        ++pattern_index;

                    } else if (pattern_index == regex_position) {
                        ++pattern_index;
                        --i;
                    }

                } else if (in_pattern && c == string_without_regex[pattern_index]) {
                    if (pattern_index == regex_position) {
                        while (buffer[i + 1] == regex_char) {
                            ++i;
                        }
                    }

                    ++pattern_index;

                } else if (in_pattern && c != string_without_regex[pattern_index]) {
                    if (pattern_index == regex_position) {
                        ++pattern_index;
                        --i;

                    } else {
                        pattern_index = 0;
                        in_pattern = false;
                    }
                }

                if (string_without_regex[pattern_index] == '\0') {
                    pattern_index = 0;
                    pattern_found = true;
                    break;
                }
            }
        } 

        else if (regex == '+') {
            for (int i = 0; buffer[i] != '\0'; ++i) {
                c = buffer[i];

                if (!in_pattern) {
                    if (c == string_without_regex[pattern_index]) {
                        if (pattern_index == regex_position) {
                            while (buffer[i + 1] == regex_char) {
                                ++i;
                            }
                        }

                        in_pattern = true;
                        ++pattern_index;
                    } 

                } else if (in_pattern && c == string_without_regex[pattern_index]) {
                    if (pattern_index == regex_position) {
                        while (buffer[i + 1] == regex_char) {
                            ++i;
                        }
                    }

                    ++pattern_index;

                } else if (in_pattern && c != string_without_regex[pattern_index]) {
                    pattern_index = 0;
                    in_pattern = false;
                }

                if (string_without_regex[pattern_index] == '\0') {
                    pattern_index = 0;
                    pattern_found = true;
                    break;
                }
            }
        }

        if (pattern_found) {
            *found = true;
            printf("%s", buffer);
            pattern_found = false;
        }
    }
}
