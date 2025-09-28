/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:20:17 by dikhalil          #+#    #+#             */
/*   Updated: 2025/09/28 19:33:46 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int main(int ac, char **av)
{
    t_data data;
    t_philo *philos;

    if (ac < 5 || ac > 6)
        exit_program(NULL, NULL,"Error: Invalid number of arguments\n", 1);
    if (check_args(ac, av))
        exit_program(NULL, NULL,"Error: Invalid arguments\n", 1);
    if (init_data(ac, av, &data))
        exit_program(NULL, NULL, "Error: Data initialization failed\n", 1);
    if (create_philos(&data, &philos))
        exit_program(&data, philos, "Error: Philosopher creation failed\n", 1);
    if (start_philo(philos))
        exit_program(&data, philos, "Error: Philosopher threads failed\n", 1);
    while (!data.stop)
        monitor_philos(philos);
    if (end_philo(philos, data.num_of_philos))
        exit_program(&data, philos, "Error: Ending Philosopher failed\n", 1);
    exit_program(&data, philos, NULL, 0);
    return (0);
}
