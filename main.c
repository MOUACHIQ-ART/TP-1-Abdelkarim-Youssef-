#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define WELCOME_MESSAGE "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define MAX_CMD_LENGTH 100

// Function to display the welcome message
void displayWelcomeMessage() {
    write(STDOUT_FILENO, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
}

// Function to display the prompt with exit code or signal
void displayPrompt(int status, int isSignal) {
    char prompt[100];
    if (isSignal) {
        snprintf(prompt, sizeof(prompt), "enseash [sign:%d] %% ", status);
    } else {
        snprintf(prompt, sizeof(prompt), "enseash [exit:%d] %% ", status);
    }
    write(STDOUT_FILENO, prompt, strlen(prompt));
}

int main() {
    displayWelcomeMessage();

    int lastStatus = 0; // Last command's status
    int isSignal = 0;   // Flag for signal (1) or exit (0)

    while (1) {
        displayPrompt(lastStatus, isSignal);

        char command[MAX_CMD_LENGTH];
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

        pid_t pid = fork();
        if (pid < 0) {
            perror("Error during fork");
            continue;
        } else if (pid == 0) {
            // Child process: Execute the command
            char *args[2];
            args[0] = command;
            args[1] = NULL;

            execvp(args[0], args);  // Replace the child process
            perror("Error executing command");
            exit(EXIT_FAILURE);
        } else {
            // Parent process: Wait for the child to finish
            int status;
            waitpid(pid, &status, 0);

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