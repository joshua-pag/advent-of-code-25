#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int calculate_maximum_joltage(const char* bank_input) {
    int first_index, second_index = -1;
    int first_max, second_max = 0;

    for(int str_i = 0; str_i < strlen(bank_input) - 2; str_i++) {
        int joltage = bank_input[str_i] - '0';
        if(joltage > first_max) {
            first_index = str_i;
            first_max = joltage;
        }
    }

    for(int str_i = first_index; str_i < strlen(bank_input) - 1; str_i++) {
        int joltage = bank_input[str_i] - '0';
        if(joltage > second_max && first_index != str_i) {
            second_index = str_i;
            second_max = joltage;
        }
    }

    if(first_index == second_index) {
        printf("ERROR: first_index und second_index sind gleich!\n");
        return -1;
    }

    return first_max * 10 + second_max;
}

int main() {

    int result = 0;

    FILE* input_file = fopen("input.txt", "r");
    
    char line[255];
    
    while( (fgets(line, 255, input_file)) != NULL) {
        int max_joltage = calculate_maximum_joltage(line);
        result += max_joltage;
        printf("Maximale Joltage der Bank: %d\n", max_joltage);
    }

    fclose(input_file);

    printf("Maximale Joltage gesamt: %d\n", result);

    return 0;
}