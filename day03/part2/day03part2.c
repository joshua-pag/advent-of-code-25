#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

uint64_t powi(u_int64_t base, uint64_t exp) {
    uint64_t result = 1;

    while(exp) {
        exp--;

        result = result * base;
    }

    return result;
}

uint64_t calculate_maximum_joltage(const char* bank_input) {


    /**
     * New Line Compensation ist wichtig -> fgets liest den Input, der möglicherweise an der letzten Stelle 
     * eine Newline hat. Wenn ja, ist strlen gegebenenfalls anders und das Programm hat möglicherweise Probleme,
     * richtig zu iterieren, da strlen verwendet wird.
     */
    int new_line_compensation = 0;
    if(bank_input[strlen(bank_input) - 1] == '\n') {
        new_line_compensation = 1;
    }


    int max[12] = {0};
    int index[12] = {-1};

    for(int digit = 0; digit < 12; digit++) {
        int starting_index = digit <= 0 ? 0 : (index[digit - 1] + 1);
        for(int str_i = starting_index; str_i < ((strlen(bank_input) - 11) + digit - new_line_compensation); str_i++) {

            // Wenn der Index (Stelle in der Bank) schon von einer vorherigen Zahl "verbraucht wurde" kann
            // diese Zahl diesen Index nicht mehr wählen.
            int bedgin = str_i < (strlen(bank_input) - 1) - (11 - digit);

            int b = 0;
            for(int a = 0; a < 12; a++) {
                if(index[a] == str_i && digit != 0) { // Index wurde schon verbraucht
                    b = 1;
                    break;
                }
            }
            if(b == 1) continue;

            int joltage = bank_input[str_i] - '0';
            if(joltage > max[digit]) {
                index[digit] = str_i;
                max[digit] = joltage;
            }
        }
    }

    for(int i = 0; i < 12; i++) {
        for(int o = 0; o < 12; o++) {
            if(i != o && index[i] == index[o]) {
                printf("ERROR: index %d und %d sind gleich!\n", index[i], index[o]);
            }
        }
    }

    uint64_t result = 0;

    for(int digit = 0; digit < 12; digit++) {
        int num_zeroes = (12 - digit) - 1;
        uint64_t stelle = powi(10, num_zeroes);
        uint64_t m = max[digit];
        result += stelle * m;
    }

    return result;
}

int main() {

    uint64_t result = 0;

    FILE* input_file = fopen("input.txt", "r");
    
    char line[255];
    
    while( (fgets(line, 255, input_file)) != NULL) {
        uint64_t max_joltage = calculate_maximum_joltage(line);
        result += max_joltage;
        printf("Maximale Joltage der Bank: %llu\n", max_joltage);
    }

    fclose(input_file);

    printf("Maximale Joltage gesamt: %llu\n", result);

    return 0;
}