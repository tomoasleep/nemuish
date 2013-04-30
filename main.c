#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parse.h"
#include "env.h"
#include "options.h"
#include "execute.h"

extern void print_job_list(job*);

int main(int argc, char *argv[], char *env[])
{
    char *s;
    job *curr_job;
    envi *envs;

    options = malloc(sizeof(shell_options));

    // for (h = env; *h != NULL; h++) {
    //     printf("%s\n", *h);
    // }
    envs = make_env(env);
    options->env = envs;
    // print_env(envs);

    while((s = readline(PROMPT))) {
        if(!strcmp(s, "exit")) 
            break;
        if(*s)
            add_history(s);

        curr_job = parse_line(s);

        print_job_list(curr_job);
        exec_job_list(curr_job);

        free(s);
        free_job(curr_job);
    }

    return 0;
}
