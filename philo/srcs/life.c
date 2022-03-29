/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:19:28 by jiglesia          #+#    #+#             */
/*   Updated: 2022/03/29 17:02:01 by jiglesia         ###   ########.fr       */
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

int	checkalive(t_philo	*p)
{
	pthread_mutex_lock(&p->alive_m);
	if (p->alive)
	{
		pthread_mutex_unlock(&p->alive_m);
		return (1);
	}
	pthread_mutex_unlock(&p->alive_m);
	return (0);
}

int	time_to_eat(t_philo *tmp, int pos, int *dir)
{
	if (!checkalive(tmp))
		return (1);
	if (pos % 2)
		pthread_mutex_lock(&tmp->mutex[pos]);
	else
	{
		if (pos < (dir[0] - 1))
			pthread_mutex_lock(&tmp->mutex[pos + 1]);
		else
			pthread_mutex_lock(&tmp->mutex[0]);
	}
	if (checkalive(tmp))
	{
		pthread_mutex_lock(&tmp->print_m);
		printf("%ld %d has taken a fork\n", time_ms(tmp), pos + 1);
		pthread_mutex_unlock(&tmp->print_m);
	}
	if (dir[0] < 2)
	{
		usleep(dir[1] * 1000);
		return (1);
	}
	if (pos % 2)
	{
		if (pos < (dir[0] - 1))
			pthread_mutex_lock(&tmp->mutex[pos + 1]);
		else
			pthread_mutex_lock(&tmp->mutex[0]);
	}
	else
		pthread_mutex_lock(&tmp->mutex[pos]);
	if (checkalive(tmp))
	{
		pthread_mutex_lock(&tmp->print_m);
		printf("%ld %d has taken a fork\n", time_ms(tmp), pos + 1);
		pthread_mutex_unlock(&tmp->print_m);
		pthread_mutex_lock(&tmp->starve_m[pos]);
		tmp->starve[pos] = time_ms(tmp);
		pthread_mutex_unlock(&tmp->starve_m[pos]);
		pthread_mutex_lock(&tmp->print_m);
		printf("%ld %d is eating\n", time_ms(tmp), pos + 1);
		pthread_mutex_unlock(&tmp->print_m);
	}
	return (0);
}

int	after_meal(t_philo *tmp, int pos, int *dir)
{
	usleep(dir[2] * 1000);
	if (checkalive(tmp))
	{
		pthread_mutex_lock(&tmp->print_m);
		printf("%ld %d is sleeping\n", time_ms(tmp), pos + 1);
		pthread_mutex_unlock(&tmp->print_m);
	}
	pthread_mutex_unlock(&tmp->mutex[pos]);
	if (pos < (dir[0] - 1))
		pthread_mutex_unlock(&tmp->mutex[pos + 1]);
	else
		pthread_mutex_unlock(&tmp->mutex[0]);
	if (!checkalive(tmp))
		return (1);
	usleep(dir[3] * 1000);
	pthread_mutex_lock(&tmp->starve_m[pos]);
	if ((time_ms(tmp) - tmp->starve[pos] + dir[3]) > dir[1])
	{
		pthread_mutex_unlock(&tmp->starve_m[pos]);
		return (1);
	}
	pthread_mutex_unlock(&tmp->starve_m[pos]);
	if (!checkalive(tmp))
		return (1);
	if (checkalive(tmp))
	{
		pthread_mutex_lock(&tmp->print_m);
		printf("%ld %d is thinking\n", time_ms(tmp), pos + 1);
		pthread_mutex_unlock(&tmp->print_m);
	}
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
	pthread_mutex_lock(&tmp->starve_m[pos]);
	tmp->starve[pos] = time_ms(tmp);
	pthread_mutex_unlock(&tmp->starve_m[pos]);
	while (dir[4] && checkalive(tmp))
	{
		if (time_to_eat(tmp, pos, dir))
			return (0);
		dir[4]--;
		if (after_meal(tmp, pos, dir))
			return (0);
	}
	if (!dir[4])
		tmp->lunchs = 0;
	return (0);
}
