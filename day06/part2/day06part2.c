#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LIMIT 4096

int main() {
    FILE* input_file = fopen("input.txt", "r");
    
    

    int amount_lines = 0;
    
    char c;
    while((c = fgetc(input_file)) != EOF) {
        if(c == '\n')amount_lines++;
    }

    amount_lines++; //In der letzten Zeile ist kein Zeilenumbruch


    rewind(input_file);

    int line_length;

    char input[LINE_LIMIT];
    memset(input, 0, LINE_LIMIT);
    fgets(input, LINE_LIMIT, input_file);
    
    line_length = strlen(input) + 1; // +1 notwendig

    rewind(input_file);

    
    char** lines;
    lines = malloc(amount_lines * sizeof(char*));

    for(int i = 0; i < amount_lines; i++) {
        lines[i] = malloc((line_length + 1) * sizeof(char));
        fgets(lines[i], line_length, input_file);
    }

    uint64_t result = 0;

    // von rechts anfangen

    uint64_t number[6];
    memset(number, 0, 6 * sizeof(uint64_t));

    int problem_end_encountered = 0;
    int op = 0;
    int number_counter = 0;

    for(int column = (line_length - 2); column >= 0; column--) {
        //printf("Ab hier wird Spalte %d geprüft\n    ", column);
        char current_number_string[10] = "";
        for(int line = 0; line < amount_lines; line++) {
            char c = lines[line][column];

            if(c == '+') {
                problem_end_encountered = 1;
                op = 1;
            } else if(c == '*') {
                problem_end_encountered = 1;
                op = 2;
            } else {
                char tmp[2] = {c, '\0'};
                strcat(current_number_string, tmp);
            }
            //printf("'%c'", c);
        }

        // aktuelles Zeichen parsen
        if(0 == 0) {
            uint64_t num = strtoull(current_number_string, NULL, 0);
            number[number_counter] = num;
            number_counter++;
            //printf("Zahl %llu wurde eingelesen\n", num);
        }
        if(problem_end_encountered == 1) {
            if(op == 0) {
                printf("ERROR\n");
                break;
            }


            problem_end_encountered = 0;
            if(op == 1) {
                for(int i = 0; i < 6; i++) {
                    if(number[i] == 0) continue;  
                    result += number[i];
                }
            } else if(op == 2) {
                uint64_t problem_result = 1;
                for(int i = 0; i < 6; i++) {
                    if(number[i] == 0) continue;  
                    problem_result *= number[i];
                }
                result += problem_result;
            }


            memset(number, 0, 6 * sizeof(uint64_t));
            op = 0;
            number_counter = 0;
        } 
    }



    fclose(input_file);

    printf("Anzahl Zeilen %d Zeilenlänge %d\n", amount_lines, line_length);

    printf("Ergebnis: %llu\n", result);

    for(int i = 0; i < amount_lines; i++) {
        free(lines[i]);
    }

    free(lines);

    return 0;
}