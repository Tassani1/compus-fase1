#include <stdio.h>

#define GREETING "Hola"


/*
gcc -Wall -Wextra -o bin/experiment experiment.c
./bin/experiment
*/
int main(void) {
    char text[] = "Hols";
    int equal = 1;
    unsigned int i;

    for (i = 0; text[i] != '\0' || GREETING[i] != '\0'; i++) {
        if (text[i] != GREETING[i]) {
            equal = 0;
            break;
        }
    }

    if (equal) {
        printf("Coincideixen.\n");
    } else {
        printf("No coincideixen.\n");
    }

    printf("text =     %s\n", text);
    printf("GREETING = %s\n", GREETING);

    return 0;
}
