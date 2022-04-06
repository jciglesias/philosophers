/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 18:53:17 by jiglesia          #+#    #+#             */
/*   Updated: 2022/04/05 17:14:45 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philoscheck(t_philo *p, int i, struct timeval time, int *dir)
{
	while (i < dir[0])
	{
		gettimeofday(&time, NULL);
		pthread_mutex_lock(&p->starve_m[i]);
		if ((time_ms(p) - p->starve[i]) > (dir[1]))
		{
			pthread_mutex_unlock(&p->starve_m[i]);
			kill_philosopher(p, i, time_ms(p));
			return ;
		}
		pthread_mutex_unlock(&p->starve_m[i]);
		i++;
	}
}

void	livecheck(t_philo *p, struct timeval time)
{
	int	dir[2];

	pthread_mutex_lock(&p->dir_m);
	dir[0] = p->n_forks;
	dir[1] = p->t_to_die;
	pthread_mutex_unlock(&p->dir_m);
	pthread_mutex_lock(&p->lunch_m);
	while (checkalive(p, 0) && p->lunchs)
	{
		pthread_mutex_unlock(&p->lunch_m);
		philoscheck(p, 0, time, dir);
		pthread_mutex_lock(&p->lunch_m);
	}
	pthread_mutex_unlock(&p->lunch_m);
}

void	freephilos(t_philo *p)
{
	free(p->philosopher);
	free(p->mutex);
	free(p->starve);
	free(p->starve_m);
	free(p->alive);
	free(p->alive_m);
	free(p->turn);
	free(p->turn_m);
}

void	lyceum(int *dir, int size)
{
	int				i;
	t_philo			p;
	struct timeval	time;

	t_philoinit(&p, dir, size);
	i = 0;
	gettimeofday(&time, NULL);
	p.start = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	while (i < dir[0])
	{
		p.starve[i] = time_ms(&p);
		if (!(i % 2))
			p.turn[i] = 0;
		else
			p.turn[i] = 1;
		pthread_create(&p.philosopher[i++], NULL, &life, &p);
	}
	livecheck(&p, time);
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
		if (dir[0])
			lyceum(dir, argc - 1);
		return (0);
	}
	printf("ERROR: expected:./philo [#of philosophers] ");
	printf("[time to die] [time to eat] [time to sleep]\n");
	return (0);
}
