#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Range {
    u_int64_t begin;
    u_int64_t end;
} typedef Range;


int is_ingredient_fresh(Range* ranges, int amount_ranges, uint64_t ingredient) {
    for(int i = 0; i < amount_ranges; i++) {
        if( ingredient >= ranges[i].begin && ingredient <= ranges[i].end) {
            return 1;
        }
    }

    return 0;
}

int main() {
    u_int64_t result = 0;
    FILE* input_file = fopen("input.txt", "r");
    
    char input[1024];
    memset(input, 0, 1024);

    int amount_ranges = 0;
    // initiale Befüllung
    if(fgets(input, 1023, input_file) == NULL) {
        fclose(input_file);
        return 0;
    }
    while(input[0] != '\n') { //blank space
        if(fgets(input, 1023, input_file) == NULL) {
            break;
        }
        amount_ranges++;
    }

    Range ranges[amount_ranges];


    // Ranges aus der Datei einlesen
    rewind(input_file);
    for(int i = 0; i < amount_ranges; i++) {
        char range_line[256];
        if(fgets(range_line, 255, input_file) == NULL) {
            fclose(input_file);
            printf("Error während dem Lesen von Ranges aus der Inputdatei.\n");
            return 0;
        }

        // Aufbau Range: abc-xyz
        char* range_begin;
        char* range_end;

        range_begin = strtok(range_line, "-");
        range_end = strtok(NULL, "-");

        Range range = {0};
        // strtoull für 64bit Integer
        range.begin = strtoull(range_begin, NULL, 0);
        range.end = strtoull(range_end, NULL, 0);

        ranges[i] = range;
    }

    // Filepointer ist jetzt in der Leerzeile

    int file_delimiter_position = ftell(input_file);

    int amount_ingredients = 0;
    while( (fgets(input, 1023, input_file)) != NULL) {
        if(input[0] == '\n')continue; //sichergehen, dass die Leerzeile übersprungen wird
        amount_ingredients++;
    }


    uint64_t ingredients[amount_ingredients];

    fseek(input_file, file_delimiter_position, SEEK_SET); // wieder an die gespeicherte Position zurück

    for(int i = 0; i < amount_ingredients; i++) {
        if(fgets(input, 1023, input_file) == NULL) {
            fclose(input_file);
            printf("Error während dem Lesen von Zutaten aus der Inputdatei.\n");
            return 0;
        }

        if(input[0] == '\n') {
            i--; // Augleich der leeren Zeile
            continue;
        }

        ingredients[i] = strtoull(input, NULL, 0);
    }


    printf("Anzahl Zutaten gesamt: %d\n", amount_ingredients);

    uint64_t amount_of_fresh_ingredients = 0;

    for(int i = 0; i < amount_ingredients; i++) {
        if (is_ingredient_fresh(ranges, amount_ranges, ingredients[i])) {
            amount_of_fresh_ingredients++;
        }
    }


    printf("Anzahl an nicht abgelaufenen Zutaten: %llu\n", amount_of_fresh_ingredients);

    fclose(input_file);
    return 0;
}