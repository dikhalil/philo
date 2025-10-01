/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:01:10 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/01 10:11:43 by dikhalil         ###   ########.fr       */
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

typedef struct s_data
{
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_of_meals;
	long				start_time;
	int				stop;
	pthread_mutex_t	*forks;
	pthread_mutex_t	data_lock;
	pthread_mutex_t	print_lock;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				meals_count;
	long			last_meal;
	struct s_data	*data;
	pthread_t		thread;
}					t_philo;

/*---------- check ----------*/
int					check_args(int ac, char **av);
int					check_data(t_data *data, int flag);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_atoi(const char *str);

/*---------- initialize ----------*/
int					init_data(int ac, char **av, t_data *data);
int					create_philos(t_data *data, t_philo **philos);

/*---------- threads ----------*/
int					start_philo(t_philo *philos);
int					end_philo(t_philo *philos, int philo_count);
void				monitor_philos(t_philo *philos);
int	all_finished(t_philo *philos);

/*---------- mutexs ----------*/
int					init_mutex(t_data *data);
void				print_status(t_philo *philo, long current_time_ms,
						char *status);

/*---------- routine ----------*/
void				*philo_routine(void *arg);

/*---------- time ----------*/
long				get_time_ms(void);
long				current_time_ms(t_data *data);
void 				custom_usleep(t_philo *philo, long timemc);

/*---------- exit ----------*/
int					free_data(t_data *data, int mutex_count, int print_lock,
						int data_lock);
void				exit_program(t_data *data, t_philo *philos, char *msg,
						int status);
int is_simulation_stoped(t_philo *philo);

#endif
