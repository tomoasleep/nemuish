#include <string.h>
#include <stdlib.h>
#include "buildin.h"

buildin_cmd *create_bcmd(
        buildin_cmd *bcmds,
        char *name,
        int (*func)(int argc, char *argv[], char *env[])){

    buildin_cmd *bcmd = malloc(sizeof(buildin_cmd));

    strcpy(bcmd->command_name, name);
    bcmd->next = bcmds;
    bcmd->exec = func;

    return bcmd;
}


buildin_cmd *search_bcmd(buildin_cmd *target, char *name) {
    buildin_cmd *iter = target;

    for (iter = target; iter != NULL; iter = iter->next) {
        if (strcmp(name, iter->command_name) == 0) {
            return iter;
        }
    }
    return NULL;
}



