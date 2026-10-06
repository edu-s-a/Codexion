/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 12:32:47 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/06 16:33:22 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	coder_burned_out(t_sim *sim, int coder_id, long now)
{
	long	deadline;

	deadline = sim->coders[coder_id].last_compile_start + sim->config.time_to_burnout;
	return (now >= deadline);
}

static bool	all_compiles_done(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (sim->coders[i].compile_count < sim->config.number_of_compiles_required)
			return (false);
		i++;
	}
	return (true);
}

static void	wake_all_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].d_mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].d_mutex);
		i++;
	}
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;
	long	now;
	int		burned_coder;

	sim = (t_sim *)arg;
	while (true)
	{
		pthread_mutex_lock(&sim->log_lock);
		if (sim->stopped)
		{
			pthread_mutex_unlock(&sim->log_lock);
			break;
		}
		now = get_elapsed_time(sim);
		burned_coder = -1;
		i = 0;
		while (i < sim->config.number_of_coders && burned_coder == -1)
		{
			if (coder_burned_out(sim, i, now))
				burned_coder = i;
			i++;
		}
		if (burned_coder != -1)
		{
			pthread_mutex_unlock(&sim->log_lock);
			log_msg(sim, burned_coder, "burned out");
			pthread_mutex_lock(&sim->log_lock);
			sim->stopped = true;
			pthread_mutex_unlock(&sim->log_lock);
			wake_all_dongles(sim);
			break;
		}
		if (all_compiles_done(sim))
		{
			sim->stopped = true;
			pthread_mutex_unlock(&sim->log_lock);
			wake_all_dongles(sim);
			break;
		}
		pthread_mutex_unlock(&sim->log_lock);
		usleep(1000);
	}
	return (NULL);
}