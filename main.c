/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:05:39 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/01 18:04:08 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int is_valid_int(const char *str)
{
	int		is_negative;
	size_t	len;
	int		i;

	i = 0;
	is_negative = 0;
	len = strlen(str);
	if (str[0] == '-' || str[0] == '+')
	{
		is_negative = (str[0] == '-');
		len--;
		i++;
	}
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	if (len > 10 || len == 0)
		return (1);
	if (len < 10)
		return (0);
	if (is_negative)
	{
		if (strcmp(str + 1, "2147483648") > 0)
			return (1);
	}
	else
	{
		if (strcmp(str + (str[0] == '+'), "2147483647") > 0)
			return (1);
	}
	return (0);
}

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
    int i;

    i = 1;
    while (i < 8){
        if (is_valid_int(argv[i])) {
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

int validate_args (t_config *config)
{
    
    if (config->number_of_coders <1 || config->number_of_coders > 200){
        fprintf(stderr, "ERROR: Invalid number of coders.   Range = [1-200]\n");
        return (1);
    }
    if (config->time_to_burnout <= 0 || config->time_to_compile <= 0 || 
        config->time_to_debug <= 0 || config->time_to_refactor <= 0 || 
        config->dongle_cooldown < 0){
            fprintf(stderr, "ERROR: Invalid time.   Range = [0-INT_MAX]\n");
            return (1);
    }
    if (config->number_of_compiles_required < 0 ){
        fprintf(stderr, "ERROR: Invalid number of compiles.   Range = [0-INT_MAX]\n");
        return (1);
    }
    return (0);
}

int main(int argc, char **argv)
{
    t_thread_arg	*t_arg;
    t_sim           sim;
    int             i;

    i = 0;
    if (argc != 9){
        fprintf(stderr,"ERROR: input should be 8 arguments long.\n");
        fprintf(stderr,"Input order:\n -number_of_coders\n -time_to_burnout\n "
                "-time_to_compile\n -time_to_debug\n "
                "-time_to_refactor\n -number_of_compiles_required\n "
                "-dongle_cooldown\n -scheduler\n");
        return(EXIT_FAILURE);   
    }
    if (parse_args(argv, &sim.config))
        return (1);
    if (validate_args(&sim.config))
        return (1);

    //TEST
    if (pthread_mutex_init(&sim.log_lock, NULL) != 0)
        return (1);
    gettimeofday(&sim.t0, NULL);
    sim.stopped = false;
    if (!init_sim(&sim))
		return (1);
    t_arg = malloc(sizeof(t_thread_arg) * sim.config.number_of_coders);
    if (!t_arg)
    {
        destroy_sim(&sim);
        return(1);
    }
    while (i < sim.config.number_of_coders)
    {
        t_arg[i].sim = &sim;
	    t_arg[i].coder_id = i;
        pthread_create(&sim.coders[i].thread, NULL, coder_routine, &t_arg[i]);
        i++;
    }
    i = 0;
    while (i < sim.config.number_of_coders)
    {
        pthread_join(sim.coders[i].thread, NULL);
        i++;
    }
    free(t_arg);
	destroy_sim(&sim);
    //END TEST
    return (0);
}

