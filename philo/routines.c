/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 10:35:38 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/11 16:24:25 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	display_msg(t_philo *philo, char *s)
{
	pthread_mutex_lock(philo->cycle->write_lock);
	if (stop_cycle(philo->cycle) != 1)
		printf("%ld %d %s\n", get_time_ms() - philo->cycle->start_time,
			philo->index + 1, s);
	pthread_mutex_unlock(philo->cycle->write_lock);
}

int	eat_sleep(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->cycle->fork_lock[philo->fork[0]]) == 0)
		display_msg(philo, "has taken a fork");
	if (pthread_mutex_lock(&philo->cycle->fork_lock[philo->fork[1]]) == 0)
		display_msg(philo, "has taken a fork");
	display_msg(philo, "is eating");
	if (stop_cycle(philo->cycle) != 1)
	{
		pthread_mutex_lock(philo->meal_lock);
		philo->last_meal = get_time_ms();
		pthread_mutex_unlock(philo->meal_lock);
		action_time(philo, philo->cycle->eat_time);
		pthread_mutex_lock(philo->meal_lock);
		philo->times_eaten++;
		pthread_mutex_unlock(philo->meal_lock);
	}
	pthread_mutex_unlock(&philo->cycle->fork_lock[philo->fork[0]]);
	pthread_mutex_unlock(&philo->cycle->fork_lock[philo->fork[1]]);
	display_msg(philo, "is sleeping");
	action_time(philo, philo->cycle->sleep_time);
	return (0);
}

int	think(t_philo *philo, int msg)
{
	time_t	think_time;

	if (philo->cycle->nb_philo % 2)
		think_time = 2 * philo->cycle->eat_time
			- philo->cycle->sleep_time;
	else
		think_time = philo->cycle->eat_time
			- philo->cycle->sleep_time;
	if (msg)
		display_msg(philo, "is thinking");
	action_time(philo, think_time);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	pthread_mutex_lock(philo->cycle->ready_lock);
	pthread_mutex_unlock(philo->cycle->ready_lock);
	if (philo->cycle->err)
		return (NULL);
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal = philo->cycle->start_time;
	pthread_mutex_unlock(philo->meal_lock);
	if (philo->cycle->nb_philo == 1)
		return (single_philo(philo));
	else if (philo->index % 2)
		action_time(philo, philo->cycle->eat_time);
	while (stop_cycle(philo->cycle) != 1)
	{
		eat_sleep(philo);
		think(philo, 1);
	}
	return (NULL);
}

void	*single_philo(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->cycle->fork_lock[philo->fork[0]]) == 0)
		display_msg(philo, "has taken a fork");
	action_time(philo, philo->cycle->die_time);
	pthread_mutex_unlock(&philo->cycle->fork_lock[philo->fork[0]]);
	return (NULL);
}
