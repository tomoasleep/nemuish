#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parse.h"

extern void print_job_list(job*);

int main(int argc, char *argv[])
{
    char* s;
    job *curr_job;

    while((s = readline(PROMPT))) {
        if(!strcmp(s, "exit")) 
            break;
        if(*s)
            add_history(s);

        curr_job = parse_line(s);

        print_job_list(curr_job);

        free(s);
        free_job(curr_job);
    }

    return 0;
}
