#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <string.h>
#include <limits.h>

#include <math.h>
#include <time.h>

#include <dirent.h>

#define RESPONSE_BUFFER 4
#define LINES_IN_FILE 5

#define ACCOUNT_RAND_RANGE_HIGH 1000000
#define ACCOUNT_RAND_RANGE_LOW 10000

#define TRYAGAIN " Please try again."

const char* DIR_NAME = "database";
const char* TEMPFILE_NAME = "tempfile";

// math related
// count number of digits in an integer
int count_digits_with_base(int number, int base) {
    int n = 1, ret = 0;
    do {
        number /= base;
        ret++;
    } while (number > 0);
    return ret;
}

// Prevent too many floating points in input
int tooManyFPoints(char* str) {
    int i = 0;
    while (*str != '.') str++;
    str++;
    while (*str != '\0') 
    {
        if (!isdigit(*str)) break;
        i++; 
        str++;
    }
    // "00" == len 2
    return i > 2;
}

// counts the floor of a double
double double_floor(double value, int decimal_places) {
    if (decimal_places == -1) return 0;
    return (int) (value * pow(10, decimal_places)) / (double) pow(10, decimal_places);
}


// buffer clearing 
void clear_buffer() { 
    char c;
    while ((c = getchar()) != '\n' && c != EOF); 
}

void conditional_fgets_buffer_clearing(char *input) {
    if (input[strlen(input) - 1] != '\n') clear_buffer();
    else input[strlen(input) - 1] = '\0';   
}


// printf aesthetic
void print_dashes(int count) {
    for (int i = 0; i < count; i++) printf("-");
}

void header_ezprint(char header[]) {
    printf("\n\n");
    print_dashes(30);
    printf(" %s ", header);
    print_dashes(30);
    printf("\n");
}

void main_menu() {
    printf("\n\n\n");
    print_dashes(30);
    printf("\nBanking System Application: \n");
    print_dashes(30);
    printf("\nPlease choose an option: \n");
    printf("1. Create new bank account \n");
    printf("2. Deposit\n");
    printf("3. Withdrawal\n");
    printf("4. Remittance\n");
    printf("5. Delete bank account\n");
    printf("[q to Quit]\n");
    print_dashes(30);
    printf("\n");
    printf(">>> ");
}

void print_options_header(const char* option_name, int option_numbers) {
    if (option_numbers == 1) printf("Please choose an option to %s (1): [q to quit]", option_name, option_numbers);
    else printf("Please choose an option to %s (1 - %d): [q to quit]", option_name, option_numbers);
    printf("\n");
}


// string operations
void strip(char *str) {
    // null
    if (str == NULL) return;

    // find 1st char (non-space)
    char *start = str;
    while (*start && isspace(*start)) {
        start++;
    }

    // empty str
    if (*start == '\0') {
        str[0] = '\0';
        return;
    }

    // del end
    char *end = str + strlen(str) - 1;
    while (end > start && isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';

    // shift position (if got space in front of str)
    if (start != str) {
        memmove(str, start, end - start + 2); // +2 to include the null terminator
    }

}

// check if input is float
int isfloat(char* str) {
    int dot = 0;
    if (*str == '\0') return 0;
    while (*str != '\0')
    {
        if (!(isdigit(*str) || *str == '.')) return 0;
        if (*str == '.' && dot == 1) return 0;
        if (*str == '.' && dot == 0) dot = 1;
        str++;
    }
    if (*(str - 1) == '.') return 0;
    return 1;
}

int isname(char* str) { 
    // strip before using this function
    if (*str == '\0') return 0;
    while (*str != '\0')
    {
        if (!isalpha(*str) && *str != ' ' || (*str == ' ' && *(str+1) == ' ')) return 0;
        str++;
    }
    return 1;
}


// file operations
char *remove_extentions(char* str) {
    char *end = str + strlen(str) - 1;

    while (end > str && *end != '.') {
        end--;
    }

    if (*end == '.') {
        *end = '\0';
    }
    return str;
}

// count number of files in directly
int count_files_in_dir(const char *path) {
    int i = 0;
    int file_count = 0;
    struct dirent *entry;

    DIR *dir = opendir(path);
    if (dir == NULL) {
        return -1;
    }

    // https://codeforwin.org/c-programming/c-program-to-list-all-files-in-a-directory-recursively
    char full_path[PATH_MAX];
    char file_num[16];
    int num;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp("..", entry -> d_name) == 0 || strcmp(".", entry -> d_name) == 0) continue;
        
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        const char *filename = entry->d_name;
        int len = strlen(filename);
        if (strcmp(filename + len - 4, ".txt") == 0) {
            
            strncpy(file_num, filename, len - 4);
            file_num[len - 4] = '\0';

            for (int _ = 0; _ < strlen(file_num); _++)
            {
                if (!isdigit(file_num[_]) || file_num[0] == '0') {
                    printf("Unexpected file name: %s\nExiting...", full_path);
                    exit(EXIT_FAILURE);
                }
            }
            
            num = atoi(file_num);
            if (num >= ACCOUNT_RAND_RANGE_LOW && num <= ACCOUNT_RAND_RANGE_HIGH) {
                file_count++;
            } else {
                printf("Unexpected file name: %s\nExiting...", full_path);
                exit(EXIT_FAILURE);
            }
        }

    }
    closedir(dir);
    return file_count;
    
}

// initialize the paths of a directory into an array
void path_array_init(char paths[][count_digits_with_base(ACCOUNT_RAND_RANGE_HIGH, 10) + 1], int len, const char* dir_name) {
    struct dirent *entry;

    DIR *dir = opendir(dir_name);
    if (dir == NULL) {
        perror("Could not open directory");
        return;
    }

    int i = 0;
    char full_path[PATH_MAX];
    while ((entry = readdir(dir)) != NULL && i < len) {
        if (strcmp("..", entry -> d_name) == 0 || strcmp(".", entry -> d_name) == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", dir_name, entry->d_name);

        strncpy(paths[i], entry->d_name, sizeof(paths[i]) - 1);
        paths[i][sizeof(paths[i]) - 1] = '\0';  // Null-terminate

        char *dot = strrchr(paths[i], '.');
        if (dot != NULL) {
            *dot = '\0';  // Truncate at the dot
        }
        
        i++;
        
    }
    closedir(dir);
}

// check if file contents are valid
int check_file(char path[]) {
    FILE* fptr = fopen(path, "r");

    char contents[LINES_IN_FILE * 2][BUFSIZ];
    for (int _ = 0; _ < LINES_IN_FILE; _++) contents[_][0] = '\0';
    
    double balance;
    
    int i = 0;
    while (fgets(contents[i], sizeof(contents[0]), fptr)) {
        strip(contents[i]);
        i++;
    }
    if (i != 5) return 0;

    int line0 = strlen(contents[0]) <= 32 && strlen(contents[0]) >= 3;

    int line1 = strlen(contents[1]) == 12;
    for (int i = 0; i < 12; i++)
    {
        if (line1 == 0) return 0;
        if (!isdigit(contents[1][i])) line1 = 0;
        else line1 = 1;
    }
    
    char* ptr = contents[2];
    while (*ptr != '\0')
    {
        if (!isdigit(contents[1][i])) return 0;
        ptr++;
    }
    int line2 = atoi(contents[2]) >= ACCOUNT_RAND_RANGE_LOW && atoi(contents[2]) <= ACCOUNT_RAND_RANGE_HIGH;
    int line3 = strcmp(contents[3], "1") == 0 || strcmp(contents[3], "2") == 0;
    int line4 = isfloat(contents[4]);

    fclose(fptr);
    return line0 && line1 && line2 && line3 && line4;
    
}