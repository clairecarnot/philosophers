/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 10:35:38 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/19 15:00:49 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	display_msg(char *s, int i, t_cycle *cycle)
{
	sem_wait(cycle->write);
	if (!am_i_dead(cycle, i) && !check_stop_flag(cycle))
		printf("%ld %d %s\n", get_time_ms()
			- cycle->start_time, i + 1, s);
	sem_post(cycle->write);
}

int	eat_sleep(t_cycle *cycle, int i)
{
	send_flag(cycle, i);
	if (!am_i_dead(cycle, i) && !check_stop_flag(cycle))
		(sem_wait(cycle->forks),
			display_msg("has taken a fork", i, cycle));
	send_flag(cycle, i);
	if (!am_i_dead(cycle, i) && !check_stop_flag(cycle))
		(sem_wait(cycle->forks),
			display_msg("has taken a fork", i, cycle));
	display_msg("is eating", i, cycle);
	if (!am_i_dead(cycle, i) && !check_stop_flag(cycle))
	{
		cycle->last_meal = get_time_ms();
		action_time(cycle->eat_time, i, cycle);
		cycle->times_eaten++;
		if (cycle->times_eaten == cycle->nb_meals)
			sem_post(cycle->eat);
	}
	sem_post(cycle->forks);
	sem_post(cycle->forks);
	send_flag(cycle, i);
	display_msg("is sleeping", i, cycle);
	action_time(cycle->sleep_time, i, cycle);
	return (0);
}

int	think(t_cycle *cycle, int i)
{
	time_t	think_time;

	if (cycle->nb_philo % 2)
		think_time = 2 * cycle->eat_time - cycle->sleep_time;
	else
		think_time = cycle->eat_time - cycle->sleep_time;
	send_flag(cycle, i);
	display_msg("is thinking", i, cycle);
	send_flag(cycle, i);
	action_time(think_time, i, cycle);
	return (0);
}

void	*child_process(t_cycle *cycle, int i)
{
	if (cycle->nb_philo == 1)
		single_philo(cycle, i);
	else
	{
		if (i % 2)
			action_time(cycle->eat_time, i, cycle);
		while (1)
		{
			if (am_i_dead(cycle, i) || check_stop_flag(cycle))
				send_flag(cycle, i);
			eat_sleep(cycle, i);
			if (am_i_dead(cycle, i) || check_stop_flag(cycle))
				send_flag(cycle, i);
			think(cycle, i);
		}
	}
	return (NULL);
}

void	single_philo(t_cycle *cycle, int i)
{
	if (sem_wait(cycle->forks) == 0)
		display_msg("has taken a fork", i, cycle);
	usleep(cycle->die_time * 1000);
	printf("%ld %d has died\n", get_time_ms() - cycle->start_time,
		i + 1);
	sem_post(cycle->forks);
	free_cycle(cycle);
	exit(0);
}
