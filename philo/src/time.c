/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:13:10 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/04 08:47:30 by dikhalil         ###   ########.fr       */
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

void custom_usleep(t_philo *philo, long timems)
{
	long start;
	
	start = get_time_ms();
	while (!is_simulation_stoped(philo) && ((get_time_ms() - start) < timems))
		usleep(100);
}

