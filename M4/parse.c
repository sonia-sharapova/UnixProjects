#include "parse.h"
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

int compute_num_args(const char *line);
int find_arg(const char *line, const int index, char *argument);
shellinfo_t parse_lines(const char*lines[], const int lines_len);

int compute_num_args(const char *line) {
    char *line_copy = strdup(line); // Make copy for str (accounts for modifications)
    char *token = strtok(line_copy, " ");
    
    int count = 0;
    while (token != NULL) {
        count++; // Increment count if index not empty
        token = strtok(NULL, " ");
    }
    return count;
}

int find_arg(const char *line, const int index, char *argument) {
    char line_copy[strlen(line) + 1];  // Make copy for str (accounts for modifications)
    strcpy(line_copy, line);
    char *token = strtok(line_copy, " "); // Use str for string splitting
    
    int arg_index = 0;
    while (token != NULL) {
        if (arg_index == index) {
            strcpy(argument, token);  // Copy the argument into parameter if found
            return 0; 
        }
        arg_index++;
        token = strtok(NULL, " "); 
    }
    return 1; // Invalid index, arg not found
}


// Function to parse lines and count fg and bg jobs
shellinfo_t parse_lines(const char *lines[], int numLines) {
    shellinfo_t results;
    results.fg = 0;
    results.bg = 0;

    for (int i = 0; i < numLines; i++) {
        char line_copy[strlen(lines[i]) + 1];
        strcpy(line_copy, lines[i]);
        char *token = strtok(line_copy, ";");

        while (token != NULL) {
            // Count the number of & characters in the token
            int ampersand_count = 0;
            char *ampersand = strchr(token, '&');
            while (ampersand != NULL) {
                ampersand_count++;
                ampersand = strchr(ampersand + 1, '&');
            }

            if (ampersand_count > 0) {
                results.bg += ampersand_count;
            } else {
                results.fg++;
            }

            token = strtok(NULL, ";");
        }
    }

    return results;
}