/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:09:47 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/04 09:40:04 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

static int	is_number(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[i])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_args(int ac, char **av)
{
	int		i;
	long	num;

	i = 1;
	while (i < ac)
	{
		if (!is_number(av[i]))
			return (1);
		num = ft_atol(av[i]);
		if (num > INT_MAX || num < INT_MIN)
			return (1);
		i++;
	}
	return (0);
}

int	check_data(t_data *data, int flag)
{
	if (data->num_of_philos < 1)
		return (1);
	if (data->time_to_die < 0 || data->time_to_eat < 0
		|| data->time_to_sleep < 0)
		return (1);
	if (data->num_of_meals <= -1 && flag)
		return (1);
	if (data->num_of_meals == 0)
		exit_program(data, NULL, NULL, 0);
	return (0);
}

int is_simulation_stoped(t_philo *philo)
{
	int stopped;
	int meals_finished;

    pthread_mutex_lock(&philo->data_lock);
    stopped = philo->data->stop;
    meals_finished = (philo->data->num_of_meals != -1 && philo->meals_count > philo->data->num_of_meals);
	if (meals_finished)
		philo->exit_status = 0;
    pthread_mutex_unlock(&philo->data_lock);
    return (stopped || meals_finished);
}
