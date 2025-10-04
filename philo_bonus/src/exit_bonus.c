/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:12:26 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/04 12:54:53 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

static void safe_sem_close(sem_t *sem)
{
	if (sem && sem != SEM_FAILED)
		sem_close(sem);
}

static void safe_sem_unlink()
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/eat");
}
void free_data(t_data *data, int is_parent)
{
	safe_sem_close(data->forks);
	safe_sem_close(data->print_lock);
	safe_sem_close(data->eat);
	if (is_parent)
		safe_sem_unlink();
}

void	exit_program(t_data *data, t_philo *philos, char *msg, int status)
{
	if (data)
		free_data(data, 1);
	if (philos)
		free(philos);
	if (msg)
		printf("%s\n", msg);
	exit(status);
}

void exit_child(t_philo *philo)
{
	free_data(philo->data, 0);
	//free(philo->philos);
	exit(philo->exit_status);
}
