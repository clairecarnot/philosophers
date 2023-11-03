/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 16:11:18 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/19 11:05:49 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	if (!cycle)
		return (NULL);
	close_sems(cycle);
	unlink_sem(cycle);
	if (cycle->tabpid)
		free(cycle->tabpid);
	free(cycle);
	return (NULL);
}

void	unlink_sem(t_cycle *cycle)
{
	if (cycle)
	{
		sem_unlink(SFORKS);
		sem_unlink(SEAT);
		sem_unlink(SDEAD);
		sem_unlink(SWRITE);
		sem_unlink(SSTOP);
	}
}
