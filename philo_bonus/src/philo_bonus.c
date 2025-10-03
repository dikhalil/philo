/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:20:17 by dikhalil          #+#    #+#             */
/*   Updated: 2025/10/03 17:23:33 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

int	main(int ac, char **av)
{
	t_data	data;
	t_philo	*philos;

	if (ac < 5 || ac > 6)
		exit_program(NULL, NULL, "Error: Invalid number of arguments\n", 1);
	if (check_args(ac, av))
		exit_program(NULL, NULL, "Error: Invalid arguments\n", 1);
	if (init_data(ac, av, &data))
		exit_program(NULL, NULL, "Error: Invalid Data\n", 1);
	if (create_philos(&data, &philos))
		exit_program(&data, philos, "Error: Philosopher creation failed\n", 1);
	if (start_philo(philos))
		exit_program(&data, philos, "Error: Philosopher Starting failed\n", 1);
	if (end_philos(philos, data.num_of_philos))
		exit_program(&data, philos, "Error: Philosopher Ending failed\n", 1);
	exit_program(&data, philos, NULL, 0);
	return (0);
}
