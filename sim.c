/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 17:14:17 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/07 17:01:05 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	init_sim(t_sim *sim)
{
	int	count;
	int	i;

	count = sim->config.number_of_coders;
	sim->next_priority = 0;
	if (pthread_mutex_init(&sim->priority_lock, NULL) != 0)
		return (false);
	sim->dongles = malloc(sizeof(t_dongle) * count);
	if (!sim->dongles)
		return (false);
	sim->coders = malloc(sizeof(t_coder) * count);
	if (!sim->coders)
	{
		free (sim->dongles);
		return (false);
	}
	i = 0;
	while (i < count)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].coder_id = -1;
		sim->dongles[i].cooldown_until = 0;
		sim->dongles[i].held = false;
		if (pthread_mutex_init(&sim->dongles[i].d_mutex, NULL) != 0
			|| !heap_init(&sim->dongles[i].waiters, count)
			|| pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
		{
			cleanup_partial_dongles(sim, i);
			free(sim->dongles);
			free(sim->coders);
			return (false);
		}
		i++;
	}
	i = 0;
	while (i < count)
	{
		sim->coders[i].id = i;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_start = 0;
		sim->coders[i].left = &sim->dongles[i];
		sim->coders[i].right = &sim->dongles[(i + 1) % count];
		i++;
	}
	return (true);
}

void	destroy_sim(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].d_mutex);
		heap_destroy(&sim->dongles[i].waiters);
		pthread_cond_destroy(&sim->dongles[i].cond);
		i++;
	}
	pthread_mutex_destroy(&sim->log_lock);
	pthread_mutex_destroy(&sim->priority_lock);
	free(sim->dongles);
	free(sim->coders);
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
