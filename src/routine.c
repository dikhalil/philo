/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:14:31 by dikhalil          #+#    #+#             */
/*   Updated: 2025/09/29 15:39:13 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static int	thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->data_lock);
	if (philo->data->stop)
	{
		pthread_mutex_unlock(&philo->data->data_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->data_lock);
	print_status(philo, current_time_ms(philo->data), "is thinking");
	return (0);
}

static int	take_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->data_lock);
	if (philo->data->stop)
	{
		pthread_mutex_unlock(&philo->data->data_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->data_lock);
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
	return (0);
}

static int	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->data_lock);
	if (philo->data->stop ||
	(philo->data->num_of_meals != -1 && philo->meals_count >= philo->data->num_of_meals))
	{
		pthread_mutex_unlock(&philo->data->data_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->data_lock);
	if (philo->data->num_of_philos == 1)
	{
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		custom_usleep(philo, philo->data->time_to_die * 1000);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		return (1);
	}
	if (take_fork(philo))
		return (1);
	pthread_mutex_lock(&philo->data->data_lock);
	if (philo->data->stop || (philo->data->num_of_meals != -1 &&
		 philo->meals_count >= philo->data->num_of_meals))
	{
		pthread_mutex_unlock(&philo->data->data_lock);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		return (1);
	}
	philo->last_meal = get_time_ms();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->data->data_lock);
	print_status(philo, current_time_ms(philo->data), "is eating");
	custom_usleep(philo, philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	return (0);
}

static int	sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->data_lock);
	if (philo->data->stop)
	{
		pthread_mutex_unlock(&philo->data->data_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->data_lock);
	print_status(philo, current_time_ms(philo->data), "is sleeping");
	custom_usleep(philo, philo->data->time_to_sleep * 1000);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int stoped;

	philo = (t_philo *)arg;
	stoped = 0;
	while (1)
	{
		pthread_mutex_lock(&philo->data->data_lock);
		stoped = philo->data->stop;
		pthread_mutex_unlock(&philo->data->data_lock);
		if (stoped)
			break ;
		if (eating(philo))
			break;
		if  (sleeping(philo))
			break;
		if (thinking(philo))
			break;
	}
	return (NULL);
}
