/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 15:07:18 by agardet           #+#    #+#             */
/*   Updated: 2022/02/26 13:09:08 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdint.h>

# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>
# include "limits.h"

# define    A_FORK  0
# define    A_EAT   1
# define    A_SLEEP 2
# define    A_THINK 3
# define    A_DIE   4

# define    MAX_THREADS 200

/*
**  Philo
*/

typedef struct s_philo
{
	uint64_t		id;
	uint64_t		t_birth;
	uint64_t		t_last_eat;
	uint64_t		n_meal_eaten;
	unsigned char	satisfied;
	pthread_t		thread;
	struct s_table	*table;
	pthread_mutex_t	*forks[2];
	pthread_mutex_t	lock;
}				t_philo;

typedef struct s_settings
{
	uint64_t		nb_philos;
	unsigned int	t_die;
	unsigned int	t_eat;
	unsigned int	t_sleep;
	unsigned int	max_meal;
	unsigned char	b_max_meal;
}				t_settings;

typedef struct s_table
{
	unsigned char	running;
	uint64_t		start_time;
	uint64_t		nb_philos;
	t_philo			*philos;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	speak_lock;
	pthread_mutex_t	*forks;
	t_settings		settings;
}				t_table;

_Bool				create_philo(t_philo *philo, t_table *table);
uint64_t			philo_action(t_philo *const philo, int action_id);
_Bool				philo_eat(t_philo *const philo);
unsigned char		philo_sleep(t_philo *const philo);
int					philo_die(t_table *table, int i);
int					all_satisfied(t_table *table);

/*
**  utils
*/

uint64_t			ft_atoi(char *s);
unsigned char		ft_isnum(char *s);
uint64_t			get_time(void);
uint64_t			get_time_since(const uint64_t time);
void				sleep_until(const uint64_t time);
int					free_table(t_table *table);
uint64_t			unlock_speak_death(t_philo *philo, uint64_t	time);

#endif