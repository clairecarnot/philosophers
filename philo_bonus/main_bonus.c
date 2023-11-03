/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:20:36 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/19 15:05:56 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_cycle	*cycle;

	if (argc != 5 && argc != 6)
		return (printf("Invalid nb of arguments\n"), 1);
	if (check_args(argc, argv))
		return (1);
	cycle = cycle_init(argc, argv);
	if (!cycle)
		return (1);
	if (start_cycle(cycle))
		return (1);
	parent_process(cycle);
	return (0);
}

int	start_cycle(t_cycle *cycle)
{
	int	i;
	int	pid;

	if (cycle->nb_meals == 0)
		return (free_cycle(cycle), 1);
	cycle->start_time = get_time_ms();
	if (cycle->start_time < 0)
		return (print_msg_err("Start time init failed\n", cycle));
	cycle->last_meal = cycle->start_time;
	i = -1;
	while (++i < cycle->nb_philo)
	{
		pid = fork();
		if (pid == -1)
			handle_fork_fail(cycle, i);
		else if (pid == 0)
			child_process(cycle, i);
		else
			cycle->tabpid[i] = pid;
	}
	if (cycle->nb_philo > 1)
		create_monitors(cycle);
	return (0);
}

void	handle_fork_fail(t_cycle *cycle, int i)
{
	while (--i >= 0)
	{
		if (kill(cycle->tabpid[i], 0) == 0)
		{
			if (kill(cycle->tabpid[i], SIGKILL) == -1)
			{
				printf("Kill processes failed\n");
				exit(1);
			}
		}
	}
	printf("A fork failed\n");
	free_cycle(cycle);
	exit(1);
}

int	parent_process(t_cycle *cycle)
{
	if (cycle->nb_philo == 1)
	{
		waitpid(-1, NULL, 0);
		free_cycle(cycle);
		return (0);
	}
	while (1)
	{
		if (check_stop_flag(cycle))
			break ;
		usleep(1000);
	}
	pthread_join(cycle->death_check, NULL);
	pthread_join(cycle->meal_check, NULL);
	end_cycle(cycle);
	free_cycle(cycle);
	return (0);
}

int	end_cycle(t_cycle *cycle)
{
	int	i;

	i = 0;
	while (i < cycle->nb_philo)
	{
		if (kill(cycle->tabpid[i], 0) == 0)
		{
			if (kill(cycle->tabpid[i], SIGKILL) == -1)
				return (print_msg_err("Failed to kill a process\n", cycle));
		}
		i++;
	}
	return (0);
}
