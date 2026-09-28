#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct s_config
{
    int num_coders;
    int burnout_time;
    int compile_time;
    int debug_time;
    int refactor_time;
    int num_compiles;
    int cooldown;
    char *scheduler;
}   t_config;

