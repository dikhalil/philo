/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:09:47 by dikhalil          #+#    #+#             */
/*   Updated: 2025/09/28 20:08:05 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static int is_number(char *str)
{
    int i = 0;
    if (!str || !str[i])
        return (0);
    if (str[i]== '+' || str[i]== '-')
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

static long ft_atol(char *str)
{
    long res = 0;
    int sign = 1;
    int i = 0;
    
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i])
    {
        res = res * 10 + (str[i] - '0');
        i++;
    }
    return (res * sign);
}

int check_args(int ac, char **av)
{
    int i;
    long num;
    
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

int check_data(t_data *data)
{
    if (data->num_of_philos < 1)
        return (1);
    if (data->time_to_die < 0 || data->time_to_eat < 0 || data->time_to_sleep < 0)
        return (1);
    if (data->num_of_meals < -1 || data->num_of_meals == 0 )
        return (1);
    return (0);
}

int all_finished(t_philo *philos)
{
    int i;

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

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*s_1;
	unsigned char	*s_2;
	size_t			i;

	s_1 = (unsigned char *)s1;
	s_2 = (unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while (i < n)
	{
		if (*s_1 != *s_2)
			return (*s_1 - *s_2);
		if (*s_1 == '\0')
			return (0);
		s_1++;
		s_2++;
		i++;
	}
	return (0);
}
