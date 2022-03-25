/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:19:28 by jiglesia          #+#    #+#             */
/*   Updated: 2022/03/25 12:03:48 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	time_ms(struct timeval time)
{
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	time_to_eat(t_philo *tmp, int pos, struct timeval time)
{
	if (!tmp->alive)
		return (1);
	pthread_mutex_lock(&tmp->mutex[pos]);
	gettimeofday(&time, NULL);
	if (tmp->alive)
		printf("%ld %d has taken a fork\n", time_ms(time) - tmp->start, pos + 1);
	if (tmp->n_forks < 2)
	{
		usleep(tmp->t_to_die * 1000);
		return (1);
	}
	if (pos < (tmp->n_forks - 1))
		pthread_mutex_lock(&tmp->mutex[pos + 1]);
	else
		pthread_mutex_lock(&tmp->mutex[0]);
	if (tmp->alive)
	{
		gettimeofday(&time, NULL);
		printf("%ld %d has taken a fork\n", time_ms(time) - tmp->start, pos + 1);
		tmp->starve[pos] = time_ms(time);
		printf("%ld %d is eating\n", time_ms(time) - tmp->start, pos + 1);
	}
	return (0);
}

int	after_meal(t_philo *tmp, int pos, struct timeval time)
{
	usleep(tmp->t_to_eat * 1000);
	gettimeofday(&time, NULL);
	if (tmp->alive)
		printf("%ld %d is sleeping\n", time_ms(time) - tmp->start, pos + 1);
	pthread_mutex_unlock(&tmp->mutex[pos]);
	if (pos < (tmp->n_forks - 1))
		pthread_mutex_unlock(&tmp->mutex[pos + 1]);
	else
		pthread_mutex_unlock(&tmp->mutex[0]);
	if (!tmp->alive)
		return (1);
	usleep(tmp->t_to_sleep * 1000);
	if ((time_ms(time) - tmp->starve[pos] + tmp->t_to_sleep) > tmp->t_to_die)
		return (1);
	gettimeofday(&time, NULL);
	if (!tmp->alive)
		return (1);
	if (tmp->alive)
		printf("%ld %d is thinking\n", time_ms(time) - tmp->start, pos + 1);
	return (0);
}

void	*life(void *p)
{
	t_philo			*tmp;
	int				pos;
	struct timeval	time;
	int				lunchs;

	tmp = (t_philo *)p;
	pos = tmp->pos++;
	gettimeofday(&time, NULL);
	tmp->starve[pos] = time_ms(time);
	lunchs = tmp->n_to_eat;
	while (lunchs && tmp->alive)
	{
		if (time_to_eat(tmp, pos, time))
			return (0);
		lunchs--;
		if (after_meal(tmp, pos, time))
			return (0);
	}
	if (!lunchs)
		tmp->lunchs = 0;
	return (0);
}
