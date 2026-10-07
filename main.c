/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:05:39 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/07 16:32:58 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	print_usage(void)
{
	fprintf(stderr, "ERROR: input should be 8 arguments long.\n");
	fprintf(stderr, "Input order:\n -number_of_coders\n -time_to_burnout\n "
		"-time_to_compile\n -time_to_debug\n "
		"-time_to_refactor\n -number_of_compiles_required\n "
		"-dongle_cooldown\n -scheduler\n");
}

static bool	setup_sim(t_sim *sim)
{
	if (pthread_mutex_init(&sim->log_lock, NULL) != 0)
		return (false);
	gettimeofday(&sim->t0, NULL);
	sim->stopped = false;
	if (!init_sim(sim))
		return (false);
	return (true);
}

int	main(int argc, char **argv)
{
	t_sim	sim;

	if (argc != 9)
	{
		print_usage();
		return (EXIT_FAILURE);
	}
	if (parse_args(argv, &sim.config) || validate_args(&sim.config))
		return (1);
	if (!setup_sim(&sim))
		return (1);
	run_simulation(&sim);
	return (0);
}

