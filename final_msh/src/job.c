#include "job.h"
#include "shell.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Add a job to the job array
bool add_job(job_t *jobs, int max_jobs, pid_t pid, job_state_t state, const char *cmd_line) {

    for (int i = 0; i < max_jobs; i++) {
        if (jobs[i].pid == -1) {  
            jobs[i].state = state;
            jobs[i].pid = pid;
            jobs[i].jid = i; 
            strcpy(jobs[i].cmd_line, cmd_line);
            return true;
        }
    }
    // If no empty slot for the job
    return false; 
}

// Remove the job at the index, return true if successful
bool delete_job(job_t *jobs, pid_t pid) {

    for (int i = 0; i < sizeof(jobs); i++) {
        if (jobs[i].pid == pid) {
            free(jobs[i].cmd_line);  
            jobs[i].cmd_line = NULL;
            jobs[i].state = UNDEFINED;
            jobs[i].pid = -1;
            jobs[i].jid = -1;
            return true;
        }
    }
    // Job not found
    return false; 
}

// Free the entire array or jobs
void free_jobs(job_t *jobs, int max_jobs) {
    for (int i = 0; i < max_jobs; i++) {
        if (jobs[i].cmd_line != NULL) {
            free(jobs[i].cmd_line);
        }
    }
    free(jobs); 
}