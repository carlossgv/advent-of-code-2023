#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "/opt/homebrew/opt/libomp/include/omp.h"

typedef struct {
    unsigned char *spaces;
    int width;
    int height;
} Base;

Base *create_base(unsigned char *input);
int calc_north_load(Base *base);
void get_diagram(Base *base, char *output);
void get_dimensions(unsigned char *input, int *width, int *height);
void cycle(Base *base);
void tilt(Base *base, char direction);
void north_tilt(Base *base);
void west_tilt(Base *base);
void south_tilt(Base *base);
void east_tilt(Base *base);

Base *create_base(unsigned char *input) {
    Base *base = (Base *)malloc(sizeof(Base));
    base->spaces = input;
    get_dimensions(input, &base->width, &base->height);
    return base;
}

int calc_north_load(Base *base) {
    int result = 0;
    for (int x = 0; x < base->width; x++) {
        for (int y = 0; y < base->height; y++) {
            int index = x + x * base->width + y;
            unsigned char current = base->spaces[index];

            if (current == 'O') {
                result += base->height - x;
            }
        }
    }
    return result;
}

void get_diagram(Base *base, char *output) {
    strcpy(output, "\n");
    for (int i = 0; i < base->spaces[i] != '\0'; i++) {
        sprintf(output + strlen(output), "%c", base->spaces[i]);
    }
}

void get_dimensions(unsigned char *input, int *width, int *height) {
    *width = strchr((char *)input, '\n') - (char *)input;
    *height = strlen((char *)input) / (*width + 1);
}

void cycle(Base *base) {
    char directions[] = {'N', 'W', 'S', 'E'};
    for (int i = 0; i < 4; i++) {
        tilt(base, directions[i]);
    }
}

void tilt(Base *base, char direction) {
    switch (direction) {
        case 'N':
            north_tilt(base);
            break;
        case 'S':
            south_tilt(base);
            break;
        case 'E':
            east_tilt(base);
            break;
        case 'W':
            west_tilt(base);
            break;
    }
}

void north_tilt(Base *base) {
    printf("North tilt: Thread %d starting\n", omp_get_thread_num());
    #pragma omp parallel for
    for (int y = 0; y < base->height; y++) {
        // run each column in order
        int barrier_index = -(base->width + 1) + y;
        for (int x = 0; x < base->width; x++) {
            int index = x + x * base->width + y;
            unsigned char current = base->spaces[index];

            if (current == '#') {
                barrier_index = index;
                continue;
            }

            if (current == 'O') {
                #pragma omp critical
                {
                    base->spaces[index] = '.';
                    barrier_index += base->width + 1;
                    base->spaces[barrier_index] = 'O';
                }
            }
        }
    }
    printf("North tilt: Thread %d finished\n", omp_get_thread_num());
}

void west_tilt(Base *base) {
    // run all rows in parallel
    #pragma omp parallel for
    for (int y = 0; y < base->height; y++) {
        int barrier_index = -1;
        for (int x = 0; x < base->width; x++) {
            int index = y * base->width + y + x;
            if (x == 0) {
                barrier_index = index - 1;
            }
            unsigned char current = base->spaces[index];

            if (current == '#') {
                barrier_index = index;
                continue;
            }

            if (current == 'O') {
                #pragma omp critical
                {
                    base->spaces[index] = '.';
                    barrier_index += 1;
                    base->spaces[barrier_index] = 'O';
                }
            }
        }
    }
}

void south_tilt(Base *base) {
    // run all rows in parallel
    #pragma omp parallel for
    for (int y = 0; y <= base->height; y++) {
        int barrier_index = base->width + 1 + y;
        for (int x = base->width - 1; x >= 0; x--) {
            int index = x + x * base->width + y;
            if (x == base->width - 1) {
                barrier_index = index + base->width + 1;
            }

            unsigned char current = base->spaces[index];

            if (current == '#') {
                barrier_index = index;
                continue;
            }

            if (current == 'O') {
                #pragma omp critical
                {
                    base->spaces[index] = '.';
                    barrier_index -= base->width + 1;
                    base->spaces[barrier_index] = 'O';
                }
            }
        }
    }
}

void east_tilt(Base *base) {
    // run all rows in parallel
    #pragma omp parallel for
    for (int y = 0; y < base->height; y++) {
        int barrier_index = base->width;
        for (int x = base->width - 1; x >= 0; x--) {
            int index = y * base->width + y + x;
            if (x == base->width - 1) {
                barrier_index = index + 1;
            }

            unsigned char current = base->spaces[index];

            if (current == '#') {
                barrier_index = index;
                continue;
            }

            if (current == 'O') {
                #pragma omp critical
                {
                    base->spaces[index] = '.';
                    barrier_index -= 1;
                    base->spaces[barrier_index] = 'O';
                }
            }
        }
    }
}

int main() {
    FILE *file = fopen("../src/input.txt", "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open input file\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long input_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *input = (unsigned char *)malloc(input_size + 1);
    fread(input, 1, input_size, file);
    input[input_size] = '\0';
    fclose(file);

    int cycles = 100000;
    printf("Running %d cycles...\n", cycles);

    clock_t start = clock();

    Base *base = create_base(input);
    for (int i = 0; i < cycles; i++) {
        cycle(base);
    }

    printf("North load: %d\n", calc_north_load(base));
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.2f seconds\n", elapsed_time);

    free(input);
    free(base);
    return 0;
}
