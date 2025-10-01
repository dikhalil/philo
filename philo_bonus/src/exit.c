/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:12:26 by dikhalil          #+#    #+#             */
/*   Updated: 2025/09/28 20:35:19 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	free_data(t_data *data, int mutex_count, int print_lock, int data_lock)
{
	int	i;

	i = 0;
	if (data->forks)
	{
		while (i < mutex_count)
			if (pthread_mutex_destroy(&data->forks[i++]))
				return (1);
		free(data->forks);
	}
	if (print_lock)
		if (pthread_mutex_destroy(&data->print_lock))
			return (1);
	if (data_lock)
		if (pthread_mutex_destroy(&data->data_lock))
			return (1);
	return (0);
}

void	exit_program(t_data *data, t_philo *philos, char *msg, int status)
{
	if (data)
	{
		if (free_data(data, data->num_of_philos, 1, 1))
		{
			printf("Error: Mutex destroy failed\n");
			status = 1;
		}
	}
	if (philos)
		free(philos);
	if (msg)
		printf("%s\n", msg);
	exit(status);
}
