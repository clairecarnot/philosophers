/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:20:36 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/19 12:15:15 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	cycle->start_time = -1;
	cycle->times_eaten = 0;
	cycle->tabpid = NULL;
	unlink_sem(cycle);
	if (init_sems(cycle))
		return (NULL);
	if (tabpid_init(cycle))
		return (NULL);
	return (cycle);
}

int	init_sems(t_cycle *cycle)
{
	cycle->forks = sem_open(SFORKS, O_CREAT, S_IRUSR | S_IWUSR,
			cycle->nb_philo);
	if (cycle->forks == SEM_FAILED)
		return (printf("Open forks sem failed"), free_cycle(cycle), 1);
	cycle->eat = sem_open(SEAT, O_CREAT, S_IRUSR | S_IWUSR, 0);
	if (cycle->eat == SEM_FAILED)
		return (printf("Open eat sem failed"), free_cycle(cycle), 1);
	cycle->dead = sem_open(SDEAD, O_CREAT, S_IRUSR | S_IWUSR, 0);
	if (cycle->dead == SEM_FAILED)
		return (printf("Open dead sem failed"), free_cycle(cycle), 1);
	cycle->write = sem_open(SWRITE, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (cycle->write == SEM_FAILED)
		return (printf("Open write sem failed"), free_cycle(cycle), 1);
	cycle->stop = sem_open(SSTOP, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (cycle->stop == SEM_FAILED)
		return (printf("Open stop sem failed"), free_cycle(cycle), 1);
	return (0);
}

int	sem_create(t_cycle *cycle)
{
	cycle->forks = sem_open(SFORKS, O_RDWR);
	if (cycle->forks == SEM_FAILED)
		return (printf("Open forks sem failed"), free_cycle(cycle), 1);
	cycle->eat = sem_open(SEAT, O_RDWR);
	if (cycle->eat == SEM_FAILED)
		return (printf("Open eat sem failed"), free_cycle(cycle), 1);
	cycle->dead = sem_open(SDEAD, O_RDWR);
	if (cycle->dead == SEM_FAILED)
		return (printf("Open dead sem failed"), free_cycle(cycle), 1);
	cycle->write = sem_open(SWRITE, O_RDWR);
	if (cycle->write == SEM_FAILED)
		return (printf("Open write sem failed"), free_cycle(cycle), 1);
	cycle->stop = sem_open(SSTOP, O_RDWR);
	if (cycle->stop == SEM_FAILED)
		return (printf("Open stop sem failed"), free_cycle(cycle), 1);
	return (0);
}

int	close_sems(t_cycle *cycle)
{
	if (sem_close(cycle->forks) < 0)
		return (printf("Close forks sem failed"), 1);
	if (sem_close(cycle->eat) < 0)
		return (printf("Close eat sem failed"), 1);
	if (sem_close(cycle->dead) < 0)
		return (printf("Close dead sem failed"), 1);
	if (sem_close(cycle->write) < 0)
		return (printf("Close write sem failed"), 1);
	if (sem_close(cycle->stop) < 0)
		return (printf("Close stop sem failed"), 1);
	return (0);
}

int	tabpid_init(t_cycle *cycle)
{
	int	i;

	cycle->tabpid = malloc(sizeof(int) * cycle->nb_philo);
	if (!cycle->tabpid)
		return (printf("Bad malloc tabpid"), free_cycle(cycle), 1);
	i = -1;
	while (++i < cycle->nb_philo)
		cycle->tabpid[i] = 0;
	return (0);
}
