/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:47:38 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/07 16:48:52 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parse_scheduler(const char *arg, t_scheduler *out)
{
	if (strcmp(arg, "fifo") == 0)
	{
		*out = fifo;
		return (0);
	}
	if (strcmp(arg, "edf") == 0)
	{
		*out = edf;
		return (0);
	}
	fprintf(stderr, "ERROR: Invalid scheduler. Must be \"fifo\" or \"edf\"\n");
	return (1);
}

int	parse_args(char **argv, t_config *config)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (is_valid_int(argv[i]))
		{
			fprintf(stderr, "ERROR: Invalid numeric argument. %s\n", argv[i]);
			return (1);
		}
		i++;
	}
	config->number_of_coders = atoi(argv[1]);
	config->time_to_burnout = atoi(argv[2]);
	config->time_to_compile = atoi(argv[3]);
	config->time_to_debug = atoi(argv[4]);
	config->time_to_refactor = atoi(argv[5]);
	config->number_of_compiles_required = atoi(argv[6]);
	config->dongle_cooldown = atoi(argv[7]);
	if (parse_scheduler(argv[8], &config->scheduler) != 0)
		return (1);
	return (0);
}

int	validate_args(t_config *config)
{
	if (config->number_of_coders < 1 || config->number_of_coders > 100000)
	{
		fprintf(stderr, "ERROR: Invalid number of coders."
			" Range = [1-100000]\n");
		return (1);
	}
	if (config->time_to_burnout <= 0 || config->time_to_compile <= 0
		|| config->time_to_debug <= 0 || config->time_to_refactor <= 0
		|| config->dongle_cooldown < 0)
	{
		fprintf(stderr, "ERROR: Invalid time. Range = [0-INT_MAX]\n");
		return (1);
	}
	if (config->number_of_compiles_required < 0)
	{
		fprintf(stderr, "ERROR: Invalid number of compiles."
			" Range = [0-INT_MAX]\n");
		return (1);
	}
	return (0);
}
