#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Range {
    u_int64_t begin;
    u_int64_t end;
    int disabled;
} typedef Range;

int main() {
    u_int64_t result = 0;
    FILE* input_file = fopen("input.txt", "r");
    
    char input[1024];
    memset(input, 0, 1024);

    int max_amount_ranges = 0;
    // initiale Befüllung
    if(fgets(input, 1023, input_file) == NULL) {
        fclose(input_file);
        return 0;
    }

    while(input[0] != '\n') { // bis zur Leerzeile überprüfen
        if(fgets(input, 1023, input_file) == NULL) {
            break;
        }
        max_amount_ranges++;
    }

    Range ranges[max_amount_ranges];

    int actual_amount_ranges = 0;

    // Ranges aus der Datei einlesen
    rewind(input_file);
    for(int i = 0; i < max_amount_ranges; i++) {
        char range_line[256];
        if(fgets(range_line, 255, input_file) == NULL) {
            fclose(input_file);
            printf("Error während dem Lesen von Ranges aus der Inputdatei.\n");
            return 0;
        }

        char* range_begin;
        char* range_end;

        range_begin = strtok(range_line, "-");
        range_end = strtok(NULL, "-");

        Range new_range = {0};
        new_range.begin = strtoull(range_begin, NULL, 0);
        new_range.end = strtoull(range_end, NULL, 0);
        new_range.disabled = 0;

        ranges[actual_amount_ranges] = new_range;
        actual_amount_ranges++;
    }


    int change_happened = 1;

    // Algorithmus zum Minimieren von Ranges. Wenn Ranges in anderen Ranges komplett oder teilweise
    // enthalten sind, werden Ranges verändert und deaktiviert. Somit kann die Anzahl der Ranges minimiert werden

    while(change_happened) {
        change_happened = 0;

        // Jede Range mit jeder vergleichen

        for(int i = 0; i < actual_amount_ranges; i++) {
            if(ranges[i].disabled) continue;
            int insert_range = 1;

            Range range_check = ranges[i];

            for(int a = 0; a < actual_amount_ranges; a++) {
                if(a == i) continue;
                if(ranges[a].disabled) continue;

                Range new_range = ranges[a];

                //       |--------|                   NEU
                //    |-------------------|           ALT
                //    |-------------------|           ERGEBNIS
                // Die neue Range ist komplett enthalten -  kann ignoriert werden
                if(new_range.begin >= range_check.begin && new_range.end <= range_check.end) {
                    ranges[a].disabled = 1;
                    change_happened = 1;
                    break;
                }

                //    |-------------------|           ALT
                //       |--------|                   NEU
                //    |-------------------|           ERGEBNIS
                // Die neue Range ist komplett enthalten -  kann ignoriert werden
                // alte Range ist komplett in der neuen Range enthalten
                if(range_check.begin > new_range.begin && range_check.end < new_range.end) {
                    ranges[a].begin = new_range.begin;
                    ranges[a].end = new_range.end;
                    ranges[i].disabled = 1;
                    change_happened = 1;
                    break;
                }

                //       |----------------|           NEU
                //    |---------------|               ALT
                //    |-------------------|           ERGEBNIS
                // Der Beginn der neuen Range ist in der alten Range enthalten, das Ende der neuen Range ist aber Länger
                if(new_range.begin >= range_check.begin && new_range.begin <= range_check.end && new_range.end > range_check.end) {
                    ranges[i].end = new_range.end; // Ende der bereits enthaltenen Range anpassen
                    ranges[a].disabled = 1;
                    change_happened = 1;
                    break;
                }

                // |----------------|           NEU
                //    |---------------|         ALT
                // |------------------|         ERGEBNIS
                // Anfang der neuen Range liegt vor der alten, Ende liegt aber in der alten
                // Anfang der alten Range wird angepasst an den neuen Umfang
                if(new_range.begin < range_check.begin && new_range.end >= range_check.begin && new_range.end <= range_check.end) {
                    ranges[i].begin = new_range.begin;
                    ranges[a].disabled = 1;
                    change_happened = 1;
                    break;
                }

            }
           
        }
    }

    uint64_t amount_fresh_ingredients = 0;

    for(int i = 0; i < actual_amount_ranges; i++) {
        Range r = ranges[i];
        if(r.disabled) continue;

        amount_fresh_ingredients += (r.end - r.begin) + 1;
    }

    printf("Anzahl frischer Zutaten: %llu\n", amount_fresh_ingredients);

    fclose(input_file);
    return 0;
}