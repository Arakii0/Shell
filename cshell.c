#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <Windows.h>
#include <Shlobj.h>

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

        // Exit Command
        if (!strcmp(input, "exit")) {
            exit(0);
        }

        // Echo Command
        if (!strncmp(input, "echo", strlen("echo"))) {
            printf("%s\n", input + 5);
            continue;
        }

        // Type Command, checks if command is found in shell
        if (!strncmp(input, "type", strlen("type"))) {
            bool found = false;
            for (int i = 0; i < sizeof(functions) / sizeof(functions[0]); i++) {

                // Checks to see if command is in the list of functions above
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

        // Print Current Directory Command
        if (!strcmp(input, "pwd")) {
            char path[100];
            GetCurrentDirectory(100 ,path);
            printf("%s\n", path);
            continue;
        }

        // Changes the directory
        if (!strncmp(input, "cd", strlen("cd"))) {

            // Changes to user's home directory
            if (!strcmp(input + 3, "~")){
                WCHAR homeDir[100];
                if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, homeDir))) {
                    SetCurrentDirectoryW(homeDir);
                }
                else {
                    printf("cd %s: Home Directory not found\n", input + 3);
                }
                continue;
            }

            if (SetCurrentDirectory(input + 3) == 0) {
                printf("cd %s: No such file or directory\n", input + 3);
            }
            continue;
        }



        // If command is not found in shell
        printf("%s: command not found\n", input);
    }
    
    return 0;
}