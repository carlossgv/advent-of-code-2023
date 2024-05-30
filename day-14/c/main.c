#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define INPUT_FILE "../input.txt"

// ASCII codes
#define DOT 46
#define HASH 35
#define O 79
#define NEWLINE 10

typedef struct {
    uint8_t *spaces;
    int width;
    int height;
} Base;

void get_dimensions(uint8_t *base, int len, int *width, int *height) {
    *width = strchr((char *)base, NEWLINE) - (char *)base;
    *height = len / (*width + 1);
}

int calc_north_load(Base *base) {
    int result = 0;
    for (int x = 0; x < base->width; x++) {
        for (int y = 0; y < base->height; y++) {
            int index = x + x * base->width + y;
            uint8_t current = base->spaces[index];
            if (current == O) {
                result += base->height - x;
            }
        }
    }
    return result;
}

void north_tilt(Base *base) {
    for (int y = 0; y < base->height; y++) {
        int barrier_index = -(base->width + 1) + y;
        for (int x = 0; x < base->width; x++) {
            int index = x + x * base->width + y;
            uint8_t current = base->spaces[index];
            if (current == HASH) {
                barrier_index = index;
                continue;
            }
            if (current == O) {
                base->spaces[index] = DOT;
                barrier_index += (base->width + 1);
                base->spaces[barrier_index] = O;
            }
        }
    }
}

void west_tilt(Base *base) {
    for (int y = 0; y < base->height; y++) {
        int barrier_index = -1;
        for (int x = 0; x < base->width; x++) {
            int index = y * base->width + y + x;
            if (x == 0) {
                barrier_index = index - 1;
            }
            uint8_t current = base->spaces[index];
            if (current == HASH) {
                barrier_index = index;
                continue;
            }
            if (current == O) {
                base->spaces[index] = DOT;
                barrier_index += 1;
                base->spaces[barrier_index] = O;
            }
        }
    }
}

void south_tilt(Base *base) {
    for (int y = 0; y <= base->height; y++) {
        int barrier_index = base->width + 1 + y;
        for (int x = base->width - 1; x >= 0; x--) {
            int index = x + x * base->width + y;
            if (x == base->width - 1) {
                barrier_index = index + base->width + 1;
            }
            uint8_t current = base->spaces[index];
            if (current == HASH) {
                barrier_index = index;
                continue;
            }
            if (current == O) {
                base->spaces[index] = DOT;
                barrier_index -= (base->width + 1);
                base->spaces[barrier_index] = O;
            }
        }
    }
}

void east_tilt(Base *base) {
    for (int y = 0; y < base->height; y++) {
        int barrier_index = base->width;
        for (int x = base->width - 1; x >= 0; x--) {
            int index = y * base->width + y + x;
            if (x == base->width - 1) {
                barrier_index = index + 1;
            }
            uint8_t current = base->spaces[index];
            if (current == HASH) {
                barrier_index = index;
                continue;
            }
            if (current == O) {
                base->spaces[index] = DOT;
                barrier_index -= 1;
                base->spaces[barrier_index] = O;
            }
        }
    }
}

void cycle(Base *base) {
    north_tilt(base);
    west_tilt(base);
    south_tilt(base);
    east_tilt(base);
}

void run_cycles(uint8_t *input, int len, int cycles) {
    Base base;
    base.spaces = input;
    get_dimensions(input, len, &base.width, &base.height);

    for (int i = 0; i < cycles; i++) {
        cycle(&base);
    }

    printf("North Load: %d\n", calc_north_load(&base));
}

int main() {
    // Read input from file
    FILE *file = fopen(INPUT_FILE, "r");
    if (file == NULL) {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long input_len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the input
    uint8_t *input = (uint8_t *)malloc(input_len);
    if (input == NULL) {
        perror("Failed to allocate memory for input");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Read the file into the input buffer
    if (fread(input, 1, input_len, file) != input_len) {
        perror("Failed to read input file");
        free(input);
        fclose(file);
        return EXIT_FAILURE;
    }
    fclose(file);

    int cycles = 100000;
    printf("Running %d cycles...\n", cycles);
    clock_t start = clock();
    run_cycles(input, input_len, cycles);
    clock_t end = clock();
    printf("Time: %ldms\n", (end - start) * 1000 / CLOCKS_PER_SEC);

    free(input);
    return 0;
}
