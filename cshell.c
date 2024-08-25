#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int main(){

    char *functions[] = {"exit", "echo", "type"};

    while(true){
        // Print prompt
        printf("$ ");
        fflush(stdout);

        // Wait for user input
        char input[100];
        fgets(input, 100, stdin);

        // Remove newline character
        input[strlen(input) - 1] = '\0'; 

        // Exit Function
        if (!strcmp(input, "exit")) {
            exit(0);
        }

        // Echo Function
        if (!strncmp(input, "echo", strlen("echo"))) {
            printf("%s\n", input + 5);
            continue;
        }

    
        // Type function, checks if command is found in shell
        if (!strncmp(input, "type", strlen("type"))) {
            bool found = false;
            for (int i = 0; i < sizeof(functions) / sizeof(functions[0]); i++) {
                if (!strcmp(input + 5, functions[i])) {
                    printf("%s is a shell builtin\n", input +5);
                    found = true;
                    break;
                }
            }
            if (!found)
                printf("%s: not found\n", input + 5);
            continue;
        }

        // If command is not found in shell
        printf("%s: command not found\n", input);
    }
    
    return 0;
}