/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:19:28 by jiglesia          #+#    #+#             */
/*   Updated: 2022/04/14 00:03:36 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	nextturn(t_table *p, int pos, int n_forks)
{
	if (pos == 0)
	{
		pthread_mutex_lock(&p->philo[n_forks - 1].turn_m);
		p->philo[n_forks - 1].turn = 1;
		pthread_mutex_unlock(&p->philo[n_forks - 1].turn_m);
	}
	else
	{
		pthread_mutex_lock(&p->philo[pos - 1].turn_m);
		p->philo[pos - 1].turn = 1;
		pthread_mutex_unlock(&p->philo[pos - 1].turn_m);
	}
	pthread_mutex_lock(&p->philo[pos].turn_m);
	p->philo[pos].turn = 0;
	pthread_mutex_unlock(&p->philo[pos].turn_m);
}

int	time_to_eat(t_table *tmp, int pos)
{
	if (!checkalive(&tmp->philo[pos]))
	{
		nextturn(tmp, pos, tmp->philo[pos].n_forks);
		return (1);
	}
	takefork(tmp, pos, tmp->philo[pos].n_forks, 0);
	printline(tmp, pos, "has taken a fork\n");
	if (tmp->philo[pos].n_forks < 2)
	{
		pthread_mutex_unlock(&tmp->mutex[pos]);
		usleep(tmp->philo[pos].t_to_die * 1000);
		return (1);
	}
	takefork(tmp, pos, tmp->philo[pos].n_forks, 1);
	printline(tmp, pos, "has taken a fork\n");
	pthread_mutex_lock(&tmp->philo[pos].starve_m);
	tmp->philo[pos].starve = time_ms(tmp);
	pthread_mutex_unlock(&tmp->philo[pos].starve_m);
	printline(tmp, pos, "is eating\n");
	return (0);
}

void	mysleep(long t)
{
	struct timeval	time;
	long			start;
	long			end;

	gettimeofday(&time, NULL);
	start = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	end = start;
	while ((end - start) < t)
	{
		usleep(500);
		gettimeofday(&time, NULL);
		end = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	}
}

int	after_meal(t_table *tmp, int pos)
{
	mysleep(tmp->philo[pos].t_to_eat);
	nextturn(tmp, pos, tmp->philo[pos].n_forks);
	printline(tmp, pos, "is sleeping\n");
	pthread_mutex_unlock(&tmp->mutex[pos]);
	if (pos == 0)
		pthread_mutex_unlock(&tmp->mutex[tmp->philo[pos].n_forks - 1]);
	else
		pthread_mutex_unlock(&tmp->mutex[pos - 1]);
	mysleep(tmp->philo[pos].t_to_sleep);
	if (!checkalive(&tmp->philo[pos]))
		return (1);
	printline(tmp, pos, "is thinking\n");
	return (0);
}

void	*life(void *p)
{
	t_table			*tmp;
	int				pos;

	tmp = (t_table *)p;
	pthread_mutex_lock(&tmp->pos_m);
	pos = tmp->pos++;
	pthread_mutex_unlock(&tmp->pos_m);
	while (tmp->philo[pos].n_to_eat)
	{
		if (tmp->philo[pos].n_forks > 1)
			checkturn(&tmp->philo[pos]);
		if (time_to_eat(tmp, pos))
			return (0);
		tmp->philo[pos].n_to_eat--;
		if (after_meal(tmp, pos))
			return (0);
	}
	if (!tmp->philo[pos].n_to_eat)
	{
		pthread_mutex_lock(&tmp->lunch_m);
		tmp->lunchs = 0;
		pthread_mutex_unlock(&tmp->lunch_m);
	}
	return (0);
}
