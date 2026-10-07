/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:05:42 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/07 12:50:46 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <sys/time.h>
# include <stdbool.h>

typedef enum e_scheduler
{
	fifo,
	edf
}	t_scheduler;

typedef struct s_config
{
	int			number_of_coders;
	long		time_to_burnout;
	long		time_to_compile;
	long		time_to_debug;
	long		time_to_refactor;
	long		number_of_compiles_required;
	long		dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

typedef struct s_request
{
	int		coder_id;
	long	priority;
}	t_request;

typedef struct s_heap
{
	t_request	*data;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	int				coder_id;
	long			cooldown_until;
	bool			held;
	pthread_mutex_t	d_mutex;
	t_heap			waiters;
	pthread_cond_t	cond;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	int			compile_count;
	long		last_compile_start;
	t_dongle	*left;
	t_dongle	*right;
	pthread_t	thread;
}	t_coder;

typedef struct s_sim
{
	t_config		config;
	pthread_mutex_t	log_lock;
	struct timeval	t0;
	bool			stopped;
	t_dongle		*dongles;
	t_coder			*coders;
	long			next_priority;
	pthread_mutex_t	priority_lock;
	pthread_t		monitor_thread;
}	t_sim;

typedef struct s_thread_arg
{
	t_sim	*sim;
	int		coder_id;
}	t_thread_arg;

long	get_elapsed_time(t_sim *sim);
void	log_msg(t_sim *sim, int coder_id, const char *msg);

/* SIM */
bool	init_sim(t_sim *sim);
void	destroy_sim(t_sim *sim);

/* CODER ROUTINE */
void	*coder_routine(void *arg);

/* HEAP */
bool	heap_init(t_heap *h, int capacity);
void	heap_destroy(t_heap *heap);
bool	heap_push(t_heap *heap, int coder_id, long priority);
int		find_min_index(t_heap *heap);
bool	heap_peek(t_heap *heap, t_request *out);
bool	heap_pop(t_heap *heap, t_request *out);
bool	heap_remove(t_heap *heap, int coder_id);
bool	heap_is_empty(t_heap *heap);

/* DONGLE MANAGEMENT */
bool	dongle_request(t_dongle *d, t_sim *sim, int coder_id);
void	dongle_release(t_dongle *d, t_sim *sim);
void	cleanup_partial_dongles(t_sim *sim, int up_to);

/* MONITOR */
void	*monitor_routine(void *arg);

#endif
