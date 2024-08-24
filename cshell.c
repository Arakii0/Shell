#include <stdio.h>
#include <stdbool.h>

int main(){

    while(true){
        printf("$ ");
        fflush(stdout);

        // Wait for user input
        char input[100];
        fgets(input, 100, stdin);
    }
    
    return 0;
}