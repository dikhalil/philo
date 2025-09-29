/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:28:04 by dikhalil          #+#    #+#             */
/*   Updated: 2025/09/29 15:19:57 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
		{
			free_data(data, i, 0, 0);
			return (1);
		}
		i++;
	}
	if (pthread_mutex_init(&data->print_lock, NULL))
	{
		free_data(data, i, 0, 0);
		return (1);
	}
	if (pthread_mutex_init(&data->data_lock, NULL))
	{
		free_data(data, i, 1, 0);
		return (1);
	}
	return (0);
}

void	print_status(t_philo *philo, long current_time_ms, char *status)
{
	pthread_mutex_lock(&philo->data->data_lock);
	if (!philo->data->stop || !ft_strncmp("died", status, 4))
	{
	    pthread_mutex_lock(&philo->data->print_lock);
	    printf("%ld %d %s\n", current_time_ms, philo->id, status);
	    pthread_mutex_unlock(&philo->data->print_lock);
	}	
	pthread_mutex_unlock(&philo->data->data_lock);
}
