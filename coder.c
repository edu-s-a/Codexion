/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 17:14:10 by edsole-a          #+#    #+#             */
/*   Updated: 2026/09/30 18:44:39 by edsole-a         ###   ########.fr       */
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
    while(true)
    {
        pthread_mutex_lock(&coder->left->d_mutex);
        if(&coder->left != &coder->right)
            pthread_mutex_lock(&coder->right->d_mutex);
        log_msg(sim,coder->id,"has taken a dongle");
        log_msg(sim,coder->id,"has taken a dongle");
        log_msg(sim,coder->id,"is compiling");
        usleep(sim->config.compile_time *1000);
        if (coder->left != coder->right)
			pthread_mutex_unlock(&coder->right->d_mutex);
        log_msg(sim,coder->id,"is debugging");
        usleep(sim->config.debug_time *1000);
        log_msg(sim,coder->id,"is refactoring");
        usleep(sim->config.refactor_time *1000);
    }
}