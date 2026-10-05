/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 17:14:10 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/05 13:50:56 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    *coder_routine(void *arg)
{
    t_thread_arg    *t_arg;
    t_sim           *sim;
    t_coder         *coder;

    t_arg = (t_thread_arg *)arg;
    sim = t_arg->sim;
    coder = &sim->coders[t_arg->coder_id];
    printf("coder %d starting\n", coder->id);
    while(true)
    {
        if (!dongle_request(coder->left, sim, coder->id))
            return (NULL);
        if(coder->left != coder->right)
        {
            if (!dongle_request(coder->right, sim, coder->id))
			{
				dongle_release(coder->left, sim);
				return (NULL);
			}
        }
        log_msg(sim,coder->id,"has taken a dongle");
        log_msg(sim,coder->id,"has taken a dongle");
        log_msg(sim,coder->id,"is compiling");
        usleep(sim->config.time_to_compile *1000);
        coder->last_compile_start = get_elapsed_time(sim);
		coder->compile_count++;
		dongle_release(coder->left, sim);
		if (coder->left != coder->right)
			dongle_release(coder->right, sim);
        log_msg(sim,coder->id,"is debugging");
        usleep(sim->config.time_to_debug *1000);
        log_msg(sim,coder->id,"is refactoring");
        usleep(sim->config.time_to_refactor *1000);
    }
    return(NULL);
}