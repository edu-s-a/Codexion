/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:29:56 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/07 16:49:08 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	create_coder_threads(t_sim *sim, t_thread_arg *t_arg)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		t_arg[i].sim = sim;
		t_arg[i].coder_id = i;
		pthread_create(&sim->coders[i].thread, NULL, coder_routine, &t_arg[i]);
		i++;
	}
	pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim);
}

static void	join_coder_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor_thread, NULL);
}

void	run_simulation(t_sim *sim)
{
	t_thread_arg	*t_arg;

	t_arg = malloc(sizeof(t_thread_arg) * sim->config.number_of_coders);
	if (!t_arg)
	{
		destroy_sim(sim);
		return ;
	}
	create_coder_threads(sim, t_arg);
	join_coder_threads(sim);
	free(t_arg);
	destroy_sim(sim);
}
