#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int main(){

    while(true){
        printf("$ ");
        fflush(stdout);

        // Wait for user input
        char input[100];
        fgets(input, 100, stdin);

        input[strlen(input) - 1] = '\0'; // Remove newline character

        if(!strcmp(input, "exit")) {
            exit(0);
        }

        printf("%s: command not found\n", input);
    }
    
    return 0;
}