/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:11:23 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/04 11:44:10 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

static sem_t *sem_create(const char *name, int value, t_data *data)
{
	sem_t *sem;

	sem_unlink(name);
	sem = sem_open(name, O_CREAT, 0466, value);
	if (sem == SEM_FAILED)
		free_data(data, 1);
	return (sem);
}

static int sems_init(t_data *data)
{
	data->forks = sem_create("/forks", data->num_of_philos, data);
	if (data->forks == SEM_FAILED)
		return (1);
	data->eat = sem_create("/eat", data->num_of_philos / 2, data);
	if (data->eat == SEM_FAILED)
		return (1);
	data->print_lock = sem_create("/print", 1, data);
	if (data->print_lock == SEM_FAILED)
		return (1);
	return (0);
}

int	init_data(int ac, char **av, t_data *data)
{
	int flag;

	flag = 0;
	data->num_of_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
	{
		flag = 1;
		data->num_of_meals = ft_atoi(av[5]);
	}
	else
		data->num_of_meals = -1;
	data->start_time = 0;
	data->stop = 0;
	if (sems_init(data))
		return (1);
	return (check_data(data, flag));
}

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
		(*philos)[i].last_meal = 0;
		(*philos)[i].data = data;
		(*philos)[i].exit_status = -1;
		(*philos)[i].philos = *philos;
		if (pthread_mutex_init(&(*philos)[i].data_lock, NULL))
			return (1);
		i++;
	}
	return (0);
}

