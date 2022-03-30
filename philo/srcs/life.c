/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:19:28 by jiglesia          #+#    #+#             */
/*   Updated: 2022/03/30 18:41:31 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	time_ms(t_philo *p)
{
	struct timeval	time;
	long			a;

	gettimeofday(&time, NULL);
	a = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	pthread_mutex_lock(&p->start_m);
	a -= p->start;
	pthread_mutex_unlock(&p->start_m);
	return (a);
}

int	checkalive(t_philo	*p, int pos)
{
	pthread_mutex_lock(&p->alive_m[pos]);
	if (p->alive[pos])
	{
		pthread_mutex_unlock(&p->alive_m[pos]);
		return (1);
	}
	pthread_mutex_unlock(&p->alive_m[pos]);
	return (0);
}

void	printline(t_philo *tmp, int pos, char *str)
{
	pthread_mutex_lock(&tmp->print_m);
	if (checkalive(tmp, pos))
		printf("%ld %d %s", time_ms(tmp), pos + 1, str);
	pthread_mutex_unlock(&tmp->print_m);
}

void	takefork(t_philo *tmp, int pos, int philos, int side)
{
	if (!side)
	{
		if (pos % 2)
			pthread_mutex_lock(&tmp->mutex[pos]);
		else
		{
			if (pos < (philos - 1))
				pthread_mutex_lock(&tmp->mutex[pos + 1]);
			else
				pthread_mutex_lock(&tmp->mutex[0]);
		}
	}
	else
	{
		if (pos % 2)
		{
			if (pos < (philos - 1))
				pthread_mutex_lock(&tmp->mutex[pos + 1]);
			else
				pthread_mutex_lock(&tmp->mutex[0]);
		}
		else
			pthread_mutex_lock(&tmp->mutex[pos]);
	}
}

int	time_to_eat(t_philo *tmp, int pos, int *dir)
{
	if (!checkalive(tmp, pos))
		return (1);
	takefork(tmp, pos, dir[0], 0);
	printline(tmp, pos, "has taken a fork\n");
	if (dir[0] < 2)
	{
		pthread_mutex_unlock(&tmp->mutex[pos]);
		usleep(dir[1] * 1000);
		return (1);
	}
	takefork(tmp, pos, dir[0], 1);
	printline(tmp, pos, "has taken a fork\n");
	pthread_mutex_lock(&tmp->starve_m[pos]);
	tmp->starve[pos] = time_ms(tmp);
	pthread_mutex_unlock(&tmp->starve_m[pos]);
	printline(tmp, pos, "is eating\n");
	return (0);
}

int	after_meal(t_philo *tmp, int pos, int *dir)
{
	long	time;
	long	new_time;

	time = time_ms(tmp);
	new_time = time_ms(tmp);
	while ((new_time - time) < dir[2])
		new_time = time_ms(tmp);
	printline(tmp, pos, "is sleeping\n");
	pthread_mutex_unlock(&tmp->mutex[pos]);
	if (pos < (dir[0] - 1))
		pthread_mutex_unlock(&tmp->mutex[pos + 1]);
	else
		pthread_mutex_unlock(&tmp->mutex[0]);
	time = time_ms(tmp);
	new_time = time_ms(tmp);
	while ((new_time - time) < dir[3])
		new_time = time_ms(tmp);
	if (!checkalive(tmp, pos))
		return (1);
	printline(tmp, pos, "is thinking\n");
	return (0);
}

void	*life(void *p)
{
	t_philo			*tmp;
	int				pos;
	int				dir[5];

	tmp = (t_philo *)p;
	pthread_mutex_lock(&tmp->dir_m);
	dir[0] = tmp->n_forks;
	dir[1] = tmp->t_to_die;
	dir[2] = tmp->t_to_eat;
	dir[3] = tmp->t_to_sleep;
	dir[4] = tmp->n_to_eat;
	pos = tmp->pos++;
	pthread_mutex_unlock(&tmp->dir_m);
	while (dir[4])
	{
		if (time_to_eat(tmp, pos, dir))
			return (0);
		dir[4]--;
		if (after_meal(tmp, pos, dir))
			return (0);
	}
	if (!dir[4])
	{
		pthread_mutex_lock(&tmp->lunch_m);
		tmp->lunchs = 0;
		pthread_mutex_unlock(&tmp->lunch_m);
	}
	return (0);
}
