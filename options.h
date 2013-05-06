#pragma once

#include "env.h"
#include "buildin.h"

typedef struct _shell_options{
    envi *env;
    char **env_str;
    buildin_cmd *bcmds;
} shell_options;

shell_options *options;


