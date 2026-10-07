/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 17:14:10 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/07 11:57:19 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	acquire_dongles(t_coder *coder, t_sim *sim)
{
	if (coder->id == sim->config.number_of_coders - 1)
	{
		if (!dongle_request(coder->right, sim, coder->id))
			return (false);
		if (!dongle_request(coder->left, sim, coder->id))
		{
			dongle_release(coder->right, sim);
			return (false);
		}
		return (true);
	}
	if (!dongle_request(coder->left, sim, coder->id))
		return (false);
	if (coder->left != coder->right)
	{
		if (!dongle_request(coder->right, sim, coder->id))
		{
			dongle_release(coder->left, sim);
			return (false);
		}
	}
	return (true);
}

static void	release_dongles(t_coder *coder, t_sim *sim)
{
	if (coder->id == sim->config.number_of_coders - 1)
	{
		dongle_release(coder->right, sim);
		dongle_release(coder->left, sim);
		return ;
	}
	dongle_release(coder->left, sim);
	if (coder->left != coder->right)
		dongle_release(coder->right, sim);
}

static void	do_compile_phase(t_coder *coder, t_sim *sim)
{
	pthread_mutex_lock(&sim->log_lock);
	coder->last_compile_start = get_elapsed_time(sim);
	pthread_mutex_unlock(&sim->log_lock);
	log_msg(sim, coder->id, "has taken a dongle");
	log_msg(sim, coder->id, "has taken a dongle");
	log_msg(sim, coder->id, "is compiling");
	usleep(sim->config.time_to_compile * 1000);
	pthread_mutex_lock(&sim->log_lock);
	coder->compile_count++;
	pthread_mutex_unlock(&sim->log_lock);
}

static void	do_debug_refactor_phase(t_coder *coder, t_sim *sim)
{
	log_msg(sim, coder->id, "is debugging");
	usleep(sim->config.time_to_debug * 1000);
	log_msg(sim, coder->id, "is refactoring");
	usleep(sim->config.time_to_refactor * 1000);
}

void	*coder_routine(void *arg)
{
	t_thread_arg	*t_arg;
	t_sim			*sim;
	t_coder			*coder;

	t_arg = (t_thread_arg *)arg;
	sim = t_arg->sim;
	coder = &sim->coders[t_arg->coder_id];
	while (true)
	{
		if (!acquire_dongles(coder, sim))
			return (NULL);
		do_compile_phase(coder, sim);
		release_dongles(coder, sim);
		do_debug_refactor_phase(coder, sim);
	}
	return (NULL);
}
