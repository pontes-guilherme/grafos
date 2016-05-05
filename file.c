/**
 * @author: Leonardo de Assis / Guilherme Pontes
 * @date: 04/05/2016
 *
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "GrafosOriginais/1.Airlines.csv"

int main(){
    FILE* stream = fopen(FILENAME, "r");
    char line[1024];
    int a, b;

    while(fgets(line, 1024, stream)){
        a = b = -1;

        sscanf(line, "%d,%d", &a, &b);
        printf("%d, %d\n", a, b);

    }
    
    fclose(stream);

    return 0;
}