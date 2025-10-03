/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:26:32 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/03 17:31:42 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	start_philo(t_philo *philos)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&philos[0].data->data_lock);
	philos[0].data->start_time = get_time_ms();
	while (i < philos[0].data->num_of_philos)
		philos[i++].last_meal = philos[0].data->start_time;
	pthread_mutex_unlock(&philos[0].data->data_lock);
	i = 0;
	while (i < philos[0].data->num_of_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]))
		{
			pthread_mutex_lock(&philos[0].data->data_lock);
			philos[0].data->stop = 1;
			pthread_mutex_unlock(&philos[0].data->data_lock);
			end_philo(philos, i);
			return (1);
		}
		i++;
	}
	return (0);
}

int	end_philo(t_philo *philos, int philo_count)
{
	int	status;

	status = 0;
	if (philo_count != philos[0].data->num_of_philos)
		status = 1;
	while (philo_count > 0)
	{
		philo_count--;
		if (pthread_join(philos[philo_count].thread, NULL))
			return (1);
	}
	return (status);
}

void	monitor_philos(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].data->num_of_philos)
	{
		pthread_mutex_lock(&philos->data->data_lock);
		if ((get_time_ms() - philos[i].last_meal) >= philos[0].data->time_to_die)
		{
			philos[i].data->stop = 1;
			pthread_mutex_unlock(&philos->data->data_lock);
			print_status(&philos[i], current_time_ms(philos->data), "died");
			return ;
		}
		pthread_mutex_unlock(&philos->data->data_lock);
		i++;
	}
	if (philos[0].data->num_of_meals != -1 && all_finished(philos))
	{
		pthread_mutex_lock(&philos->data->data_lock);
		philos[0].data->stop = 1;
		pthread_mutex_unlock(&philos->data->data_lock);
		return ;
	}
	usleep(100);
}
