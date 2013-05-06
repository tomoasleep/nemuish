#pragma once
#define BCMD_MAXLEN 20

typedef struct _buildin_cmd{
    char command_name[BCMD_MAXLEN];
    struct _buildin_cmd *next;
    int (*exec)(int argc, char *argv[], char *env[]);
} buildin_cmd;

buildin_cmd *create_bcmd(
        buildin_cmd *bcmds,
        char *name,
        int (*exec)(int argc, char *argv[], char *env[]));

buildin_cmd *search_bcmd(buildin_cmd *target, char *name);

