#ifndef _JOB_H_
#define _JOB_H_

#include <sys/types.h>
#include <stdbool.h>


typedef enum job_state { FOREGROUND, BACKGROUND, SUSPENDED, UNDEFINED } job_state_t;

// Represents a job in a shell.
typedef struct job {
    char *cmd_line;     // The command line for this specific job.
    job_state_t state;  // The current state for this job.
    pid_t pid;          // The process id for this job.
    int jid;            // The job number for this job.
} job_t;

// Function declarations
/*
* add_job: function that adds a new job to the array
*
* jobs: An array jobs that are running in the shell.
*
* max_jobs: The maximum number of jobs that can be in existence at any point in time.
*
* pid: The ID of the process
*
* state: The state of the job
*
* cmd_line: The command line given to the shell
*
* Returns: Return a false if job was not added, and true if job was added.
*/
bool add_job(job_t *jobs, int max_jobs, pid_t pid, job_state_t state, const char *cmd_line);

/*
* delete_job: Function that deletes a job from the array
*
* jobs: An array jobs that are running in the shell.
*
* pid: The pid of the job to be deleted
*
* Returns: false if the job was not found
*/
bool delete_job(job_t *jobs, pid_t pid);

/*
* free_jobs: function to deallocate the jobs array.
*
* jobs: An array jobs that are running in the shell.
*
* max_jobs: The maximum number jobs for memory.
*/
void free_jobs(job_t *jobs, int max_jobs);

#endif
