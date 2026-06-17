/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:14:31 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/04 14:07:07 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

static void	thinking(t_philo *philo)
{
	print_status(philo, current_time_ms(philo->data), "is thinking");
}

static void	take_fork(t_philo *philo)
{
	sem_wait(philo->data->forks);
	print_status(philo, current_time_ms(philo->data), "has taken fork");
	sem_wait(philo->data->forks);
	print_status(philo, current_time_ms(philo->data), "has taken fork");
	if (is_simulation_stopped(philo))
	{
		sem_post(philo->data->forks);
		sem_post(philo->data->forks);
	}
}

static void	eating(t_philo *philo)
{
	if (philo->data->num_of_philos == 1)
	{
		sem_wait(philo->data->forks);
		print_status(philo, current_time_ms(philo->data), "has taken fork");
		custom_usleep(philo, philo->data->time_to_die);
		sem_post(philo->data->forks);
		return ;
	}
	sem_wait(philo->data->eat);
	take_fork(philo);
	if (is_simulation_stopped(philo))
	{
		sem_post(philo->data->forks);
		sem_post(philo->data->forks);
		sem_post(philo->data->eat);
		return ;
	}
	pthread_mutex_lock(&philo->data_lock);
	philo->last_meal = get_time_ms();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->data_lock);
	print_status(philo, current_time_ms(philo->data), "is eating");
	custom_usleep(philo, philo->data->time_to_eat);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	sem_post(philo->data->eat);
}

static void	sleeping(t_philo *philo)
{
	print_status(philo, current_time_ms(philo->data), "is sleeping");
	custom_usleep(philo, philo->data->time_to_sleep);
}

void	philo_routine(t_philo	*philo)
{
	pthread_create(&philo->monitor, NULL, monitor, philo);
	while (!is_simulation_stopped(philo))
	{
		if (philo->id % 2)
			usleep (50);
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	pthread_join(philo->monitor, NULL);
	exit_child(philo);
}
