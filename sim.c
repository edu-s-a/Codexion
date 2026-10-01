/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 17:14:17 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/01 18:04:08 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool    init_sim(t_sim *sim)
{
    int     count;
    int     i;

    count = sim->config.number_of_coders;
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
        sim->dongles[i].dongle_cooldown = 0;
        sim->dongles[i].held = false;
        if (pthread_mutex_init(&sim->dongles[i].d_mutex, NULL) != 0)
        {
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
        sim->coders[i].right =  &sim->dongles[(i+1) % count];
        i++;
    }
    return (true);
}

void    destroy_sim(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->config.number_of_coders)
    {
        pthread_mutex_destroy(&sim->dongles[i].d_mutex);
        i++;
    }
    pthread_mutex_destroy(&sim->log_lock);
    free(sim->dongles);
    free(sim->coders);
}