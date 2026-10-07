/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:27:15 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/07 16:49:20 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	is_valid_digits(const char *str, int start)
{
	int	i;

	i = start;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

static bool	fits_in_int(const char *str, bool is_negative)
{
	size_t	len;
	int		start;

	start = (str[0] == '-' || str[0] == '+');
	len = strlen(str) - start;
	if (len > 10)
		return (false);
	if (len < 10)
		return (true);
	if (is_negative)
		return (strcmp(str + 1, "2147483648") <= 0);
	return (strcmp(str + start, "2147483647") <= 0);
}

int	is_valid_int(const char *str)
{
	bool	is_negative;
	int		start;

	is_negative = (str[0] == '-');
	start = (str[0] == '-' || str[0] == '+');
	if (!is_valid_digits(str, start))
		return (1);
	if (!fits_in_int(str, is_negative))
		return (1);
	return (0);
}
