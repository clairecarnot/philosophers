/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:19:21 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/19 15:00:26 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	create_monitors(t_cycle *cycle)
{
	if (pthread_create(&cycle->death_check, NULL, &death_check, cycle) != 0)
	{
		(end_cycle(cycle), free_cycle(cycle));
		exit(1);
	}
	if (pthread_create(&cycle->meal_check, NULL, &meal_check, cycle) != 0)
	{
		stop_cycle(cycle);
		sem_post(cycle->dead);
		pthread_join(cycle->death_check, NULL);
		(end_cycle(cycle), free_cycle(cycle));
		exit(1);
	}
}

void	*death_check(void *arg)
{
	t_cycle	*cycle;

	cycle = (t_cycle *)arg;
	sem_wait(cycle->dead);
	if (!check_stop_flag(cycle))
	{
		stop_cycle(cycle);
		sem_post(cycle->eat);
	}
	return (NULL);
}

void	*meal_check(void *arg)
{
	t_cycle	*cycle;
	int		i;

	cycle = (t_cycle *)arg;
	i = -1;
	while (++i < cycle->nb_philo && !check_stop_flag(cycle))
		sem_wait(cycle->eat);
	if (!check_stop_flag(cycle))
	{
		stop_cycle(cycle);
		sem_post(cycle->dead);
	}
	return (NULL);
}

int	check_stop_flag(t_cycle *cycle)
{
	sem_wait(cycle->stop);
	if (cycle->stop_flag == 1)
	{
		sem_post(cycle->stop);
		return (1);
	}
	sem_post(cycle->stop);
	return (0);
}

void	stop_cycle(t_cycle *cycle)
{
	sem_wait(cycle->stop);
	cycle->stop_flag = 1;
	sem_post(cycle->stop);
}
