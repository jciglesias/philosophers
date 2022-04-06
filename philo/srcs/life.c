/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 11:19:28 by jiglesia          #+#    #+#             */
/*   Updated: 2022/04/05 17:21:19 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	nextturn(t_philo *p, int pos, int n_forks)
{
	if (pos == 0)
	{
		pthread_mutex_lock(&p->turn_m[n_forks - 1]);
		p->turn[n_forks - 1] = 1;
		pthread_mutex_unlock(&p->turn_m[n_forks - 1]);
	}
	else
	{
		pthread_mutex_lock(&p->turn_m[pos - 1]);
		p->turn[pos - 1] = 1;
		pthread_mutex_unlock(&p->turn_m[pos - 1]);
	}
	pthread_mutex_lock(&p->turn_m[pos]);
	p->turn[pos] = 0;
	pthread_mutex_unlock(&p->turn_m[pos]);
}

int	time_to_eat(t_philo *tmp, int pos, int *dir)
{
	if (!checkalive(tmp, pos))
	{
		nextturn(tmp, pos, dir[0]);
		return (1);
	}
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
	while ((new_time - time) <= dir[2])
		new_time = time_ms(tmp);
	printline(tmp, pos, "is sleeping\n");
	nextturn(tmp, pos, dir[0]);
	pthread_mutex_unlock(&tmp->mutex[pos]);
	if (pos < (dir[0] - 1))
		pthread_mutex_unlock(&tmp->mutex[pos + 1]);
	else
		pthread_mutex_unlock(&tmp->mutex[0]);
	time = time_ms(tmp);
	new_time = time_ms(tmp);
	while ((new_time - time) <= dir[3])
		new_time = time_ms(tmp);
	if (!checkalive(tmp, pos))
		return (1);
	printline(tmp, pos, "is thinking\n");
	return (0);
}

int	dirinit(t_philo *p, int *dir)
{
	dir[0] = p->n_forks;
	dir[1] = p->t_to_die;
	dir[2] = p->t_to_eat;
	dir[3] = p->t_to_sleep;
	dir[4] = p->n_to_eat;
	return (p->pos++);
}

void	*life(void *p)
{
	t_philo			*tmp;
	int				pos;
	int				dir[5];

	tmp = (t_philo *)p;
	pthread_mutex_lock(&tmp->dir_m);
	pos = dirinit(tmp, dir);
	pthread_mutex_unlock(&tmp->dir_m);
	while (dir[4])
	{
		if (dir[0] > 1)
			checkturn(tmp, pos);
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
