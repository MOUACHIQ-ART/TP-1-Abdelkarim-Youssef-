#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define WELCOME_MESSAGE "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define MAX_CMD_LENGTH 100

// Function to display the welcome message
void displayWelcomeMessage() {
    write(STDOUT_FILENO, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
}

// Function to display the prompt with exit code or signal
void displayPrompt(int status, int isSignal, long execTime) {
    char prompt[100];
    if (isSignal) {
        snprintf(prompt, sizeof(prompt),  "enseash [sign:%d|%ldms] %% ", status, execTime);
    } else {
        snprintf(prompt, sizeof(prompt),  "enseash [exit:%d|%ldms] %% ", status, execTime);
    }
    write(STDOUT_FILENO, prompt, strlen(prompt));
}

// Function to split input into command and arguments
void parseCommand(char *input, char **args) {
    char *token;
    int index = 0;

    // Token the input string by spaces
    token = strtok(input, " ");
    while (token != NULL) {
        args[index++] = token;
        token = strtok(NULL, " ");
    }

    args[index] = NULL; // NULL-terminate the array
}

int main() {
    displayWelcomeMessage();

    int lastStatus = 0; // Last command's status
    int isSignal = 0;   // Flag for signal (1) or exit (0)
    long execTime = 0;     // Execution time in milliseconds

    while (1) {
        displayPrompt(lastStatus, isSignal, execTime );

        char command[MAX_CMD_LENGTH];
        char *args[MAX_CMD_LENGTH / 2 + 1]; // Array to hold command and arguments
        if (read(STDIN_FILENO, command, sizeof(command)) <= 0) {
            write(STDOUT_FILENO, "\n", 1);  // Handle Ctrl+D (EOF)
            break;
        }

        command[strcspn(command, "\n")] = '\0';  // Remove newline character

        if (strcmp(command, "exit") == 0) {
            write(STDOUT_FILENO, "Bye bye...\n", strlen("Bye bye...\n"));
            break;
        }

        if (strlen(command) == 0) {
            continue;  // Ignore empty input
        }

        // Parse the command into args array
        parseCommand(command, args);

        // Measure time
        struct timespec startTime, endTime;
        clock_gettime(CLOCK_MONOTONIC, &startTime); // Start time

        pid_t pid = fork();
        if (pid < 0) {
            perror("Error during fork");
            continue;
        } else if (pid == 0) {
            // Child process: Execute the command
            execvp(args[0], args);  // Execute command with arguments
            perror("Error executing command"); // Only reached if execvp fails
            exit(EXIT_FAILURE);
        } else {
            // Parent process: Wait for the child to finish
            int status;
            waitpid(pid, &status, 0);
            

            clock_gettime(CLOCK_MONOTONIC, &endTime); // End time

            // Calculate execution time in milliseconds
            execTime = (endTime.tv_sec - startTime.tv_sec) * 1000;
            execTime += (endTime.tv_nsec - startTime.tv_nsec) / 1000000;

            if (WIFEXITED(status)) {
                lastStatus = WEXITSTATUS(status); // Exit code
                isSignal = 0;
            } else if (WIFSIGNALED(status)) {
                lastStatus = WTERMSIG(status); // Signal number
                isSignal = 1;
            }
        }
    }

    return 0;
} 
