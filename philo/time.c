/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 11:55:35 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/12 10:42:12 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	action_time(t_philo *philo, time_t sleep_duration)
{
	time_t	end;

	end = get_time_ms() + sleep_duration;
	while (get_time_ms() <= end - 9)
	{
		if (stop_cycle(philo->cycle))
			return (1);
		usleep(9000);
	}
	while (get_time_ms() < end)
	{
		if (stop_cycle(philo->cycle))
			return (1);
		usleep(1000);
	}
	return (0);
}
