/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:26:32 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/02 20:31:56 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

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
		i++;
	}
	return (0);
}
int	end_philos(t_philo *philos, int philo_count)
{
	int	status;

	status = 0;
	if (philo_count != philos->data->num_of_philos)
		status = 1;
	while (philo_count > 0)
	{
		philo_count--;
		if (waitpid(philos[philo_count].pid, NULL, WNOHANG) == 0)
		{
			kill(philos[philo_count].pid, SIGKILL);
			waitpid(philos[philo_count].pid, NULL, 0);
		}
	}
	return (status);
}
static void wait_childs(t_philo *philos)
{
	int flag;
	pid_t pid;
	int status;
	int all_finished;

	flag = 0;
	status = 0;
	all_finished = 0;
	while (all_finished < philos->data->num_of_philos)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (WIFEXITED(status))
		{
			all_finished++;
			if (WEXITSTATUS(status) == 1)
				flag = 1;
		}
		if (flag)
		{
			end_philos(philos, philos->data->num_of_philos);
			break ;
		}
	}
}

int	start_philo(t_philo *philos)
{
	int	i;
	pid_t pid;
	
	i = 0;
	pid = 0;
	philos[0].data->start_time = get_time_ms();
	while (i < philos->data->num_of_philos)
		philos[i++].last_meal = philos->data->start_time;
	i = 0;
	while (i < philos->data->num_of_philos)
	{
		pid = fork();
		if (pid == -1)
			return (end_philos(philos, i));
		else if (pid == 0)
			philo_routine(&philos[i]);
		else
			philos[i].pid = pid;
		i++;
	}
	wait_childs(philos);
	return (0);
}

//pip becuze evrey process have data struct so not see the stop valus so i will pass it to evrey processs

void	*monitor(void *arg)
{
	t_philo *philo;
	
	philo = (t_philo *)arg;
	sem_wait(philo->data->data_lock);
	if ((get_time_ms() - philo->last_meal) > philo->data->time_to_die)
	{
		philo->data->stop = 1;
		sem_post(philo->data->data_lock);
		print_status(philo, current_time_ms(philo->data), "died");
		exit(1);
	}
	sem_post(philo->data->data_lock);
	sem_wait(philo->data->data_lock);
	if (philo->data->num_of_meals != -1 && philo->meals_count == philo->data->num_of_meals)
	{
		sem_post(philo->data->data_lock);
		exit(0);
	}
	sem_post(philo->data->data_lock);
	usleep(100);
	return (NULL);
}
