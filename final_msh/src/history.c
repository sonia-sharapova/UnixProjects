#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *HISTORY_FILE_PATH = "../data/.msh_history";

history_t *alloc_history(int max_history) {
    history_t* history_p = (history_t*)(malloc(sizeof(history_t)));
    history_p->lines = (char **)(malloc(sizeof(char *) * max_history));

    for (int i = 0; i < max_history; i++) {
        history_p->lines[i] = NULL;
    }

    history_p->max_history = max_history;
    history_p->next = 0;

    // To open history file
    FILE *h_file = fopen(HISTORY_FILE_PATH, "r"); 


    if (h_file) {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        while ((read = getline(&line, &len, h_file)) != -1) {
            if (history_p->next >= max_history) {
                // Shift history up when max_history is reached
                free(history_p->lines[0]);
                memmove(history_p->lines, history_p->lines + 1, sizeof(char *) * (max_history - 1));
                history_p->lines[max_history - 1] = NULL;
                history_p->next--;
            }
            // Remove newline character
            if (line[read - 1] == '\n') {
                line[read - 1] = '\0';
            }
            history_p->lines[history_p->next++] = strdup(line);
        }
        free(line);
        fclose(h_file);
    }
    return history_p;
}


// Add to the history
void add_line_history(history_t *history, const char *cmd_line) {
    // Check for empty or "exit" command
    if (!cmd_line || strcmp(cmd_line, "") == 0 || strcmp(cmd_line, "exit") == 0) {
        return;
    }

    // If history is full, delete the oldest command line
    if (history->next == history->max_history) {
        free(history->lines[0]);
        memmove(history->lines, history->lines + 1, sizeof(char *) * (history->max_history - 1));
        history->lines[history->max_history - 1] = strdup(cmd_line);
    } else {
        // Add new command line to history
        history->lines[history->next++] = strdup(cmd_line);
    }
}

// Print history
void print_history(history_t *history) {
    for(int i = 1; i <= history->next; i++) {
        printf("%5d\t%s\n",i,history->lines[i-1]);
    }
}

// Find a specific line
char *find_line_history(history_t *history, int index) {
    // For one-based indexing
    int adjusted_index = index - 1;

    // Check if index is within the valid range
    if (adjusted_index < 0 || adjusted_index >= history->next) {
        return NULL;
    }

    // Return the command line at the adjusted index
    return history->lines[adjusted_index];
}

// Free the memory from history
void free_history(history_t *history) {
    // Open the file for writing
    FILE *file = fopen(HISTORY_FILE_PATH, "w");
    
    // Write history to file
    for (int i = 0; i < history->next; i++) {
        if (history->lines[i] != NULL) {
            fprintf(file, "%s\n", history->lines[i]);
        }
    }
    fclose(file);
    
    // Free the memory allocated for the lines
    for (int i = 0; i < history->next; i++) {
        free(history->lines[i]);
    }
    free(history->lines);

    // Free history struct
    free(history);
}



