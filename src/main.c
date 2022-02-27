/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 00:56:01 by agardet           #+#    #+#             */
/*   Updated: 2022/02/26 15:55:35 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

//creation of mutexs for forks, speech and death_lock

int	init_mutex(t_table *table)
{
	uint64_t	i;
	uint64_t	nb_forks;

	nb_forks = table->nb_philos;
	table->forks = (pthread_mutex_t *) \
		malloc(sizeof(pthread_mutex_t) * nb_forks);
	if (!table->forks)
		return (EXIT_FAILURE);
	i = 0;
	while (i < nb_forks)
		if (pthread_mutex_init(table->forks + i++, NULL) != 0)
			return (EXIT_FAILURE);
	if (pthread_mutex_init(&table->speak_lock, NULL) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&table->death_lock, NULL) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

//lock mutex to avoid sync issues at start 
//then giving forks to each one and creating threads(philos)

static void	init_philos(t_table *table)
{
	uint64_t	i;

	i = 0;
	if (pthread_mutex_lock(&table->death_lock) != 0)
		return ;
	while (i < table->nb_philos)
	{
		table->philos[i].forks[0] = table->forks + i;
		if (i < table->nb_philos - 1)
			table->philos[i].forks[1] = table->forks + i + 1;
		else
			table->philos[i].forks[1] = table->forks;
		create_philo(table->philos + i, table);
		i++;
	}
	if (pthread_mutex_unlock(&table->death_lock) != 0)
		return ;
}

//init then main loop -> checking if anyone died or have had enough meals

static _Bool	init_table(t_settings settings)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (EXIT_FAILURE);
	table->settings = settings;
	table->philos = malloc(sizeof(t_philo) * settings.nb_philos);
	if (!table->philos)
		return (EXIT_FAILURE);
	table->nb_philos = settings.nb_philos;
	table->start_time = get_time();
	table->running = 1;
	init_mutex(table);
	init_philos(table);
	while (table->running && !all_satisfied(table))
		usleep(500);
	free_table(table);
	return (EXIT_SUCCESS);
}

//check if user_input is correct

static _Bool	check_usr_input(t_settings settings, char **argv)
{
	uint64_t	time;

	time = get_time();
	if (!ft_isnum(argv[1]) || !ft_isnum(argv[2]) || !ft_isnum(argv[3])
		|| !ft_isnum(argv[4]) || (settings.b_max_meal && !ft_isnum(argv[5])))
		return (printf ("Error: parameters are not digits.\n")
			* 0 + 1);
	if (settings.t_die > INT_MAX || settings.t_eat > INT_MAX
		|| settings.t_sleep > INT_MAX
		|| (settings.b_max_meal && settings.max_meal > INT_MAX))
		return (printf ("Error: parameters are greater than MAX_INT.\n")
			* 0 + 1);
	if (settings.t_die < 60 || settings.t_eat < 60 || settings.t_sleep < 60
		|| (settings.b_max_meal && settings.max_meal < 1))
		return (printf ("Error: values under 60ms are not allowed.\n")
			* 0 + 1);
	if (settings.nb_philos <= 1)
	{
		usleep(settings.t_die * 1000);
		return (printf ("\033[1;32m%6u\033[0m 1 has died\n", settings.t_die)
			* 0 + 1);
	}
	else if (settings.nb_philos > MAX_THREADS)
		return (printf("Error: too many philosophers.\n") * 0 + 1);
	return (EXIT_SUCCESS);
}

//main

int	main(int argc, char **argv)
{
	t_settings	settings;

	if (argc != 5 && argc != 6)
		return (printf ("Error: invalid parameters\n") * 0 + EXIT_FAILURE);
	settings.nb_philos = ft_atoi(argv[1]);
	settings.t_die = ft_atoi(argv[2]);
	settings.t_eat = ft_atoi(argv[3]);
	settings.t_sleep = ft_atoi(argv[4]);
	settings.b_max_meal = 0;
	settings.max_meal = 0;
	if (argc == 6)
	{
		settings.b_max_meal = 1;
		settings.max_meal = ft_atoi(argv[5]);
	}
	if (check_usr_input(settings, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	init_table(settings);
	return (EXIT_SUCCESS);
}
