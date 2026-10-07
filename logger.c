/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 19:22:09 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/07 12:46:57 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_msg(t_sim *sim, int coder_id, const char *msg)
{
	long	elapsed;

	pthread_mutex_lock(&sim->log_lock);
	if (sim->stopped)
	{
		pthread_mutex_unlock(&sim->log_lock);
		return ;
	}
	elapsed = get_elapsed_time(sim);
	printf("%ld %d %s\n", elapsed, coder_id + 1, msg);
	pthread_mutex_unlock(&sim->log_lock);
}
