#pragma once

#define ENVNAME_MAXLEN 30
#define ENVCONTENTS_MAXLEN 100

typedef struct _env_contents{
    char value[ENVCONTENTS_MAXLEN];
    struct _env_contents *next;
} env_contents;

typedef struct _env{
    char name[ENVNAME_MAXLEN];
    env_contents *contents;
    struct _env *next;
} envi;

envi *make_env(char *envs[]);
void print_env(envi *env);
envi *search_env_by_name(envi *env, char *name);

