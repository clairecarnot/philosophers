/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:20:36 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/05 14:43:41 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_cycle	*cycle_init(int argc, char **argv)
{
	t_cycle	*cycle;

	cycle = malloc(sizeof(t_cycle) * 1);
	if (!cycle)
		return (print_msg_null("Bad malloc cycle\n", cycle));
	cycle->nb_philo = ft_atoi(argv[1]);
	cycle->die_time = ft_atoi(argv[2]);
	cycle->eat_time = ft_atoi(argv[3]);
	cycle->sleep_time = ft_atoi(argv[4]);
	cycle->nb_meals = -1;
	if (argc == 6)
		cycle->nb_meals = ft_atoi(argv[5]);
	cycle->stop_flag = 0;
	cycle->err = 0;
	cycle->write_lock = NULL;
	cycle->fork_lock = NULL;
	cycle->stop_lock = NULL;
	cycle->ready_lock = NULL;
	cycle->start_time = -1;
	if (create_mutex(cycle))
		return (NULL);
	if (philos_struct_init(cycle))
		return (NULL);
	return (cycle);
}

int	create_mutex(t_cycle *cycle)
{
	cycle->write_lock = malloc(sizeof(pthread_mutex_t) * 1);
	if (!cycle->write_lock)
		return (print_msg_err("Bad malloc mutex write", cycle));
	cycle->fork_lock = malloc(sizeof(pthread_mutex_t) * cycle->nb_philo);
	if (!cycle->fork_lock)
		return (print_msg_err("Bad malloc mutex fork", cycle));
	cycle->stop_lock = malloc(sizeof(pthread_mutex_t) * 1);
	if (!cycle->stop_lock)
		return (print_msg_err("Bad malloc mutex stop", cycle));
	cycle->ready_lock = malloc(sizeof(pthread_mutex_t) * 1);
	if (!cycle->ready_lock)
		return (print_msg_err("Bad malloc mutex ready", cycle));
	if (init_mutex(cycle))
		return (1);
	return (0);
}

int	init_mutex(t_cycle *cycle)
{
	int	i;

	if (pthread_mutex_init(cycle->write_lock, NULL) != 0)
		return (print_msg_err("Init error mutex write", cycle));
	i = 0;
	while (i < cycle->nb_philo)
	{
		if (pthread_mutex_init(&cycle->fork_lock[i], NULL) != 0)
			return (print_msg_err("Init error mutex fork", cycle));
		i++;
	}
	if (pthread_mutex_init(cycle->stop_lock, NULL) != 0)
		return (print_msg_err("Init error mutex stop", cycle));
	if (pthread_mutex_init(cycle->ready_lock, NULL) != 0)
		return (print_msg_err("Init error mutex ready", cycle));
	return (0);
}

int	philos_struct_init(t_cycle *cycle)
{
	int	i;

	cycle->philo = malloc(sizeof(t_philo *) * cycle->nb_philo);
	if (!cycle->philo)
		return (print_msg_err("Bad malloc **philos\n", cycle));
	i = -1;
	while (++i < cycle->nb_philo)
		cycle->philo[i] = NULL;
	i = -1;
	while (++i < cycle->nb_philo)
	{
		cycle->philo[i] = malloc(sizeof(t_philo) * 1);
		if (!cycle->philo[i])
			return (print_msg_err("Bad malloc philos\n", cycle));
		cycle->philo[i]->index = i;
		cycle->philo[i]->cycle = cycle;
		assign_forks(cycle->philo[i], cycle);
		cycle->philo[i]->meal_lock = NULL;
		cycle->philo[i]->meal_lock = malloc(sizeof(pthread_mutex_t) * 1);
		if (!cycle->philo[i]->meal_lock)
			return (print_msg_err("Bad malloc mutex meal", cycle));
		if (pthread_mutex_init(cycle->philo[i]->meal_lock, NULL) != 0)
			return (print_msg_err("Init error mutex meal", cycle));
	}
	return (0);
}

void	assign_forks(t_philo *philo, t_cycle *cycle)
{
	philo->times_eaten = 0;
	philo->last_meal = 0;
	if (philo->index % 2 != 0)
	{
		philo->fork[0] = philo->index;
		if (philo->index == cycle->nb_philo - 1)
			philo->fork[1] = 0;
		else
			philo->fork[1] = philo->index + 1;
	}
	else
	{
		if (philo->index == cycle->nb_philo - 1)
			philo->fork[0] = 0;
		else
			philo->fork[0] = philo->index + 1;
		philo->fork[1] = philo->index;
	}
}
