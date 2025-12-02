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

        // Teiler finden
        char i_as_string[32];
        sprintf(i_as_string, "%llu", i);
        int i_len = strlen(i_as_string);

        int digit_is_valid = 0;

        for(int d = 1; d <= (digits)/2; d++) {
            if(digit_is_valid)break;
            if(i_len % d == 0) {
                // Überprüfen, ob es einen Teil mit der Länge d gibt, der sich über den String wiederholt

                // Zuerst - Teil extrahieren
                int part_len = d;

                char string_part[16];
                memset(string_part, '\0', 16);
                strncpy(string_part, i_as_string, part_len);

                // string_part enthält jetzt den Teil mit der Länge d
                // Überprüfen, ob sich nur noch dieser Teil wiederholt
                int is_valid = 1;
                for(int a = d; a < i_len; a+=d) {
                    if(a > i_len) break;

                    if( strncmp(string_part, &i_as_string[a], d) != 0) {
                        is_valid = 0; 
                        break;
                    }
                }

                if(is_valid) {
                    *result += i; // Zahl besteht nur aus der selben Zahl, zb. 11111
                    printf("Identische Teile %s\n", i_as_string);
                    digit_is_valid = 1;
                }

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

    // Ranges parsen

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