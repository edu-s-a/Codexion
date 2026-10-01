/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:05:42 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/01 18:27:51 by edsole-a         ###   ########.fr       */
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
#include <stdbool.h>

typedef enum e_scheduler
{
    fifo,
    edf
}   t_scheduler;

typedef struct s_config
{
    int             number_of_coders;
    long            time_to_burnout;
    long            time_to_compile;
    long            time_to_debug;
    long            time_to_refactor;
    long            number_of_compiles_required;
    long            dongle_cooldown;
    t_scheduler     scheduler;
}   t_config;

typedef struct s_dongle
{
    int             id;
    int             coder_id;
    long            dongle_cooldown;
    bool            held;
    pthread_mutex_t d_mutex;
    pthread_cond_t  d_cond;
}   t_dongle;

typedef struct s_coder
{   
    int             id;
    int             compile_count;
    long            last_compile_start;
    t_dongle        *left;
    t_dongle        *right;
    pthread_t       thread;
}   t_coder;

typedef struct s_sim
{
    t_config        config;
    pthread_mutex_t log_lock;
    struct timeval  t0;
    bool            stopped;
    t_dongle        *dongles;
    t_coder         *coders;
}   t_sim;

typedef struct s_thread_arg
{
	t_sim	*sim;
	int		coder_id;
}	t_thread_arg;

typedef struct s_heap_entry
{
	long	priority;
	int		coder_id;
}	t_heap_entry;

typedef struct s_heap
{
	t_heap_entry	*data;
	int				size;
	int				capacity;
}	t_heap;

long    get_elapsed_time(t_sim *sim);
void    log_msg(t_sim *sim, int coder_id, const char *msg);

//SIM
bool    init_sim(t_sim *sim);
void    destroy_sim(t_sim *sim);

//CODER ROUTINE
void    *coder_routine(void *arg);

#endif
