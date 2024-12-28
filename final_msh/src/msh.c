#include "shell.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Include this for getopt
#include <string.h>


int main(int argc, char *argv[]) {
    // Process command line arguments
    int a[] = {0, 0, 0};
    int *s_limit = &a[0];
    int *j_limit = &a[1];
    int *l_limit = &a[2];

    int opt;
    while ((opt = getopt(argc, argv, "s:j:l:")) != -1) {
        switch (opt) {
            case 's':
                if (sscanf(optarg, "%d", s_limit) != 1 || s_limit <= 0) {
                    printf("usage: msh [-s NUMBER] [-j NUMBER] [-l NUMBER]\n");
                    return 1;
                }
                *s_limit = atoi(optarg);
                break;

            case 'j':
                if (sscanf(optarg, "%d", j_limit) != 1 || j_limit <= 0) {
                    printf("usage: msh [-s NUMBER] [-j NUMBER] [-l NUMBER]\n");
                    return 1;
                }
                *j_limit = atoi(optarg);
                break;

            case 'l':
                if (sscanf(optarg, "%d", l_limit) != 1 || l_limit <= 0) {
                    printf("usage: msh [-s NUMBER] [-j NUMBER] [-l NUMBER]\n");
                    return 1;
                }
                *l_limit = atoi(optarg);
                break;

            default:
                printf("usage: msh [-s NUMBER] [-j NUMBER] [-l NUMBER]\n");
                return 1;
        }

        if (optind < argc) {
            // Extra arguments found
            printf("usage: msh [-s NUMBER] [-j NUMBER] [-l NUMBER]\n");
            return 1;
        }
        
    }

    // Allocate and initialize msh_t state
    msh_t *shell_state = alloc_shell(*j_limit, *l_limit, *s_limit);

    // REPL mechanism
    char *line = NULL;
    size_t len = 0;

    printf("msh> ");
    ssize_t n_read;
    while ((n_read = getline(&line, &len, stdin)) != -1) {
        // Remove newline character
        if (line[n_read - 1] == '\n') {
            line[n_read - 1] = '\0';
        }

        // Check if the user wants to exit
        if (strcmp(line, "exit") == 0) {
            break;
        }

        // Evaluate the command line
        evaluate(shell_state, line);

        // Prompt for the next command
        printf("msh> ");
    }

    // Free allocated resources
    free(line);
    exit_shell(shell_state);

    return 0;
}