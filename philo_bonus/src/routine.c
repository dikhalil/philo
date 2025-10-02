/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:14:31 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/02 20:06:40 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

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
	sem_wait(philo->data->forks);
	print_status(philo, current_time_ms(philo->data), "has taken fork");
	sem_wait(philo->data->forks);
	print_status(philo, current_time_ms(philo->data), "has taken fork");
	if (is_simulation_stoped(philo))
	{
		sem_post(philo->data->forks);
		sem_post(philo->data->forks);
		return (0);
	}
	return (1);
}

static void	eating(t_philo *philo)
{
	if (philo->data->num_of_philos == 1)
	{
		sem_wait(philo->data->forks);
		print_status(philo, current_time_ms(philo->data), "has taken fork");
		custom_usleep(philo, philo->data->time_to_die * 1000);
		sem_post(philo->data->forks);
		return ;
	}
	sem_wait(philo->data->eat);
	if (!take_fork(philo))
		return ;
	sem_wait(philo->data->data_lock);
	philo->last_meal = get_time_ms();
	philo->meals_count++;
	sem_post(philo->data->data_lock);
	print_status(philo, current_time_ms(philo->data), "is eating");
	custom_usleep(philo, philo->data->time_to_eat * 1000);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	sem_post(philo->data->eat);
}

static void	sleeping(t_philo *philo)
{
	if (is_simulation_stoped(philo))
		return ;
	print_status(philo, current_time_ms(philo->data), "is sleeping");
	custom_usleep(philo, philo->data->time_to_sleep * 1000);
}

void	philo_routine(t_philo	*philo)
{	
	pthread_create(&philo->monitor, NULL, monitor, philo);
	pthread_detach(philo->monitor);
	while (!is_simulation_stoped(philo))
	{
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	exit(1);
}

int is_simulation_stoped(t_philo *philo)
{
	sem_wait(philo->data->data_lock);
	if (get_time_ms() - philo->last_meal > philo->data->time_to_die)
	{
		if (!philo->data->stop)
		{
			philo->data->stop = 1;
			sem_post(philo->data->data_lock);
			print_status(philo, current_time_ms(philo->data), "died");
			return(1);
		}
	}
	sem_post(philo->data->data_lock);
	return (0);
}
