/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dikhalil <dikhalil@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:20:17 by dikhalil          #+#    #+#             */
/*   Updated: 2025/09/28 14:14:32 by dikhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_time_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.usec / 1000);
}

int is_number(char *str)
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
long ft_atol(char *str)
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

int init_data(int ac, char **av, t_data *data)
{
    int i;

    i = 0;
    data->num_of_philos = atoi(av[1]);
    data->time_to_die = atoi(av[2]);
    data->time_to_eat = atoi(av[3]);
    data->time_to_sleep = atoi(av[4]);
    if (ac == 6)
        data->num_of_meals = atoi(av[5]);
    else
        data->num_of_meals = -1;
    data->start_time = get_time_ms();
    data->stop = 0;
    data->forks = malloc(sizeof(pthread_mutex_t) * data->num_of_philos);
    if (!data->forks)
        return (1);
    while (i < data->num_of_philos)
    {
        if (pthread_mutex_init(&data->forks[i], NULL))
        {
            free_data(data, i, 0, 0);
            return (1);
        }
        i++;
    }
    if (pthread_mutex_init(&data->print_lock, NULL))
    {
        free_data(data, i, 0, 0);
        return (1);
    }
    if (pthread_mutex_init(&data->data_lock, NULL))
    {
        free_data(data, i, 1, 0);
        return (1);
    }
    return (check_data(data));
}

int create_philos(t_data *data, t_philo **philos)
{
    int i;

    i = 0;
    *philos = malloc(sizeof(t_philo) * data->num_of_philos);
    if (!*philos)
        return (1);
    while (i < data->num_of_philos)
    {
        (*philos)[i].id = i + 1;
        (*philos)[i].meals_count = 0;
        (*philos)[i].left_fork = i;
        (*philos)[i].right_fork = (i + 1) % data->num_of_philos;
        (*philos)[i].last_meal = get_time_ms();
        (*philos)[i].data = data;
        i++;
    }
    return (0);
}

void thinking(t_philo *philo)
{
    print_status(philo, current_time_ms(philo->data), "is thinking");
}
void take_leftfork(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
    print_status(philo, current_time_ms(philo->data), "has taken left fork");
}
void take_rightfork(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
    print_status(philo, current_time_ms(philo->data), "has taken right fork");
}
void eating(t_philo *philo)
{
    if (philo->data->num_of_philos == 1)
    {
        take_leftfork(philo);
        usleep(philo->data->time_to_die * 1000);
        pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
        return ;
    }
    if (philo->id % 2 == 0)
    {
        take_leftfork(philo);
        take_rightfork(philo);
    }
    else
    {
        take_rightfork(philo);
        take_leftfork(philo);
    }
    pthread_mutex_lock(&philo->data->data_lock);
    philo->meals_count++;
    philo->last_meal = get_time_ms();
    pthread_mutex_unlock(&philo->data->data_lock);
    print_status(philo, current_time_ms(philo->data), "is eating");
    usleep(philo->data->time_to_eat * 1000);
    pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
    pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
}

void sleeping(t_philo *philo)
{
    print_status(philo, current_time_ms(philo->data), "is sleeping");
    usleep(philo->data->time_to_sleep * 1000);
}

void *philo_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    while (1)
    {
        pthread_mutex_lock(&philo->data->data_lock);
        if (philo->data->stop)
        {
            pthread_mutex_unlock(&philo->data->data_lock);
            break;
        }
        pthread_mutex_unlock(&philo->data->data_lock);
        thinking(philo);
        eating(philo);
        sleeping(philo);
    }
    return (NULL);
}

int   start_philo(t_philo *philos)
{
    int i;

    i = 0;
    while (i < philos[0].data->num_of_philos)
    {
        if (pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]))
        {
            end_philo(philos, i);
            return (1);
        }
        i++;
    }
    return (0);
}
void print_status(t_philo *philo, long current_time_ms, char *status)
{
    pthread_mutex_lock(&philo->data->print_lock);
    printf("%ld %d %s\n", current_time_ms, philo->id, status);
    pthread_mutex_unlock(&philo->data->print_lock);
}


long current_time_ms(t_data *data)
{
    return (get_time_ms() - data->start_time);
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
void monitor_philos(t_philo *philos)
{
    int i;

    i = 0;
    while (i < philos[0].data->num_of_philos)
    {
        pthread_mutex_lock(&philos->data->data_lock);
        if ((get_time_ms() - philos[i].last_meal) > philos[0].data->time_to_die)
        {
            philos[i].data->stop = 1;
            pthread_mutex_unlock(&philos->data->data_lock);
            print_status(&philos[i], current_time_ms(philos->data), "died");
            return ;
        }
        pthread_mutex_unlock(&philos->data->data_lock);
        i++;
    }
    if (philos[0].data->num_of_meals != -1 && all_finished(philos))
    {   
        pthread_mutex_lock(&philos->data->data_lock);
        philos[0].data->stop = 1;
        pthread_mutex_unlock(&philos->data->data_lock);
        return ;
    }
    usleep(100);
}
int end_philo(t_philo *philos, int philo_count)
{
    int status;

    status = 0;
    if (philo_count != philos[0].data->num_of_philos)
        status = 1;
    while (philo_count--)
    {
        if (pthread_join(philos[philo_count].thread, NULL))
            return (1);
    }
    return (status);
}

int free_data(t_data *data, int mutex_count, int print_lock, int data_lock)
{
    int i;

    i = 0;
    if (data->forks)
    {
        while (i < mutex_count)
            if (pthread_mutex_destroy(&data->forks[i++]))
                return (1);
        free(data->forks);
    }
    if (print_lock)
        if (pthread_mutex_destroy(&data->print_lock))
            return (1);
    if (data_lock)
        if (pthread_mutex_destroy(&data->data_lock))
            return (1);
    return (0);
}

void exit_program(t_data *data, t_philo *philos, char *msg, int status)
{
    if (data)
        if (free_data(data, data->num_of_philos, 1, 1))
        {
            printf("Error: Mutex destroy failed\n");
            status = 1;
        }
    if (philos)
        free(philos);
    if (msg)
        printf("%s\n", msg);
    exit(status);
}


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
