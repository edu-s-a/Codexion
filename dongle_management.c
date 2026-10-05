/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 11:54:12 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/05 13:47:32 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool    dongle_request(t_dongle *d, t_sim *sim, int coder_id)
{
    t_request	front;
	bool		my_turn;
	long		priority;
    
    pthread_mutex_lock(&d->d_mutex);
    if (sim->config.scheduler == fifo)
    {
        pthread_mutex_lock(&sim->priority_lock);
		priority = sim->next_priority;
		sim->next_priority++;
		pthread_mutex_unlock(&sim->priority_lock);
    }
    else 
        priority = sim->coders[coder_id].last_compile_start + sim->config.time_to_burnout;
	
    heap_push(&d->waiters, coder_id, priority);
    while (true)
	{
		my_turn = false;
		if (heap_peek(&d->waiters, &front) && front.coder_id == coder_id
			&& !d->held && get_elapsed_time(sim) >= d->cooldown_until)
			my_turn = true;
		if (my_turn || sim->stopped)
			break;
		pthread_cond_wait(&d->cond, &d->d_mutex);
	}
    if(sim->stopped && !my_turn)
    {
        heap_remove(&d->waiters, coder_id);
        pthread_mutex_unlock(&d->d_mutex);
        return (true);
    }
    heap_pop(&d->waiters, &front);
    d->held = true;
    pthread_mutex_unlock(&d->d_mutex);
    return(true);
}

void    dongle_release(t_dongle *d, t_sim *sim)
{
    pthread_mutex_lock(&d->d_mutex);
    d->held = false;
    d->cooldown_until = get_elapsed_time(sim) + sim->config.dongle_cooldown;
    pthread_cond_broadcast(&d->cond);
    pthread_mutex_unlock(&d->d_mutex);
}

void	cleanup_partial_dongles(t_sim *sim, int up_to)
{
	int	j;

	j = 0;
	while (j < up_to)
	{
		pthread_mutex_destroy(&sim->dongles[j].d_mutex);
		heap_destroy(&sim->dongles[j].waiters);
		pthread_cond_destroy(&sim->dongles[j].cond);
		j++;
	}
}