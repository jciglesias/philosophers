/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 18:53:17 by jiglesia          #+#    #+#             */
/*   Updated: 2022/03/29 12:59:08 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*kill_philosopher(t_philo *p, int pos, long time)
{
	pthread_mutex_lock(&p->alive_m);
	p->alive = 0;
	pthread_mutex_unlock(&p->alive_m);
	printf("%ld %d died\n", time, pos + 1);
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
	p->philosopher = malloc(sizeof(pthread_t) * dir[0]);
	p->mutex = malloc(sizeof(pthread_mutex_t) * dir[0]);
	p->starve = malloc(sizeof(long) * dir[0]);
	p->starve_m = malloc(sizeof(pthread_mutex_t) * dir[0]);
	p->alive = 1;
	pthread_mutex_init(&p->alive_m, NULL);
	p->lunchs = 1;
	p->pos = 0;
	pthread_mutex_init(&p->pos_m, NULL);
	pthread_mutex_init(&p->start_m, NULL);
	pthread_mutex_init(&p->dir_m, NULL);
}

void	livecheck(t_philo *p, struct timeval time)
{
	int	i;
	int	dir[2];

	i = 0;
	pthread_mutex_lock(&p->dir_m);
	dir[0] = p->n_forks;
	dir[1] = p->t_to_die;
	pthread_mutex_unlock(&p->dir_m);
	while (checkalive(p) && p->lunchs)
	{
		i = 0;
		while (i < dir[0])
		{
			gettimeofday(&time, NULL);
			pthread_mutex_lock(&p->starve_m[i]);
			if ((time_ms(p) - p->starve[i]) > (dir[1]))
			{
				pthread_mutex_unlock(&p->starve_m[i]);
				pthread_mutex_lock(&p->alive_m);
				p->alive = 0;
				pthread_mutex_unlock(&p->alive_m);
				kill_philosopher(p, i, time_ms(p));
				break ;
			}
			pthread_mutex_unlock(&p->starve_m[i]);
			i++;
		}
	}
}

void	lyceum(int *dir, int size)
{
	int				i;
	t_philo			p;
	struct timeval	time;

	t_philoinit(&p, dir, size);
	i = 0;
	while (i < dir[0])
		pthread_mutex_init(&(p.mutex[i++]), NULL);
	i = 0;
	gettimeofday(&time, NULL);
	p.start = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	while (i < dir[0])
	{
		pthread_mutex_lock(&p.starve_m[i]);
		p.starve[i] = time_ms(&p);
		pthread_mutex_unlock(&p.starve_m[i]);
		pthread_create(&p.philosopher[i++], NULL, &life, &p);
		usleep(10);
	}
	livecheck(&p, time);
	i = 0;
	while (i < dir[0])
		pthread_join(p.philosopher[i++], NULL);
	free(p.philosopher);
	free(p.mutex);
	free(p.starve);
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
		if (dir[0])
			lyceum(dir, argc - 1);
		return (0);
	}
	printf("ERROR: expected:./philo [#of philosophers] ");
	printf("[time to die] [time to eat] [time to sleep]\n");
	return (0);
}
