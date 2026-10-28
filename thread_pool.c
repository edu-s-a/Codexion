/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:29:56 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/28 16:47:00 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_coder_threads(t_sim *sim, t_thread_arg *t_arg)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		t_arg[i].sim = sim;
		t_arg[i].coder_id = i;
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &t_arg[i]) != 0)
			break ;
		i++;
	}
	return (i);
}

static void	join_coder_threads(t_sim *sim, int created, bool monitor_on)
{
	int	i;

	i = 0;
	while (i < created)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	if (monitor_on)
		pthread_join(sim->monitor_thread, NULL);
}

int	run_simulation(t_sim *sim)
{
	t_thread_arg	*t_arg;
	int				created;
	bool			monitor_on;
	int				ret;

	t_arg = malloc(sizeof(t_thread_arg) * sim->config.number_of_coders);
	if (!t_arg)
	{
		fprintf(stderr, "ERROR: out of memory\n");
		destroy_sim(sim);
		return (1);
	}
	created = create_coder_threads(sim, t_arg);
	ret = pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim);
	monitor_on = (ret == 0);
	ret = (created < sim->config.number_of_coders || !monitor_on);
	if (ret)
	{
		fprintf(stderr, "ERROR: could not create all threads\n");
		stop_simulation(sim);
	}
	join_coder_threads(sim, created, monitor_on);
	free(t_arg);
	destroy_sim(sim);
	return (ret);
}
