#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parse.h"
#include "env.h"
#include "options.h"

#define FILEPATH_MAXLEN 256

static char *search_path(char *program_name) {
    env_contents *iter;
    envi *pathenv = search_env_by_name(options->env, "PATH");
    if (!pathenv) {
        fprintf(stderr, "no path\n");
        return NULL;
    }

    for (iter = pathenv->contents; iter != NULL; iter = iter->next) {
        char filepath[FILEPATH_MAXLEN];
        strcpy(filepath, iter->value);
        strcat(filepath, "/");
        strcat(filepath, program_name);
        
        if (access(filepath, F_OK) == 0) return filepath;
    }
    return NULL;
}

static int exec_process(process* pr) {

    int index = 0;
    print_process(pr);

    if(pr->program_name == NULL) {
        return -1;
    }

    if (pr->program_name[0] != '\0') {
        if (pr->program_name[0] == '.' && pr->program_name[1] == '/') {
            execve(pr->program_name, pr->argument_list, options->env_str);
        } else if (pr->program_name[0] == '/') {
            execve(pr->program_name, pr->argument_list, options->env_str);
        } else {
            execve(search_path(pr->program_name), pr->argument_list, options->env_str);
        }
    } else {
        execve(search_path(pr->program_name), pr->argument_list, options->env_str);
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

int exec_job_list(job* job_list)
{
    int index;
    int exitval = 0;
    job*     jb;
    pid_t pid;

    for(index = 0, jb = job_list; jb != NULL; jb = jb->next, ++index) {
        
        // print_process(jb->process_list);

        pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid > 0) {
            if (jb->mode == FOREGROUND) {
                waitpid(pid, &exitval, 0);
            } else {
                fprintf(stderr, "background pid:%d\n", pid);
            }
        } else {
            exec_process(jb->process_list);
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
        // }
    }
    return exitval;
}
