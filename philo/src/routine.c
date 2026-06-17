/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:14:31 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/04 09:50:21 by dikhalil         ###   ########.fr       */
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
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		print_status(philo, current_time_ms(philo->data), "has taken fork");
		custom_usleep(philo, philo->data->time_to_die);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		return ;
	}
	take_fork(philo);
	if (is_simulation_stopped(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		return ;
	}
	pthread_mutex_lock(&philo->data->data_lock);
	philo->last_meal = get_time_ms();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->data->data_lock);
	print_status(philo, current_time_ms(philo->data), "is eating");
	custom_usleep(philo, philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
}

static void	sleeping(t_philo *philo)
{
	print_status(philo, current_time_ms(philo->data), "is sleeping");
	custom_usleep(philo, philo->data->time_to_sleep);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!is_simulation_stopped(philo))
	{
		if (philo->id % 2)
			usleep(50);
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}
