#include <stdio.h>
#include <stdlib.h>


int main() {
    int dial = 50;
    int result = 0;
    FILE* input = fopen("input.txt", "r");
    
    char line[255];
    
    while( (fgets(line, 255, input)) != NULL) {
      char dir = line[0]; 
      int amount = atoi(&line[1]);

      for(int i = 0; i < amount; i++) {
        dial = dir == 'R' ? (dial + 1) : (dial - 1);

        dial = dial % 100;

        if(dial == 0) result++;
      }

      printf("Richtung %c mit %i\n", dir, amount);
    }

    printf("Ergebnis: %i\n", result);

    fclose(input);

    return 0;
}
