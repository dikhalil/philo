/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:13:10 by dikhalil          #+#    #+#             */
/*   Updated: 2025/09/29 14:41:51 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long	current_time_ms(t_data *data)
{
	long ct;
	
	ct = get_time_ms() - data->start_time;
	return (ct);
}

void custom_usleep(t_philo *philo, long timemc)
{
	long start;
	long end;
	
	start = get_time_ms() * 1000;
	end = start + timemc;
	while (get_time_ms() * 1000 < end)
	{
		pthread_mutex_lock(&philo->data->data_lock);
		if (philo->data->stop)
		{
			pthread_mutex_unlock(&philo->data->data_lock);
			break;
		}
		pthread_mutex_unlock(&philo->data->data_lock);
		usleep(100);
	}
}
