/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 12:32:47 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/07 19:01:39 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	find_burned_coder(t_sim *sim, long now)
{
	long	deadline;
	int		i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		deadline = sim->coders[i].last_compile_start
			+ sim->config.time_to_burnout;
		if (now >= deadline)
			return (i);
		i++;
	}
	return (-1);
}

static bool	all_compiles_done(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (sim->coders[i].compile_count
			< sim->config.number_of_compiles_required)
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

static void	stop_simulation(t_sim *sim)
{
	pthread_mutex_lock(&sim->log_lock);
	sim->stopped = true;
	pthread_mutex_unlock(&sim->log_lock);
	wake_all_dongles(sim);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	long	now;
	int		burned_coder;

	sim = (t_sim *)arg;
	while (!sim->stopped)
	{
		now = get_elapsed_time(sim);
		burned_coder = find_burned_coder(sim, now);
		if (burned_coder != -1)
		{
			log_msg(sim, burned_coder, "burned out");
			stop_simulation(sim);
			break ;
		}
		if (all_compiles_done(sim))
		{
			stop_simulation(sim);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
