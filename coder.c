/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 17:14:10 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/07 11:31:28 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
		if (coder->id == sim->config.number_of_coders - 1)
        {
            if (!dongle_request(coder->right, sim, coder->id))
                return (NULL);
            if (!dongle_request(coder->left, sim, coder->id))
            {
                dongle_release(coder->right, sim);
                return (NULL);
            }
        }
        else
        {
            if (!dongle_request(coder->left, sim, coder->id))
                return (NULL);
            if (coder->left != coder->right)
            {
                if (!dongle_request(coder->right, sim, coder->id))
                {
                    dongle_release(coder->left, sim);
                    return (NULL);
                }
            }
        }
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
		if (coder->id == sim->config.number_of_coders - 1)
        {
            dongle_release(coder->right, sim);
            dongle_release(coder->left, sim);
        }
        else
        {
            dongle_release(coder->left, sim);
            if (coder->left != coder->right)
                dongle_release(coder->right, sim);
        }
        log_msg(sim, coder->id, "is debugging");
		usleep(sim->config.time_to_debug * 1000);
		log_msg(sim, coder->id, "is refactoring");
		usleep(sim->config.time_to_refactor * 1000);
	}
	return (NULL);
}