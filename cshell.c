#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <Windows.h>
#include <Shlobj.h>


void home_directory(char input[]);
void get_window_ver();
void get_system_info();
void Read_file(char file[]);
void List_files();

int main(){

    char *functions[] = {"exit", "echo", "type", "cd", "pwd"};

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
                home_directory(input);
                continue;
            }

            if (SetCurrentDirectory(input + 3) == 0) {
                printf("cd %s: No such file or directory\n", input + 3);
            }
            continue;
        }

        // List the computer's information
        if (!strcmp(input, "sysinfo")) {
            get_window_ver();
            get_system_info();
            continue;
        }

        // Clearing terminal
        if (!strcmp(input, "clear")) {
            system("cls");
            continue;
        }

        // list contents of file
        if (!strncmp(input, "cat", strlen("cat"))) {
            Read_file(input + 4);
            continue;
        }

        if (!strcmp(input, "ls")) {
            char path[100];
            GetCurrentDirectory(100 ,path);
            List_files(path);
            continue;
        }

        // If command is not found in shell
        printf("%s: command not found\n", input);
    }
    
    return 0;
}





void home_directory(char input[]) {
     WCHAR homeDir[100];
        if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, homeDir))) {
            SetCurrentDirectoryW(homeDir);
        }
        else {
            printf("cd %s: Home Directory not found\n", input + 3);
        }
}


void get_window_ver() {
    DWORD dwVersion = 0; 
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0; 
    DWORD dwBuild = 0;

    dwVersion = GetVersion();
 
    // Get the Windows version.
    dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

    // Get the build number.
    if (dwVersion < 0x80000000)              
        dwBuild = (DWORD)(HIWORD(dwVersion));

    printf("Version is %d.%d (%d)\n", 
                dwMajorVersion,
                dwMinorVersion,
                dwBuild);
}


void get_system_info() {
    SYSTEM_INFO systeminformation;
    // Get the system information
    GetNativeSystemInfo(&systeminformation);

    printf("Processor Architecture: ");
    switch (systeminformation.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            printf("x64 (AMD or Intel)\n");
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            printf("ARM\n");
            break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            printf("ARM64\n");
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            printf("x86\n");
            break;
        case PROCESSOR_ARCHITECTURE_UNKNOWN:
        default:
            printf("Unknown architecture\n");
            break;
    }

    printf("Number of processors: %u\n", systeminformation.dwNumberOfProcessors);
}


void Read_file(char file[]) {
    HANDLE hFile = CreateFile(
        file,           // File name
        GENERIC_READ,            // Open for reading
        0,                       // Do not share
        NULL,                    // Default security
        OPEN_EXISTING,           // Open existing file only
        FILE_ATTRIBUTE_NORMAL,   // Normal file
        NULL                     // No template file
    );
    
    // If file is not found
    if (hFile == INVALID_HANDLE_VALUE) 
    { 
        printf("%s not found.\n", file);
    }
    else {
        DWORD bytesRead;
        char buffer[1000000];  // Buffer to store the read data
        BOOL success = ReadFile(
            hFile,            // Handle to the file
            buffer,           // Buffer to store data
            sizeof(buffer) - 1, // Number of bytes to read (leaving space for null terminator)
            &bytesRead,       // Number of bytes that were read
            NULL              // No overlapping structure (for asynchronous I/O)
        );

        if (!success) {
            printf("Could not read file (error %lu)\n", GetLastError());
            CloseHandle(hFile);
        }
        else {
            // Process the data
            buffer[bytesRead] = '\0';  // Null-terminate the buffer
            printf("%s\n", buffer);

            // Close the file handle
            CloseHandle(hFile);
        }
    }
}


void List_files(char directory_path[]) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // Specify the directory and file type (e.g., *.* for all files)
    char all_files[] = "\\*.*";
    strcat(directory_path, all_files);

    // Step 1: Find the first file in the directory
    hFind = FindFirstFile(directory_path, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (error %lu)\n", GetLastError());
    } 

    // Step 2: List all the files in the directory
    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            printf("[DIR]  %s\n", findFileData.cFileName); // It's a directory
        } else {
            printf("[FILE] %s\n", findFileData.cFileName); // It's a file
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    // Step 3: Handle the end of the list or errors
    if (GetLastError() != ERROR_NO_MORE_FILES) {
        printf("FindNextFile failed (error %lu)\n", GetLastError());
    }

    // Close the search handle
    FindClose(hFind);
}