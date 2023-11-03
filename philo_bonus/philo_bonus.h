/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:17:06 by ccarnot           #+#    #+#             */
/*   Updated: 2023/10/19 11:06:19 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

# define SFORKS "/semaphore_forks"
# define SWRITE "/semaphore_write"
//# define SREADY "/semaphore_ready"
# define SEAT "/semaphore_eat"
# define SDEAD "/semaphore_dead"
# define SSTOP "/semaphore_stop"

typedef struct s_cycle
{
	int				nb_philo;
	int				stop_flag;
	int				times_eaten;
	time_t			nb_meals;
	time_t			die_time;
	time_t			eat_time;
	time_t			sleep_time;
	time_t			start_time;
	time_t			last_meal;
	sem_t			*forks;
	sem_t			*write;
	sem_t			*eat;
	sem_t			*dead;
	sem_t			*stop;
	int				*tabpid;
	pthread_t		death_check;
	pthread_t		meal_check;
}		t_cycle;

int		check_args(int argc, char **argv);
int		digits_check(char *s);
int		ft_atoi(char *s);

t_cycle	*cycle_init(int argc, char **argv);
int		sem_create(t_cycle *cycle);
int		init_sems(t_cycle *cycle);
int		close_sems(t_cycle *cycle);
int		tabpid_init(t_cycle *cycle);
int		start_cycle(t_cycle *cycle);
int		end_cycle(t_cycle *cycle);
int		parent_process(t_cycle *cycle);
void	handle_fork_fail(t_cycle *cycle, int i);
void	create_monitors(t_cycle *cycle);
void	*death_check(void *arg);
void	*meal_check(void *arg);
int		check_stop_flag(t_cycle *cycle);
void	stop_cycle(t_cycle *cycle);

int		print_msg(char *s, int exit_no);
int		print_msg_err(char *s, t_cycle *cycle);
void	*print_msg_null(char *s, t_cycle *cycle);
void	*free_cycle(t_cycle *cycle);
void	unlink_sem(t_cycle *cycle);

void	*child_process(t_cycle *cycle, int i);
void	single_philo(t_cycle *cycle, int i);
int		eat_sleep(t_cycle *cycle, int i);
int		think(t_cycle *cycle, int i);
time_t	get_time_ms(void);
int		action_time(time_t sleep_duration, int i, t_cycle *cycle);
int		am_i_dead(t_cycle *cycle, int i);
int		send_flag(t_cycle *cycle, int i);
void	display_msg(char *s, int i, t_cycle *cycle);

#endif
