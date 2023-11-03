/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:19:21 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/12 12:26:48 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_situation(t_cycle *cycle)
{
	int	i;
	int	all_ate;

	if (cycle->nb_meals == 0)
		return (philos_ate_enough(cycle));
	all_ate = 1;
	i = -1;
	while (++i < cycle->nb_philo)
	{
		pthread_mutex_lock(cycle->philo[i]->meal_lock);
		if (cycle->philo[i]->last_meal == 0)
			cycle->philo[i]->last_meal = cycle->start_time;
		if (has_someone_died(cycle, cycle->philo[i]))
			return (pthread_mutex_unlock(cycle->philo[i]->meal_lock), 0);
		if (cycle->philo[i]->times_eaten < cycle->nb_meals)
			all_ate = 0;
		pthread_mutex_unlock(cycle->philo[i]->meal_lock);
	}
	if (cycle->nb_meals != -1 && all_ate == 1)
		return (philos_ate_enough(cycle));
	usleep(1000);
	return (0);
}

int	has_someone_died(t_cycle *cycle, t_philo *philo)
{
	if (get_time_ms() - philo->last_meal >= cycle->die_time)
	{
		pthread_mutex_lock(cycle->stop_lock);
		cycle->stop_flag = 1;
		pthread_mutex_unlock(cycle->stop_lock);
		pthread_mutex_lock(philo->cycle->write_lock);
		printf("%ld %d %s\n", get_time_ms() - philo->cycle->start_time,
			philo->index + 1, "has died");
		pthread_mutex_unlock(philo->cycle->write_lock);
		return (1);
	}
	return (0);
}

int	philos_ate_enough(t_cycle *cycle)
{
	pthread_mutex_lock(cycle->stop_lock);
	cycle->stop_flag = 1;
	pthread_mutex_unlock(cycle->stop_lock);
	return (0);
}
