/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:14:31 by dikhalil          #+#    #+#             */
/*   Updated: 2025/09/30 19:02:56 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	thinking(t_philo *philo)
{
	if (is_simulation_stoped(philo))
		return ;
	print_status(philo, current_time_ms(philo->data), "is thinking");
}

static int	take_fork(t_philo *philo)
{
	if (is_simulation_stoped(philo))
		return (0);
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
	if (is_simulation_stoped(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		return (0);
	}
	return (1);
}

static void	eating(t_philo *philo)
{
	if (philo->data->num_of_philos == 1)
	{
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		custom_usleep(philo, philo->data->time_to_die * 1000);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		return ;
	}
	if (!take_fork(philo))
		return ;
	pthread_mutex_lock(&philo->data->data_lock);
	philo->last_meal = get_time_ms();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->data->data_lock);
	print_status(philo, current_time_ms(philo->data), "is eating");
	custom_usleep(philo, philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
}

static void	sleeping(t_philo *philo)
{
	if (is_simulation_stoped(philo))
		return ;
	print_status(philo, current_time_ms(philo->data), "is sleeping");
	custom_usleep(philo, philo->data->time_to_sleep * 1000);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!is_simulation_stoped(philo))
	{
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}

int is_simulation_stoped(t_philo *philo)
{
	int stoped;

	stoped = 1;
	if (get_time_ms() - philo->last_meal > philo->data->time_to_die)
		return (stoped);
	pthread_mutex_lock(&philo->data->data_lock);
	stoped = philo->data->stop;
	pthread_mutex_unlock(&philo->data->data_lock);
	return (stoped);
}
