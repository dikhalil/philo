/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:11:23 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/03 07:34:38 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

int	init_data(int ac, char **av, t_data *data)
{
	int flag;

	flag = 0;
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/data");
	sem_unlink("/eat");
	sem_unlink("/stop");

	data->num_of_philos = atoi(av[1]);
	data->time_to_die = atoi(av[2]);
	data->time_to_eat = atoi(av[3]);
	data->time_to_sleep = atoi(av[4]);
	if (ac == 6)
	{
		flag = 1;
		data->num_of_meals = atoi(av[5]);
	}
	else
		data->num_of_meals = -1;
	data->start_time = 0;
	data->stop = 0;
	data->forks = sem_open("/forks", O_CREAT, 0466, data->num_of_philos);
	if (data->forks == SEM_FAILED)
		return (1);
	data->print_lock = sem_open("/print", O_CREAT, 0466, 1);
	if (data->print_lock == SEM_FAILED)
		return (free_data(data, 1, 0, 0,0,0,1));
	data->data_lock = sem_open("/data", O_CREAT, 0466, 1);
	if (data->data_lock == SEM_FAILED)
		return(free_data(data, 1, 1, 0,0,0, 1));
	data->eat = sem_open("/eat", O_CREAT, 0466, data->num_of_philos / 2);
	if (data->eat == SEM_FAILED)
		return(free_data(data, 1, 1, 1,0,0,1));
	data->sem_stop = sem_open("/stop", O_CREAT, 0466, 1);
	if (data->sem_stop == SEM_FAILED)
		return(free_data(data, 1, 1, 1,1,0,1));
	return (check_data(data, flag));
}
