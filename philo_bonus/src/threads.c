/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:26:32 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/01 10:32:25 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	create_philos(t_data *data, t_philo **philos)
{
	int	i;

	i = 0;
	*philos = malloc(sizeof(t_philo) * data->num_of_philos);
	if (!*philos)
		return (1);
	while (i < data->num_of_philos)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].meals_count = 0;
		(*philos)[i].left_fork = i;
		(*philos)[i].right_fork = (i + 1) % data->num_of_philos;
		(*philos)[i].last_meal = 0;
		(*philos)[i].data = data;
		i++;
	}
	return (0);
}

int	start_philo(t_philo *philos)
{
	int	i;
	pid_t pid;
	
	i = 0;
	pid = 0;
	//pthread_mutex_lock(&philos[0].data->data_lock);
	philos[0].data->start_time = get_time_ms();
	while (i < philos[0].data->num_of_philos)
	{
		philos[i].last_meal = philos[0].data->start_time;
		i++;
	}
	//pthread_mutex_unlock(&philos[0].data->data_lock);
	i = 0;
	while (i < philos[0].data->num_of_philos)
	{
		pid = fork();
		if (pid == -1)
		{
			//pthread_mutex_lock(&philos[0].data->data_lock);
			//philos[0].data->stop = 1;
			//pthread_mutex_unlock(&philos[0].data->data_lock);
			//end_philo(philos, i);
			//return (1);
		}
		if (pid == 0)
			philo_routine(&philos[i]);
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

 int	all_finished(t_philo *philos)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&philos->data->data_lock);
	while (i < philos[0].data->num_of_philos)
	{
		if (philos[i].meals_count < philos[i].data->num_of_meals)
		{
			pthread_mutex_unlock(&philos->data->data_lock);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&philos->data->data_lock);
	return (1);
}

void	monitor_philos(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].data->num_of_philos)
	{
		pthread_mutex_lock(&philos->data->data_lock);
		if ((get_time_ms() - philos[i].last_meal) > philos[0].data->time_to_die)
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
