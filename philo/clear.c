/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 16:11:18 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/12 12:01:22 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_msg(char *s, int exit_no)
{
	printf("%s", s);
	return (exit_no);
}

void	*print_msg_null(char *s, t_cycle *cycle)
{
	if (cycle)
		free_cycle(cycle);
	print_msg(s, 1);
	return (NULL);
}

int	print_msg_err(char *s, t_cycle *cycle)
{
	if (cycle)
		free_cycle(cycle);
	return (print_msg(s, 1));
}

void	*free_cycle(t_cycle *cycle)
{
	int	i;

	if (!cycle)
		return (NULL);
	i = 0;
	if (cycle->philo)
	{
		while (i < cycle->nb_philo)
		{
			pthread_mutex_destroy(cycle->philo[i]->meal_lock);
			free(cycle->philo[i]->meal_lock);
			if (cycle->philo[i])
				free(cycle->philo[i]);
			i++;
		}
		free(cycle->philo);
	}
	free_mutex(cycle);
	free(cycle);
	return (NULL);
}

void	free_mutex(t_cycle *cycle)
{
	int	i;

	if (cycle->write_lock)
	{
		pthread_mutex_destroy(cycle->write_lock);
		free(cycle->write_lock);
	}
	if (cycle->fork_lock)
	{
		i = -1;
		while (++i < cycle->nb_philo)
			pthread_mutex_destroy(&cycle->fork_lock[i]);
		free(cycle->fork_lock);
	}
	if (cycle->stop_lock)
	{
		pthread_mutex_destroy(cycle->stop_lock);
		free(cycle->stop_lock);
	}
	if (cycle->ready_lock)
	{
		pthread_mutex_destroy(cycle->ready_lock);
		free(cycle->ready_lock);
	}
}
