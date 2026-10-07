/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 19:06:13 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/07 16:26:13 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include    "codexion.h"

long    get_elapsed_time(t_sim *sim)
{
    long sec;
    long usec;
    long elapsed;
    struct timeval now;

    gettimeofday(&now, NULL);
    sec = (now.tv_sec - sim->t0.tv_sec);
    usec = (now.tv_usec - sim->t0.tv_usec); 
    if (usec < 0)
    {
        sec -= 1;
        usec += 1000000;
    }
    elapsed = (sec * 1000 + usec / 1000);
    return (elapsed);
}
