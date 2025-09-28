/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:14:31 by dikhalil          #+#    #+#             */
/*   Updated: 2025/09/28 20:50:13 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	thinking(t_philo *philo)
{
	print_status(philo, current_time_ms(philo->data), "is thinking");
}

static void	take_fork(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		print_status(philo, current_time_ms(philo->data), "has taken fork");
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		print_status(philo, current_time_ms(philo->data), "has taken fork");
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		print_status(philo, current_time_ms(philo->data), "has taken fork");
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		print_status(philo, current_time_ms(philo->data), "has taken fork");
	}
}

static void	eating(t_philo *philo)
{
	if (philo->data->num_of_philos == 1)
	{
		take_leftfork(philo);
		usleep(philo->data->time_to_die * 1000);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		return ;
	}
	take_fork(philo);
	pthread_mutex_lock(&philo->data->data_lock);
	philo->last_meal = get_time_ms();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->data->data_lock);
	print_status(philo, current_time_ms(philo->data), "is eating");
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
}

static void	sleeping(t_philo *philo)
{
	print_status(philo, current_time_ms(philo->data), "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->data->data_lock);
		if (philo->data->stop)
		{
			pthread_mutex_unlock(&philo->data->data_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->data->data_lock);
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}
