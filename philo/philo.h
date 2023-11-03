/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:17:06 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/11 16:24:51 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_cycle	t_cycle;

typedef struct s_philo
{
	int				index;
	int				times_eaten;
	int				fork[2];
	pthread_t		thread;
	t_cycle			*cycle;
	time_t			last_meal;
	pthread_mutex_t	*meal_lock;
}		t_philo;

typedef struct s_cycle
{
	int				nb_philo;
	int				err;
	int				stop_flag;
	time_t			nb_meals;
	time_t			die_time;
	time_t			eat_time;
	time_t			sleep_time;
	time_t			start_time;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*fork_lock;
	pthread_mutex_t	*stop_lock;
	pthread_mutex_t	*ready_lock;
	t_philo			**philo;
}		t_cycle;

int		check_args(int argc, char **argv);
int		digits_check(char *s);
int		ft_atoi(char *s);

t_cycle	*cycle_init(int argc, char **argv);
int		create_mutex(t_cycle *cycle);
int		init_mutex(t_cycle *cycle);
int		philos_struct_init(t_cycle *cycle);
void	assign_forks(t_philo *philo, t_cycle *cycle);
int		start_cycle(t_cycle *cycle);
int		end_cycle(t_cycle *cycle);
int		stop_cycle(t_cycle *cycle);
int		check_situation(t_cycle *cycle);
int		has_someone_died(t_cycle *cycle, t_philo *philo);
int		philos_ate_enough(t_cycle *cycle);

int		print_msg(char *s, int exit_no);
int		print_msg_err(char *s, t_cycle *cycle);
void	*print_msg_null(char *s, t_cycle *cycle);
void	*free_cycle(t_cycle *cycle);
void	free_mutex(t_cycle *cycle);

void	*routine(void *arg);
void	*single_philo(t_philo *philo);
int		eat_sleep(t_philo *philo);
int		think(t_philo *philo, int msg);
time_t	get_time_ms(void);
int		action_time(t_philo *philo, time_t sleep_duration);
void	display_msg(t_philo *philo, char *s);

#endif
