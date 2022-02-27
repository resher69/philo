/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 15:06:27 by agardet           #+#    #+#             */
/*   Updated: 2022/02/26 14:46:27 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000.0);
}

uint64_t	get_time_since(const uint64_t time)
{
	uint64_t	t;

	t = get_time() - time;
	if (t < 0)
		return (0);
	return (t);
}

void	sleep_until(const uint64_t	time)
{
	while (get_time() < time)
		usleep(50);
}

uint64_t	ft_atoi(char *s)
{
	uint64_t	n;

	n = 0;
	if (*s == '-')
		return (0);
	while (s && *s >= '0' && *s <= '9' && n < UINT_MAX)
		n = (n * 10) + (*s++ - '0');
	if (n >= UINT_MAX)
		return (UINT_MAX);
	return (n);
}

uint8_t	ft_isnum(char *s)
{
	if (s && *s == '-')
		s++;
	while (s && *s >= '0' && *s <= '9')
		s++;
	if (*s)
		return (0);
	return (1);
}
