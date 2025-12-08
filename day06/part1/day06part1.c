#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Operation {
    ADDITION,
    MULTIPLICATION,
    UNDEFINED
} typedef Operation;

struct Problem {

    uint64_t* numbers;
    int number_amount;
    uint64_t result;
    Operation operation;

} typedef Problem;

int main() {
     u_int64_t result = 0;
    FILE* input_file = fopen("input.txt", "r");
    
    char input[2048];
    memset(input, 0, 2048);

    int amount_lines = 0;
    
    char c;
    while((c = fgetc(input_file)) != EOF) {
        if(c == '\n')amount_lines++;
    }

    amount_lines++; //In der letzten Zeile ist kein Zeilenumbruch


    rewind(input_file);

    int amount_problems = 0;
    int in_word = 0;

    while((c = fgetc(input_file)) != EOF) {
        if(c != ' ' && c != '\n') {
            if(!in_word) {
                in_word = 1;
                amount_problems++;
            }
        } else {
            in_word = 0;
        }
        if(c == '\n')break;
    }

    rewind(input_file);

    Problem problems[amount_problems];

    for(int i = 0; i < amount_problems; i++) {
        Problem problem;
        problem.number_amount = amount_lines - 1;
        problem.numbers = malloc((amount_lines - 1) * sizeof(uint64_t));
        memset(problem.numbers, 0, (amount_lines - 1) * sizeof(uint64_t));
        problem.operation = UNDEFINED;
        problem.result = 0;
        problems[i] = problem;
    }


    // Jetzt jede Zeile parsen
    for(int i = 0; i < (amount_lines - 1); i++) {
        // Zeile lesen
        char line[8192];

        fgets(line, 8191, input_file);

        int problem = 0;

        char* number = strtok(line, " ");
        while(number != NULL && strcmp(number, "\n")) { 
            problems[problem].numbers[i] = strtoull(number, NULL, 0);
            number = strtok(NULL, " ");
            problem++;
        }
    }

    char line[8192];

    fgets(line, 8191, input_file);
    int problem = 0;

    char* operation = strtok(line, " ");
    while(operation != NULL && strcmp(operation, "\n")) { 
        problems[problem].operation = (strcmp(operation, "*") == 0) ? MULTIPLICATION : (strcmp(operation, "+") == 0) ? ADDITION : UNDEFINED;
        operation = strtok(NULL, " ");
        problem++;
    }

    uint64_t total_result = 0;

    for(int i = 0; i < amount_problems; i++) {

        if(problems[i].operation == ADDITION) {
            problems[i].result = 0;
            for(int a = 0; a < problems[i].number_amount; a++) {
                problems[i].result += problems[i].numbers[a];
            }
            total_result += problems[i].result;
        } else if(problems[i].operation == MULTIPLICATION) {
            problems[i].result = 1;
            for(int a = 0; a < problems[i].number_amount; a++) {
                problems[i].result *= problems[i].numbers[a];
            }
            total_result += problems[i].result;
        } else if(problems[i].operation == UNDEFINED) {
            printf("Error: Operation von Problem %d ist undefinert.\n", i);
        }

    }



    printf("Anzahl Zeilen %d\n", amount_lines);
    printf("Anzahl Probleme %d \n", amount_problems);
    printf("Total Result: %llu\n", total_result);

    fclose(input_file);

    for(int i = 0; i < amount_problems; i++) {
        Problem problem = problems[i];
        free(problems[i].numbers);
    }
    return 0;
}