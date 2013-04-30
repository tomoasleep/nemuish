#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "env.h" 

typedef enum _env_parse_state {
    NAME,
    CONTENTS,
} env_parse_state;

static envi *parse_envstr(char *env);

static envi *init_env(void) {
    envi *env;
    env = malloc(sizeof(envi));
    env->next = NULL;

    return env;
}

static env_contents *init_env_contents(void) {
    env_contents *eco;
    eco = malloc(sizeof(env_contents));
    eco->next = NULL;
    return eco;
}

envi *make_env(char *envs_str[]) {
    char **iter;
    envi *envs = NULL, *before = NULL;

    for (iter = envs_str; *iter != NULL; iter++) {
        envi *env;
        if ((env = parse_envstr(*iter)) != NULL) {
            if (!envs) envs = env;
            if (before) before->next = env;
            before = env;
        }
    }
    return envs;
}

static envi *parse_envstr(char *strenv) {
    char *iter, *idx;
    int i;
    envi *env;
    env_contents *before = NULL;
    env_parse_state state = NAME;

    for (iter = strenv, idx = strenv; iter != NULL; iter++) {
        if (state == NAME) {
            if (*iter == '=') {
                env = init_env();

                for (i = 0; idx != iter; idx++) env->name[i++] = *idx;
                env->name[i] = '\0';
                idx++;
                state = CONTENTS;
            }
            if (*iter == '\0') {
                break;
            }
        } else if (state == CONTENTS) {
            if (*iter == ':' || *iter == '\0') {
                env_contents *eco = init_env_contents();

                if (before) before->next = eco;
                if (!env->contents) env->contents = eco;
                before = eco;

                for (i = 0; idx != iter; idx++) eco->value[i++] = *idx;
                eco->value[i] = '\0';
                idx++;
                if (*iter == '\0') break;
            }
        }
    }

    if (state == NAME) return NULL;
    else return env;
}

void print_env(envi *env) {
    envi *iter;

    for (iter = env; iter != NULL; iter = iter->next) {
        env_contents *eco;

        printf("%s: ", iter->name);
        for (eco = iter->contents; eco != NULL; eco = eco->next) {
            printf("%s, ", eco->value);
        }
        printf("\n");
    }
}

envi *search_env_by_name(envi *env, char* name) {
    if (env == NULL) return NULL;
    if (env->name == NULL) return search_env_by_name(env->next, name);
    if (strcmp(env->name, name) == 0) {
        return env;
    } else {
        return search_env_by_name(env->next, name);
    }
}

