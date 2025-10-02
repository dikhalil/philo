/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphore.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:18:10 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/02 20:12:22 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <philo_bonus.h>

void	print_status(t_philo *philo, long current_time_ms, char *status)
{
	sem_wait(philo->data->data_lock);
	if (!philo->data->stop || !ft_strncmp(status, "died", 4))
	{
		sem_wait(philo->data->print_lock);
	    printf("%ld %d %s\n", current_time_ms, philo->id, status);
		sem_post(philo->data->print_lock);
	}
	sem_post(philo->data->data_lock);
}
