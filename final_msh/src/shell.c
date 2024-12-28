#include "shell.h"

// shell.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wordexp.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>

const int MAX_JOBS = 16;
const int MAX_LINE = 1024;
const int MAX_HISTORY = 10;

msh_t *alloc_shell(int max_jobs, int max_line, int max_history) {
    // Allocate memory for msh_t structure
    msh_t *shell = (msh_t *)malloc(sizeof(msh_t));

    if (!shell) {
        fprintf(stderr, "Error: Unable to allocate memory for shell\n");
        exit(EXIT_FAILURE);
    }

    // Shell state initialization
    shell->max_jobs = (max_jobs == 0) ? MAX_JOBS : max_jobs;
    shell->max_line = (max_line == 0) ? MAX_LINE : max_line;
    shell->max_history = (max_history == 0) ? MAX_HISTORY : max_history;

    // Initialize jobs

    // job_t *jobs = (job_t*)(malloc(sizeof(job_t) * max_jobs));
    shell->jobs = (job_t *)malloc(max_jobs * sizeof(job_t));

    if (!shell->jobs) {
        fprintf(stderr, "Error: Unable to allocate memory for jobs\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < max_jobs; i++) {
        shell->jobs[i].cmd_line = NULL;
        shell->jobs[i].state = UNDEFINED;
        shell->jobs[i].pid = -1;
        shell->jobs[i].jid = -1;
    }


    return shell;
}

char *parse_tok(char *line, int *job_type) {
    
    static char *current_line = NULL;
    static char *original_token = NULL;

    // NULL when first declared
    if (original_token == NULL) {
        original_token = NULL;
    }

    if (line != NULL) {
        // Updatewith new line
        current_line = line;
        // Free the memory allocated for the prev token
        free(original_token);
        original_token = NULL;
    }

    if (current_line == NULL || *current_line == '\0') {
        *job_type = -1;
        return NULL;
    }

    // Find the next token separated by & or ;
    char *token = current_line;

    while (*current_line != '\0' && *current_line != '&' && *current_line != ';' ) {
        current_line++;
    }

    // Copy the original token without removing leading and trailing whitespaces
    size_t token_length = current_line - token;
    original_token = malloc(token_length + 1);
    if (original_token != NULL) {
        strncpy(original_token, token, token_length);
        original_token[token_length] = '\0';
    } else {
        // Memory allocation fail
        return NULL;
    }

    if (*current_line == '&') {
        // If '&' is found in the token, it's a background job
        *job_type = 0;  // Background job
        // Move the current_line pointer to the next position
        current_line++;
    } else {
        *job_type = 1;  // Foreground job
        if (*current_line == ';') {
            // Move the current_line pointer to the next position
            current_line++;
        } else if (*current_line != '\0') {
            // If the token is not at the end of the line, move the pointer to the next position
            current_line++;
        }
    }

    return original_token;
}


char **separate_args(char *line, int *argc, bool *is_builtin) {
    // Check if the command line is empty
    if (line == NULL || *line == '\0') {
        *argc = 0;
        return NULL;
    }

    // Check if ~ is present in the line
    bool tildePresent = false;
    for (size_t i = 0; i < strlen(line); i++) {
        if (line[i] == '~') {
            tildePresent = true;
            break;
        }
    }

    // Perform tilde expansion (referenced internet for some of this logic)
    wordexp_t p;
    if (tildePresent) {
        wordexp(line, &p, WRDE_NOCMD);
        char *home = getenv("HOME");
        if (home != NULL) {
            for (size_t i = 0; i < p.we_wordc; i++) {
                if (strncmp(p.we_wordv[i], home, strlen(home)) == 0) {
                    // Replace home directory path with ~
                    char *tildePath = (char *)malloc(strlen(p.we_wordv[i]) + 1);
                    strcpy(tildePath, "~");
                    strcat(tildePath, p.we_wordv[i] + strlen(home));
                    free(p.we_wordv[i]);
                    p.we_wordv[i] = tildePath;
                }
            }
        }
    } else {
        wordexp(line, &p, 0);
    }

    // Tokenize the command line
    int count = p.we_wordc;

    // Allocate memory for the array of strings
    char **argv = (char **)malloc((count + 1) * sizeof(char *));

    for (int i = 0; i < count; i++) {
        // Allocate memory for each argument
        argv[i] = (char *)malloc((strlen(p.we_wordv[i]) + 1) * sizeof(char));
        strcpy(argv[i], p.we_wordv[i]);
    }

    argv[count] = NULL;
    *argc = count;

    // Free memory
    wordfree(&p);

    return argv;
}


int evaluate(msh_t *shell, char *line) {

    // Check if line longer than max_line
    if (strlen(line) > shell->max_line) { 
        printf("error: reached the maximum line limit\n");
        return 1;
    }

    int type;
    char *token;
    token = parse_tok(line,&type);

    while (token != NULL) {
        int child_state;

        // Tokenize further based on space to get command and arguments
        char *command = strtok(token, " ");
        char *argv[shell->max_line];  
        int argc = 0;

        while (command != NULL) {
            argv[argc++] = command;
            command = strtok(NULL, " ");
        }

        // Null-terminate the argument array
        argv[argc] = NULL; 
        
        // Create a new child process to handle the execution of the current job.
        pid_t child_pid = fork();

        if (child_pid == 0) {            
            // Child process executes the command
            if (execve(argv[0], argv, NULL) == -1) {
 	            exit(0);
            }
        } 

        // Parent process (waits for the child to complete)
        add_job(shell->jobs, shell->max_jobs, child_pid, SUSPENDED, token);

        if (waitpid(child_pid, &child_state, 0) == -1) {
            perror("waitpid");
            return 1;
        } 
        
        delete_job(shell->jobs, child_pid);

        // TODO: background checks

        // Move to the next command
        token = parse_tok(NULL, &type);
    }

    return 0;
}

// Deallocate memory for the shell
void exit_shell(msh_t *shell) {
    free(shell); 
}