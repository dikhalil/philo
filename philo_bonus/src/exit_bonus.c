/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:12:26 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/03 17:23:45 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

static void safe_sem_close(sem_t *sem, char *name)
{
	if (sem && sem != SEM_FAILED)
	{
		sem_close(sem);
		sem_unlink(name);
	}
}
void free_data(t_data *data)
{
	safe_sem_close(data->forks, "/forks");
    safe_sem_close(data->print_lock, "/print");
    safe_sem_close(data->data_lock, "/data");
    safe_sem_close(data->eat, "/eat");
}

void	exit_program(t_data *data, t_philo *philos, char *msg, int status)
{
	if (data)
		free_data(data);
	if (philos)
		free(philos);
	if (msg)
		printf("%s\n", msg);
	exit(status);
}
