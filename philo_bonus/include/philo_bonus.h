/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:01:10 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/04 11:43:46 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

typedef struct s_data
{
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_of_meals;
	long			start_time;
	int				stop;
	sem_t			*forks;
	sem_t			*print_lock;
	sem_t			*eat;
}					t_data;

typedef struct s_philo
{
	int				id;
	pid_t 			pid;
	int				meals_count;
	long			last_meal;
	int exit_status;
	struct s_data	*data;
	struct s_philo	*philos;
	pthread_mutex_t data_lock;
	pthread_t		monitor;

}					t_philo;

/*---------- check ----------*/
int					check_args(int ac, char **av);
int					check_data(t_data *data, int flag);
int 				is_simulation_stopped(t_philo *philo);

/*---------- utils ----------*/
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_atoi(const char *str);
long				ft_atol(char *str);
void				print_status(t_philo *philo, long current_time_ms,
						char *status);

/*---------- initialize ----------*/
int					init_data(int ac, char **av, t_data *data);
int					create_philos(t_data *data, t_philo **philos);

/*---------- process ----------*/
int					start_philo(t_philo *philos);
int					end_philos(t_philo *philos, int philo_count);
void				*monitor(void *arg);

/*---------- routine ----------*/
void	philo_routine(t_philo	*philo);

/*---------- time ----------*/
long				get_time_ms(void);
long				current_time_ms(t_data *data);
void 				custom_usleep(t_philo *philo, long timemc);

/*---------- exit ----------*/
void free_data(t_data *data, int is_parent);
void				exit_program(t_data *data, t_philo *philos, char *msg,
						int status);
void exit_child(t_philo *philo);
#endif
