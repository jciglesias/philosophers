/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 14:05:42 by jiglesia          #+#    #+#             */
/*   Updated: 2022/04/05 14:07:03 by jiglesia         ###   ########.fr       */
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

void	checkturn(t_philo *p, int pos)
{
	pthread_mutex_lock(&p->turn_m[pos]);
	while (!p->turn[pos])
	{
		pthread_mutex_unlock(&p->turn_m[pos]);
		pthread_mutex_lock(&p->turn_m[pos]);
	}
	pthread_mutex_unlock(&p->turn_m[pos]);
}
