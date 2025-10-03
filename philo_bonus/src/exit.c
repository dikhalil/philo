/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:12:26 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/02 22:50:36 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

int	free_data(t_data *data, int forks, int print_lock, int data_lock, int eat,int stop, int status)
{
	if (forks)
	{
		sem_close(data->forks);
		sem_unlink("/forks");
	}
	if (print_lock)
	{
		sem_close(data->print_lock);
		sem_unlink("/print");
	}
	if (data_lock)
	{
		sem_close(data->data_lock);
		sem_unlink("/data");
	}
	if (eat)
	{
		sem_close(data->eat);
		sem_unlink("/eat");
	}
	if (stop)
	{
		sem_close(data->sem_stop);
		sem_unlink("/stop");
	}
	return (status);
}

void	exit_program(t_data *data, t_philo *philos, char *msg, int status)
{
	if (data)
		free_data(data, 1, 1, 1, 1,1,status);
	if (philos)
		free(philos);
	if (msg)
		printf("%s\n", msg);
	exit(status);
}
