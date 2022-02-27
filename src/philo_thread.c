/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 02:47:02 by agardet           #+#    #+#             */
/*   Updated: 2022/02/26 15:01:27 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

//routine

static int	philo_loop(t_philo *self)
{
	if (philo_eat(self) == EXIT_FAILURE)
		return (0);
	if (!self->satisfied && philo_sleep(self) == EXIT_FAILURE)
		return (0);
	if (self->table->settings.b_max_meal
		&& self->n_meal_eaten >= self->table->settings.max_meal)
		return (0);
	return (1);
}

//waiting all to create to start, delayed start based on the philo's id parity
//checking the table to stop if needed , preventing datarace

static void	*philo_routine(void *const arg)
{
	t_philo	*self;

	self = (t_philo *)arg;
	if (pthread_mutex_lock(&self->table->death_lock) != 0)
		return (arg);
	if (pthread_mutex_unlock(&self->table->death_lock) != 0)
		return (arg);
	if (self->id % 2 != 0)
		usleep(2000);
	while (1)
	{
		if (!philo_loop(self))
			return (NULL);
		if (pthread_mutex_lock(&self->table->death_lock) != 0)
			return (NULL);
		if (!self->table->running || self->satisfied)
			break ;
		if (pthread_mutex_unlock(&self->table->death_lock) != 0)
			return (NULL);
	}
	if (pthread_mutex_unlock(&self->table->death_lock) != 0)
		return (NULL);
	return (arg);
}

//init philo, creating mutex (avoid  speak if dead) and starting thread

_Bool	create_philo(t_philo *philo, t_table *table)
{
	static uint64_t	philo_id = 0;

	philo_id++;
	philo->id = philo_id;
	philo->t_birth = get_time();
	philo->t_last_eat = philo->t_birth;
	philo->n_meal_eaten = 0;
	philo->satisfied = 0;
	philo->table = table;
	if (pthread_mutex_init(&philo->lock, NULL) != 0)
		return (EXIT_FAILURE);
	if ((pthread_create(&philo->thread, NULL, philo_routine, philo)) != 0)
		return (EXIT_FAILURE);
	if (pthread_detach(philo->thread) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

//unlocking mutexs

uint64_t	unlock_speak_death(t_philo *philo, uint64_t	time)
{
	if (pthread_mutex_unlock(&philo->table->speak_lock) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_unlock(&philo->table->death_lock) != 0)
		return (EXIT_FAILURE);
	return (time);
}

//re-locking deathlock to change running then unlocking it 
//and speak_lock since it has been kept locked in  talk

int	philo_die(t_table *table, int i)
{
	philo_action(table->philos + i, A_DIE);
	if (pthread_mutex_lock(&table->death_lock) != 0)
		return (EXIT_FAILURE);
	table->running = 0;
	if (pthread_mutex_unlock(&table->death_lock) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_unlock(&table->speak_lock) != 0)
		return (EXIT_FAILURE);
	return (1);
}
