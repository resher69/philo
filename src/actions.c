/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 15:05:59 by agardet           #+#    #+#             */
/*   Updated: 2022/02/26 17:49:45 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//locking all mutex to avoid datarace and give a getaway if anyone died

uint64_t	philo_action(t_philo *philo, int action_id)
{
	uint64_t	time;
	static char	*action[5] = {
		"has taken a fork",
		"is eating",
		"is sleeping",
		"is thinking",
		"died"
	};

	time = get_time();
	if (pthread_mutex_lock(&philo->table->speak_lock) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_lock(&philo->table->death_lock) != 0)
		return (EXIT_FAILURE);
	if (!philo->table->running)
		return (unlock_speak_death(philo, time));
	if (pthread_mutex_unlock(&philo->table->death_lock) != 0)
		return (EXIT_FAILURE);
	printf ("\033[1;32m%6llu\033[0m %2llu %s\n",
		time - philo->table->start_time, philo->id, action[action_id]);
	if (action_id != 5)
		if (pthread_mutex_unlock(&philo->table->speak_lock) != 0)
			return (EXIT_FAILURE);
	return (time);
}

//locking forks, locking self to avoid datarace then eating

_Bool	philo_eat(t_philo *philo)
{
	if (pthread_mutex_lock(philo->forks[0]) != 0)
		return (EXIT_FAILURE);
	if (philo_action(philo, A_FORK) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (pthread_mutex_lock(philo->forks[1]) != 0)
		return (EXIT_FAILURE);
	if (philo_action(philo, A_FORK) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (pthread_mutex_lock(&philo->lock) != 0)
		return (EXIT_FAILURE);
	philo->t_last_eat = philo_action(philo, A_EAT);
	if (philo->table->settings.b_max_meal)
		philo->n_meal_eaten++;
	if (!philo->satisfied && philo->table->settings.b_max_meal
		&& philo->n_meal_eaten == philo->table->settings.max_meal)
		philo->satisfied = 1;
	if (pthread_mutex_unlock(&philo->lock) != 0)
		return (EXIT_FAILURE);
	sleep_until(philo->t_last_eat + philo->table->settings.t_eat);
	if (pthread_mutex_unlock(philo->forks[0]) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_unlock(philo->forks[1]) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

//sleep

unsigned char	philo_sleep(t_philo *philo)
{
	sleep_until(philo_action(philo, A_SLEEP) + philo->table->settings.t_sleep);
	philo_action(philo, A_THINK);
	return (EXIT_SUCCESS);
}

// locking self to check satisfaction, and checking death

int	all_satisfied(t_table *table)
{
	uint64_t		satisfied;
	unsigned char	i;

	i = 0;
	satisfied = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_lock(&table->philos[i].lock) != 0)
			return (EXIT_FAILURE);
		satisfied += table->philos[i].satisfied;
		if (get_time_since(table->philos[i].t_last_eat) \
			>= table->settings.t_die)
			return (philo_die(table, i));
		if (pthread_mutex_unlock(&table->philos[i].lock) != 0)
			return (EXIT_FAILURE);
		i++;
	}
	if (table->settings.b_max_meal > 0 && satisfied == table->nb_philos)
		return (1);
	return (0);
}

int	free_table(t_table *table)
{
	usleep(1000 * (table->settings.t_eat + 2 * table->settings.t_sleep));
	free(table->philos);
	free(table->forks);
	free(table);
	return (EXIT_SUCCESS);
}
