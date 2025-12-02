#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Range {
    uint64_t begin;
    uint64_t end;
} typedef Range;

/**
 * @return: Zahl der Stellen von a
 */
int get_number_of_digits(uint64_t a) {
    if(a == 0) return 1;

    int digits = 0;

    while(a != 0) {
        a /= 10;
        digits++;
    }
    return digits;
}

/**
 * @param result: Ergebnis-Integer, zu dem eine mögliche ungültige Zahl addiert wird
 * @param range: Das Range Objekt zu dem alle möglichen ungültigen Zahlen überprüft werden
 * Berechnet alle möglichen ungültigen Zahlen in der angegebenen Range und addiert das Ergebnis zu result
 */
void add_invalid_numbers(u_int64_t *result, Range range) {
    for(uint64_t i = range.begin; i <= range.end; i++) {
        int digits = get_number_of_digits(i);
        if(digits % 2 == 0) {
            // Zahl ist gerade. Bedingung ist erfüllt, wenn die Zahl in der Mitte teilbar ist und zwei
            // gleiche Zahlen dabei entstehen, zum Beispiel 123123 => 123 & 123

            char i_as_string[32];
            sprintf(i_as_string, "%llu", i); // sprintf schreibt \0

            int i_len = strlen(i_as_string);
            if(i_len % 2 == 1) {
                printf("FEHLER: Zahl hat keine gerade Anzahl an Stellen!");
                break;
            }

            int part_len = i_len/2;

            char string_half[16];
            memset(string_half, '\0', 16);
            strncpy(string_half, i_as_string, part_len);

            if(strcmp(string_half, &i_as_string[part_len]) == 0) {
                // vorderer Teil und hinterer Teil sind identisch
                *result += i;
            } 
        }
    }
}

int main() {
    u_int64_t result = 0;
    FILE* input_file = fopen("input.txt", "r");
    
    char input[1024];
    
    if( fgets(input, 1024, input_file) == NULL ) {
        printf("Error beim Lesen der Datei\n");
        fclose(input_file);
    }
    
    int input_len = strlen(input);

    // Anzahl der benötigten Ranges bestimmen

    int range_amount = 0;
    for(int i = 0; i < input_len; i++) {
        if(input[i] == '-') range_amount++;
    }

    // Ranges definieren


    char* range_token, *range_context;
    range_token = strtok_r(input, ",", &range_context);

    while( range_token != NULL ) {

        // --- Range parsen ---
        char* range_begin, *range_end, *range_item_context;
        range_begin = strtok_r(range_token, "-", &range_item_context);
        range_end = strtok_r(0, "-", &range_item_context);
        
        // Neue Range mit den geparsten Rändern einfügen, strtoull für 64 bit Integer
        Range range;
        range.begin = strtoull(range_begin, NULL, 0);
        range.end = strtoull(range_end, NULL, 0);

        add_invalid_numbers(&result, range); // In Frage kommende Zahlen zu Result addieren

        range_token = strtok_r(0, ",", &range_context); //weitere Tokens bekommen
    }

    printf("Ergebnis: %llu\n", result);

    fclose(input_file);
    return 0;
}