/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 14:05:42 by jiglesia          #+#    #+#             */
/*   Updated: 2022/04/11 23:20:05 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	time_ms(t_table *p)
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

void	printline(t_table *tmp, int pos, char *str)
{
	pthread_mutex_lock(&tmp->print_m);
	if (checkalive(&tmp->philo[pos]))
		printf("%ld %d %s", time_ms(tmp), pos + 1, str);
	pthread_mutex_unlock(&tmp->print_m);
}

void	takefork(t_table *tmp, int pos, int philos, int side)
{
	if (!side)
	{
		if (pos % 2)
			pthread_mutex_lock(&tmp->mutex[pos]);
		else
		{
			if (pos == 0)
				pthread_mutex_lock(&tmp->mutex[philos - 1]);
			else
				pthread_mutex_lock(&tmp->mutex[pos - 1]);
		}
	}
	else
	{
		checkturn(&tmp->philo[pos], 1);
		if (pos % 2)
			pthread_mutex_lock(&tmp->mutex[pos - 1]);
		else
			pthread_mutex_lock(&tmp->mutex[pos]);
	}
}

void	checkturn(t_philo *p, int e)
{
	pthread_mutex_lock(&p->turn_m);
	if (!p->turn)
	{
		pthread_mutex_unlock(&p->turn_m);
		if (!e)
			usleep((p->t_to_eat * 1000) + 10);
		else
			usleep(1000);
	}
	else
		pthread_mutex_unlock(&p->turn_m);
}
