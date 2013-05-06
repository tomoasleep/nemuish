#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "cd.h"
#include "options.h"
#include "env.h"

int exec_cd(int argc, char *argv[], char *env[]) {
    char *pth = NULL;
    envi *hme = NULL;

    if (argc > 2) {
        pth = argv[1];
    } else if ((hme = search_env_by_name(options->env, "HOME"))) {
        if (hme->contents) {
            pth = hme->contents->value;
        }
    }

    if (chdir(pth) < 0) {
        perror("cd");
        return 1;
    }
    return 0;
}




