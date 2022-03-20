/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 18:53:17 by jiglesia          #+#    #+#             */
/*   Updated: 2022/03/20 16:15:22 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*kill_philosopher(t_philo *p, int pos, int time)
{
	printf("%d philosopher %d died\n", time, pos + 1);
	pthread_mutex_unlock(&p->mutex[pos]);
	if (pos < (p->n_forks - 1))
		pthread_mutex_unlock(&p->mutex[pos + 1]);
	else
		pthread_mutex_unlock(&p->mutex[0]);
	p->alive = 0;
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
	tmp->starve[pos] = time.tv_usec;
	lunchs = tmp->n_to_eat;
	while (lunchs && tmp->alive)
	{
		if (!tmp->alive)
			return (0);
		pthread_mutex_lock(&tmp->mutex[pos]);
		gettimeofday(&time, NULL);
		printf("%ld philosopher %d has taken a fork\n", time.tv_usec / 1000, pos + 1);
		if (pos < (tmp->n_forks - 1))
			pthread_mutex_lock(&tmp->mutex[pos + 1]);
		else
			pthread_mutex_lock(&tmp->mutex[0]);
		gettimeofday(&time, NULL);
		tmp->starve[pos] = time.tv_usec;
		printf("%ld philosopher %d is eating\n", time.tv_usec / 1000, pos + 1);
		lunchs--;
		usleep(tmp->t_to_eat * 1000);
		pthread_mutex_unlock(&tmp->mutex[pos]);
		if (pos < (tmp->n_forks - 1))
			pthread_mutex_unlock(&tmp->mutex[pos + 1]);
		else
			pthread_mutex_unlock(&tmp->mutex[0]);
		gettimeofday(&time, NULL);
		if (!tmp->alive)
			return (0);
		printf("%ld philosopher %d is sleeping\n", time.tv_usec / 1000, pos + 1);
		if ((time.tv_usec - tmp->starve[pos]) > (tmp->t_to_die * 1000 - tmp->t_to_sleep))
			return (0);
		usleep(tmp->t_to_sleep * 1000);
		gettimeofday(&time, NULL);
		if (!tmp->alive)
			return (0);
		printf("%ld philosopher %d is thinking\n", time.tv_usec / 1000, pos + 1);
	}
	if (!lunchs)
		tmp->lunchs = 0;
	return (0);
}

void	lyceum(int *dir, int size)
{
	int				i;
	t_philo			p;
	struct timeval	time;

	p.n_inst = size;
	p.n_forks = dir[0];
	p.t_to_die = dir[1];
	p.t_to_eat = dir[2];
	p.t_to_sleep = dir[3];
	p.n_to_eat = -1;
	p.alive = 1;
	p.lunchs = 1;
	if (size == 5)
		p.n_to_eat = dir[4];
	p.pos = 0;
	i = 0;
	while (i < dir[0])
		pthread_mutex_init(&(p.mutex[i++]), NULL);
	i = 0;
	while (i < dir[0])
	{
		gettimeofday(&time, NULL);
		p.starve[i] = time.tv_usec;
		pthread_create(&p.philosopher[i++], NULL, &life, &p);
		usleep(5);
	}
	while (p.alive && p.lunchs)
	{
		i = 0;
		while (i < dir[0])
		{
			gettimeofday(&time, NULL);
			if ((time.tv_usec - p.starve[i]) >= (p.t_to_die * 1000))
			{
				kill_philosopher(&p, i, time.tv_usec / 1000);
				break ;
			}
			i++;
		}
	}
	i = 0;
	while (i < dir[0])
		pthread_join(p.philosopher[i++], NULL);
}

int	main(int argc, char **argv)
{
	int	i;
	int	dir[5];

	if (argc == 5 || argc == 6)
	{
		i = 0;
		while (++i < argc)
		{
			if (!ft_isdigit(argv[i]))
			{
				printf("%s wrong value", argv[i]);
				return (0);
			}
			dir[i -1] = ft_atoi(argv[i]);
		}
		lyceum(dir, argc - 1);
		return (0);
	}
	printf("ERROR: expected:./philo [#of philosophers] ");
	printf("[time to die] [time to eat] [time to sleep]\n");
	return (0);
}
