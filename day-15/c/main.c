#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char *label;
    unsigned short box_;
    char operand;
    int focal_length;
} Label;

typedef struct {
    Label *labels;
    size_t size;
    size_t capacity;
} LabelArray;

typedef struct {
    unsigned short box_;
    LabelArray label_array;
} HashMapEntry;

typedef struct {
    HashMapEntry *entries;
    size_t size;
    size_t capacity;
} HashMap;

unsigned short hash_char(char c, unsigned short hash) {
    hash += (unsigned short)c;
    hash = (unsigned short)((hash * 17) % 256);
    return hash;
}

unsigned short hash_string(const char *s, unsigned short hash) {
    while (*s) {
        hash = hash_char(*s++, hash);
    }
    return hash;
}

void init_label_array(LabelArray *arr) {
    arr->size = 0;
    arr->capacity = 4;
    arr->labels = malloc(arr->capacity * sizeof(Label));
}

void append_label_array(LabelArray *arr, Label label) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->labels = realloc(arr->labels, arr->capacity * sizeof(Label));
    }
    arr->labels[arr->size++] = label;
}

void init_hash_map(HashMap *map) {
    map->size = 0;
    map->capacity = 4;
    map->entries = malloc(map->capacity * sizeof(HashMapEntry));
}

void insert_hash_map(HashMap *map, unsigned short box_, Label label) {
    for (size_t i = 0; i < map->size; i++) {
        if (map->entries[i].box_ == box_) {
            LabelArray *labels = &map->entries[i].label_array;
            for (size_t j = 0; j < labels->size; j++) {
                if (strcmp(labels->labels[j].label, label.label) == 0) {
                    labels->labels[j].focal_length = label.focal_length;
                    return;
                }
            }
            append_label_array(labels, label);
            return;
        }
    }
    if (map->size >= map->capacity) {
        map->capacity *= 2;
        map->entries = realloc(map->entries, map->capacity * sizeof(HashMapEntry));
    }
    map->entries[map->size].box_ = box_;
    init_label_array(&map->entries[map->size].label_array);
    append_label_array(&map->entries[map->size].label_array, label);
    map->size++;
}

void remove_label(HashMap *map, unsigned short box_, const char *label) {
    for (size_t i = 0; i < map->size; i++) {
        if (map->entries[i].box_ == box_) {
            LabelArray *labels = &map->entries[i].label_array;
            for (size_t j = 0; j < labels->size; j++) {
                if (strcmp(labels->labels[j].label, label) == 0) {
                    for (size_t k = j; k < labels->size - 1; k++) {
                        labels->labels[k] = labels->labels[k + 1];
                    }
                    labels->size--;
                    return;
                }
            }
        }
    }
}

Label parse_label(const char *complete_label) {
    Label label;
    label.label = malloc(strlen(complete_label) + 1);
    strcpy(label.label, complete_label);
    
    char *operand_pos = strchr(label.label, '=');
    if (operand_pos) {
        label.operand = '=';
    } else {
        operand_pos = strchr(label.label, '-');
        if (operand_pos) {
            label.operand = '-';
        }
    }
    
    if (operand_pos) {
        *operand_pos = '\0';
        label.focal_length = atoi(operand_pos + 1);
    } else {
        label.focal_length = 0;
    }
    
    label.box_ = hash_string(label.label, 0);
    return label;
}

int calc_focusing_power(const char *input) {
    char *input_copy = strdup(input);
    char *token = strtok(input_copy, ",");
    HashMap hash_map;
    init_hash_map(&hash_map);
    
    while (token) {
        Label label = parse_label(token);
        if (label.operand == '-') {
            remove_label(&hash_map, label.box_, label.label);
        } else if (label.operand == '=') {
            insert_hash_map(&hash_map, label.box_, label);
        } else {
            fprintf(stderr, "Invalid operand\n");
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, ",");
    }
    
    int focusing_power = 0;
    for (size_t i = 0; i < hash_map.size; i++) {
        unsigned short box_ = hash_map.entries[i].box_;
        LabelArray *labels = &hash_map.entries[i].label_array;
        for (size_t j = 0; j < labels->size; j++) {
            int box_value = (box_ + 1);
            int index_value = (j + 1);
            focusing_power += box_value * index_value * labels->labels[j].focal_length;
        }
    }
    
    free(input_copy);
    return focusing_power;
}

int main() {
    FILE *file = fopen("../input.txt", "r");
    if (!file) {
        fprintf(stderr, "Unable to read file\n");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *input = malloc(fsize + 1);
    fread(input, 1, fsize, file);
    fclose(file);
    input[fsize] = '\0';

    char *newline = strrchr(input, '\n');
    if (newline) {
        *newline = '\0';
    }

    clock_t start = clock();
    int response = calc_focusing_power(input);
    clock_t end = clock();
    
    printf("Focusing power: %d\n", response);
    printf("Time taken: %ldms\n", (end - start) * 1000 / CLOCKS_PER_SEC);

    free(input);
    return 0;
}
