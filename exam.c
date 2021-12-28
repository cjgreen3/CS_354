#include <stdio.h>
#include <stdlib.h>
char * Binary(int n) {
    char * binary = malloc(n * sizeof(char));
    int i = 0;
    for ( ; n > 0; i++) {
        binary[i] = '0' + n%2;
        n /= 2;
    }
    binary[i] = '\0';
    return binary;
}

int main() {
    printf("%s\n", Binary(100));
    return 0; 
}