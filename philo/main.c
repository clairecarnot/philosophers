/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:20:36 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/12 12:31:54 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_cycle	*cycle;

	if (argc != 5 && argc != 6)
		return (print_msg("Invalid nb of arguments\n", 1));
	if (check_args(argc, argv))
		return (1);
	cycle = cycle_init(argc, argv);
	if (!cycle)
		return (1);
	if (start_cycle(cycle))
		return (1);
	while (stop_cycle(cycle) != 1)
		check_situation(cycle);
	if (end_cycle(cycle))
		return (1);
	free_cycle(cycle);
	return (0);
}

int	start_cycle(t_cycle *cycle)
{
	int	i;

	if (pthread_mutex_lock(cycle->ready_lock) != 0)
		return (print_msg_err("Ready lock failed\n", cycle));
	i = -1;
	while (++i < cycle->nb_philo)
	{
		if (pthread_create(&cycle->philo[i]->thread,
				NULL, &routine, cycle->philo[i]) != 0)
		{
			cycle->err = 1;
			if (pthread_mutex_unlock(cycle->ready_lock) != 0)
				return (print_msg_err("Unlock failed - exit all\n", cycle));
			while (--i >= 0)
				if (pthread_join(cycle->philo[i]->thread, NULL) != 0)
					return (print_msg_err("Create/join thrds failed\n", cycle));
			return (print_msg_err("Create thread failed\n", cycle));
		}
	}
	cycle->start_time = get_time_ms();
	if (cycle->start_time < 0)
		return (printf("No time\n"), end_cycle(cycle), free_cycle(cycle), 1);
	if (pthread_mutex_unlock(cycle->ready_lock) != 0)
		return (printf("Not ready\n"), end_cycle(cycle), free_cycle(cycle), 1);
	return (0);
}

int	stop_cycle(t_cycle *cycle)
{
	pthread_mutex_lock(cycle->stop_lock);
	if (cycle->stop_flag)
	{
		pthread_mutex_unlock(cycle->stop_lock);
		return (1);
	}
	pthread_mutex_unlock(cycle->stop_lock);
	return (0);
}

int	end_cycle(t_cycle *cycle)
{
	int	i;

	i = 0;
	while (i < cycle->nb_philo)
	{
		if (pthread_join(cycle->philo[i]->thread, NULL) != 0)
			return (print_msg_err("Failed to join a thread\n", cycle));
		i++;
	}
	return (0);
}
