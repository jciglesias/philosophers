/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structinit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 17:14:03 by jiglesia          #+#    #+#             */
/*   Updated: 2022/04/11 15:30:39 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*kill_philosopher(t_table *p, int pos, long time)
{
	int	i;

	i = 0;
	while (i < p->n_forks)
	{
		pthread_mutex_lock(&p->philo[i].alive_m);
		p->philo[i].alive = 0;
		pthread_mutex_unlock(&p->philo[i++].alive_m);
	}
	pthread_mutex_lock(&p->print_m);
	printf("%ld %d died\n", time, pos + 1);
	pthread_mutex_unlock(&p->print_m);
	return (0);
}

void	t_philoinit(t_philo *p, int *dir, int size)
{
		p->n_forks = dir[0];
		p->t_to_die = dir[1];
		p->t_to_eat = dir[2];
		p->t_to_sleep = dir[3];
		p->n_to_eat = -1;
		if (size == 5)
			p->n_to_eat = dir[4];
		p->starve = 0;
		pthread_mutex_init(&(p->starve_m), NULL);
		pthread_mutex_init(&p->alive_m, NULL);
		pthread_mutex_init(&p->turn_m, NULL);
		p->alive = 1;
}

void	t_tableinit(t_table *p, int *dir, int size)
{
	int	i;

	i = 0;
	p->philo = malloc(sizeof(t_philo) * dir[0]);
	p->mutex = malloc(sizeof(pthread_mutex_t) * dir[0]);
	p->n_forks = dir[0];
	while (i < dir[0])
	{
		t_philoinit(&p->philo[i], dir, size);
		if (!(i % 2))
			p->philo[i].turn = 0;
		else
			p->philo[i].turn = 1;
		pthread_mutex_init(&(p->mutex[i++]), NULL);
	}
	pthread_mutex_init(&p->pos_m, NULL);
	pthread_mutex_init(&p->start_m, NULL);
	pthread_mutex_init(&p->print_m, NULL);
	pthread_mutex_init(&p->lunch_m, NULL);
	p->lunchs = 1;
	p->pos = 0;
}
