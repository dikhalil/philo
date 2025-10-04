/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:26:32 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/04 13:16:51 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

//int	end_philos(t_philo *philos, int philo_count)
//{
//	int	status;
//	int i;

//	i = 0;
//	status = 0;
//	if (philo_count != philos->data->num_of_philos)
//		status = 1;
//	while (i < philo_count)
//	{
//		if (waitpid(philos[i].pid, NULL, WNOHANG) == 0)
//		{
//			free_data(philos[i].data, 0);
//			kill(philos[i].pid, SIGKILL);
//			waitpid(philos[i].pid, NULL, 0);
//		}
//		else
//			waitpid(philos[i].pid, NULL, 0);
//		pthread_mutex_destroy(&philos[i].data_lock);
//		i++;
//	}
//	return (status);
//}

//static void wait_childs(t_philo *philos)
//{
//	int status;
//	int all_finished;

//	status = 0;
//	all_finished = 0;
//	while (all_finished < philos->data->num_of_philos)
//	{
//		waitpid(-1, &status, 0);
//		if (WIFEXITED(status))
//		{
//			if (WEXITSTATUS(status) == 1)
//			{
//				end_philos(philos, philos->data->num_of_philos);
//				break ;
//			}
//			else if (WEXITSTATUS(status) == 0)
//				all_finished++;
//		}
//	}
//}
int end_philos(t_philo *philos, int philo_count)
{
	int i;

	for (i = 0; i < philo_count; i++)
	{
		if (kill(philos[i].pid, SIGKILL) == 0)
			waitpid(philos[i].pid, NULL, 0);
		pthread_mutex_destroy(&philos[i].data_lock);
	}
	return 0;
}
static void wait_childs(t_philo *philos)
{
	int status;
	int finished = 0;
	int total = philos->data->num_of_philos;

	while (finished < total)
	{
		if (waitpid(-1, &status, 0) > 0)
		{
			if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
			{
				end_philos(philos, total);
				break;
			}
			finished++;
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

void	*monitor(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->data_lock);
		if ((get_time_ms() - philo->last_meal) > philo->data->time_to_die)
		{
			philo->data->stop = 1;
			philo->exit_status = 1;
			pthread_mutex_unlock(&philo->data_lock);
			sem_wait(philo->data->print_lock);
			printf("%ld %d %s\n", current_time_ms(philo->data), philo->id, "died");
			return (NULL);
		}
		if (philo->data->num_of_meals != -1 && philo->meals_count > philo->data->num_of_meals)
		{
			philo->exit_status = 0;
			pthread_mutex_unlock(&philo->data_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data_lock);
		usleep(100);
	}
	return (NULL);
}
