/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structinit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 17:14:03 by jiglesia          #+#    #+#             */
/*   Updated: 2022/04/05 17:14:43 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*kill_philosopher(t_philo *p, int pos, long time)
{
	int	i;

	i = 0;
	while (i < p->n_forks)
	{
		pthread_mutex_lock(&p->alive_m[i]);
		p->alive[i] = 0;
		pthread_mutex_unlock(&p->alive_m[i++]);
	}
	pthread_mutex_lock(&p->print_m);
	printf("%ld %d died\n", time, pos + 1);
	pthread_mutex_unlock(&p->print_m);
	return (0);
}

void	philoalloc(t_philo *p, int n_forks)
{
	p->philosopher = malloc(sizeof(pthread_t) * n_forks);
	p->mutex = malloc(sizeof(pthread_mutex_t) * n_forks);
	p->starve = malloc(sizeof(long) * n_forks);
	p->starve_m = malloc(sizeof(pthread_mutex_t) * n_forks);
	p->alive = malloc(sizeof(int) * n_forks);
	p->alive_m = malloc(sizeof(pthread_mutex_t) * n_forks);
	p->turn = malloc(sizeof(int) * n_forks);
	p->turn_m = malloc(sizeof(pthread_mutex_t) * n_forks);
	pthread_mutex_init(&p->pos_m, NULL);
	pthread_mutex_init(&p->start_m, NULL);
	pthread_mutex_init(&p->dir_m, NULL);
	pthread_mutex_init(&p->print_m, NULL);
	pthread_mutex_init(&p->lunch_m, NULL);
}

void	t_philoinit(t_philo *p, int *dir, int size)
{
	int	i;

	i = 0;
	p->n_forks = dir[0];
	p->t_to_die = dir[1];
	p->t_to_eat = dir[2];
	p->t_to_sleep = dir[3];
	p->n_to_eat = -1;
	if (size == 5)
		p->n_to_eat = dir[4];
	philoalloc(p, dir[0]);
	while (i < dir[0])
		p->alive[i++] = 1;
	p->lunchs = 1;
	p->pos = 0;
	i = 0;
	while (i < dir[0])
	{
		pthread_mutex_init(&(p->mutex[i]), NULL);
		pthread_mutex_init(&(p->starve_m[i]), NULL);
		pthread_mutex_init(&p->alive_m[i], NULL);
		pthread_mutex_init(&p->turn_m[i++], NULL);
	}
}
