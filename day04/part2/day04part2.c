#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Überprüft an einer Position:
 * Gibt 1 zurück, wenn an dieser Position ein Papier ist
 * Gibt 0 zurück, wenn an dieser Position ein (.) ist oder die Position
 * außerhalb des Feldes liegt
 */
int check_pos(int line_length, int line_count, int check_y, int check_x, char grid[line_length][line_count]) {
    if(check_y >= line_count || check_y < 0) return 0;
    if(check_x >= line_length || check_x < 0) return 0;

    return grid[check_y][check_x] == '@'; // returnt 1, wenn an der Stelle ein Papier ist
}

int is_paper_accessible(int line_length, int line_count, int paper_y, int paper_x, char grid[line_length][line_count]) {
    int rolls_free = 0;
    for(int y_offset = -1; y_offset < 2; y_offset++) {
        for(int x_offset = -1; x_offset < 2; x_offset++) {
            if(y_offset == 0 && x_offset == 0) continue;
            rolls_free += check_pos(line_length, line_count, paper_y + y_offset, paper_x + x_offset, grid);
        }
    }

    return rolls_free < 4;
}

int main() {

    FILE* input_file = fopen("input.txt", "r");
    char line[255];
    
    // Gesamte Dateilänge erhalten
    fseek(input_file, 0, SEEK_END);
    int input_file_length = ftell(input_file);
    rewind(input_file);

    // Länge einer Zeile erhalten
    if (fgets(line, 255, input_file) == NULL) {
        fclose(input_file);
        printf("Fehler beim Lesen der Datei!\n");
        return 0;
    }

    rewind(input_file);
    
    int line_length = strlen(line);
    if (line[line_length - 1] == '\n') {
        line_length--;
    }
    
    int line_count = input_file_length / line_length;

    char grid[line_count][line_length];

    char current_char;

    int current_line = 0;
    int current_index = 0;

    // Einlesen
    while( (current_char = fgetc(input_file)) != EOF) {
        if(current_char == '\n') {
            current_index = 0;
            current_line++;
            continue;
        }

        grid[current_line][current_index] = current_char;
        current_index++;
    }

    int changes = 1;
    int papers_removed = 0;
    char new_accessible[line_count][line_length];

    // aktuellen Stand in das Änderungsarray übernehmen
    memcpy(new_accessible, grid, line_count * line_length);

    while(changes) {
        changes = 0;
        memcpy(grid, new_accessible, line_count * line_length);

        for(int y = 0; y < line_count; y++) {
            for(int x = 0; x < line_length; x++) {
    
                if(grid[y][x] == '@' && is_paper_accessible(line_length, line_count, y, x, grid)) {
                    papers_removed++;
                    new_accessible[y][x] = '.';
                    changes = 1;
                }
            }
        }
    }


    printf("Insgesamt entfernte Papiere: %d\n", papers_removed);
    fclose(input_file);
    return 0;
}