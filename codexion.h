/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:05:42 by edsole-a          #+#    #+#             */
/*   Updated: 2026/09/29 20:01:53 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
#define CODEXION_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>

typedef struct s_config
{
    int num_coders;
    long burnout_time;
    long compile_time;
    long debug_time;
    long refactor_time;
    long num_compiles;
    long cooldown;
    char *scheduler;
}   t_config;

typedef struct s_sim
{
    struct timeval t0;
}   t_sim;

typedef struct s_dongle
{
    int     id;
    int     coder_id;
    pthread_mutex_t d_mutex;
    
}   t_dongle;

typedef struct s_coder
{
    int             id;
    int             compile_count;
    t_dongle        *left;
    t_dongle        *right;
    struct timeval  last_compile_start;
}   t_coder;

long    get_elapsed_time(t_sim *sim);
void    log_msg(t_sim *sim, int coder_id, const char *msg);

#endif
