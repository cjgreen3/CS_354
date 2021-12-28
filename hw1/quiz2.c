#include <stdio.h>
int main(){
    int * a = 0x0;
    char * b = 0x0;
    int ** c = 0x0;

    a += 1;
    b += 1;
    c += 1;

    printf("%d,%d,%d\n", a,b,c);
}