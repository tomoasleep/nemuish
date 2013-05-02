#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "parse.h"
#include "print.h"
#include "env.h"
#include "options.h"

#define FILEPATH_MAXLEN 256

static int search_path(const char *program_name, char *path) {
    env_contents *iter;
    envi *pathenv = search_env_by_name(options->env, "PATH");
    if (!pathenv) {
        fprintf(stderr, "no path\n");
        return -1;
    }

    for (iter = pathenv->contents; iter != NULL; iter = iter->next) {
        strcpy(path, iter->value);
        strcat(path, "/");
        strcat(path, program_name);
        
        if (access(path, F_OK) == 0) return 0;
    }
    return -1;
}

static int exec_process(process* pr) {

    // int index = 0;
    // print_process(pr);

    if(pr->program_name == NULL) {
        return -1;
    }

    char path[FILEPATH_MAXLEN];
    if (pr->program_name[0] != '\0') {
        if (pr->program_name[0] == '.' && pr->program_name[1] == '/') {
            execve(pr->program_name, pr->argument_list, options->env_str);
        } else if (pr->program_name[0] == '/') {
            execve(pr->program_name, pr->argument_list, options->env_str);
        } else {
            search_path(pr->program_name, path);
            execve(path, pr->argument_list, options->env_str);
        }
    } else {
        search_path(pr->program_name, path);
        execve(path, pr->argument_list, options->env_str);
    }
    // error
    fprintf(stderr, "%s: command not found:\n", pr->program_name);
    exit(1);


    if(pr->input_redirection != NULL) {
        printf("  - input redirection = %s\n", pr->input_redirection);
    }

    if (pr->output_redirection != NULL)
        printf("  - output redirection = %s [ %s ]\n",
                pr->output_redirection,
                pr->output_option == TRUNC ? "trunc" : "append" );


    return 0;
}

static int wait_process_list(job *jb) {
    int exitval = 0;
    process *pr;

    if (jb->mode == FOREGROUND) {
        for(pr = jb->process_list; pr != NULL; pr = pr->next) {
            if (pr->pid < 0) {
                perror("wait");
                exit(1);
            }
            waitpid(pr->pid, &exitval, 0);
        }
    }
    return 0;
}

int exec_job_list(job* job_list)
{
    int index;
    job *jb;
    process *pr;

    for(index = 0, jb = job_list; jb != NULL; jb = jb->next, ++index) {
        int fileds[2][2] = {
            {-1, -1},
            {-1, -1}
        };
        int has_before = 0;

        for(pr = jb->process_list; pr != NULL; pr = pr->next) {
            // get before process'es pipeno (for input)
            fileds[0][0] = fileds[1][0];
            fileds[0][1] = fileds[1][1];

            if (pr->next) {
                if (pipe(fileds[1]) < 0) {
                    perror("pipe");
                    exit(3);
                }
            }

            pr->pid = fork();

            if (pr->pid < 0) {
                perror("fork");
                exit(3);
            } else if (pr->pid > 0) {
                // parents do not use before process'es pipe 
                if (fileds[0][0] != -1) close(fileds[0][0]);
                if (fileds[0][1] != -1) close(fileds[0][1]);

                if (pr->next) {
                    has_before = 1;
                } else has_before = 0;
                continue;
            }

            // redirection
            if (pr->input_redirection) {
                int inputds = open(pr->input_redirection, O_RDONLY);
                close(STDIN_FILENO); dup2(inputds, STDIN_FILENO);
                close(inputds);
            }

            if (pr->output_redirection) {
                int outputds = -1;
                switch (pr->output_option) {
                    case APPEND:
                        outputds = open(pr->output_redirection,
                                O_WRONLY | O_APPEND | O_CREAT, S_IREAD | S_IWRITE);
                        break;
                    case TRUNC:
                        outputds = open(pr->output_redirection,
                                O_WRONLY | O_TRUNC | O_CREAT, S_IREAD | S_IWRITE);
                }
                close(STDOUT_FILENO); dup2(outputds, STDOUT_FILENO);
                close(outputds);
            }

            // pipeline
            if (has_before) {
                // connect before process'es out to in
                close(STDIN_FILENO); dup2(fileds[0][0], STDIN_FILENO);
                close(fileds[0][0]);
                // close unuse pipe
                close(fileds[0][1]);
            }
            if (pr->next) {
                // close unuse pipe
                close(fileds[1][0]);
                // connect my process'es out to next process'es in
                close(STDOUT_FILENO); dup2(fileds[1][1], STDOUT_FILENO);
                close(fileds[1][1]);
            }
            exec_process(pr);
        }
    }

    for(index = 0, jb = job_list; jb != NULL; jb = jb->next, index++) {
        wait_process_list(jb);
    }

    //    printf("id %d [ %s ]\n", index, 
    //            jb->mode == FOREGROUND ? "foreground" : "background" );

        // for(pr = jb->process_list; pr != NULL; pr = pr->next) {
        //     if(print_process( pr ) < 0) {
        //         exit(EXIT_FAILURE);
        //     }
        //     if(jb->next != NULL) {
        //         printf( "\n" );
        //     }
    return 0;
}
