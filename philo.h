/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:01:10 by dikhalil          #+#    #+#             */
/*   Updated: 2025/09/25 19:42:05 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>

typedef stuct s_data
{
    int num_of_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int num_of_meals;
    int start_time;
    int stop;
    mutex_t *forks;
    mutex_t print_lock;
}               t_data;

typedef struct s_philo
{
    int id;
    int left_fork;
    int right_fork;
    int meals_count;
    long last_meal;
    struct s_data *data;
    pthread_t thread;
}               t_philo;

#endif

