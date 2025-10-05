/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:11:23 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/05 18:02:54 by dikhalil         ###   ########.fr       */
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

int	init_data(int ac, char **av, t_data *data)
{
	int	flag;

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
	if (data->num_of_philos < 1)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_of_philos);
	if (!data->forks)
		return (1);
	if (init_mutex(data))
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
		(*philos)[i].left_fork = i;
		(*philos)[i].right_fork = (i + 1) % data->num_of_philos;
		(*philos)[i].last_meal = 0;
		(*philos)[i].data = data;
		i++;
	}
	return (0);
}
