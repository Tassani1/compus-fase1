#include <stdio.h>

/*
gcc -Wall -Wextra -o bin/compara_strings compara_strings.c
./bin/compara_strings
*/

// Definició dels valors vàlids de PIN (de longitud variable)
const char *valid_pins[] = {"1234", "5678", "0000", "9999", "12", "12345"};
const int num_pins = 6;
//const int num_pins = sizeof(valid_pins) / sizeof(valid_pins[0]);

// Funció per comparar dues cadenes de caràcters de qualsevol longitud
int string_equals(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

// Funció que comprova si el PIN és vàlid
int esPINValid(const char *pin) {
    for (int i = 0; i < num_pins; i++) {
        if (string_equals(pin, valid_pins[i])) {
            return 1;
        }
    }
    return 0;
}

int main() {
    // Proves amb cadenes de longitud variable
    const char *proves[] = {"1234", "12", "12345", "0000", "9999", "5678", "1", "123", ""};
    int num_proves = 9;
    for (int i = 0; i < num_proves; i++) {
        printf("PIN: '%s' -> %s\n", proves[i], esPINValid(proves[i]) ? "Vàlid" : "Invàlid");
    }
    return 0;
}
