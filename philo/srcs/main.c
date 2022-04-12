/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 18:53:17 by jiglesia          #+#    #+#             */
/*   Updated: 2022/04/12 19:47:27 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philoscheck(t_table *p, int i, struct timeval time, int *dir)
{
	while (i < dir[0])
	{
		gettimeofday(&time, NULL);
		pthread_mutex_lock(&p->philo[i].starve_m);
		if ((time_ms(p) - p->philo[i].starve) > (dir[1]))
		{
			pthread_mutex_unlock(&p->philo[i].starve_m);
			kill_philosopher(p, i, time_ms(p));
			return (0);
		}
		pthread_mutex_unlock(&p->philo[i].starve_m);
		i++;
	}
	return (1);
}

void	livecheck(t_table *p, struct timeval time, int *dir)
{
	int	alive;

	alive = 1;
	pthread_mutex_lock(&p->lunch_m);
	while (alive && p->lunchs)
	{
		pthread_mutex_unlock(&p->lunch_m);
		alive = philoscheck(p, 0, time, dir);
		usleep(2000);
		pthread_mutex_lock(&p->lunch_m);
	}
	pthread_mutex_unlock(&p->lunch_m);
}

void	freephilos(t_table *p)
{
	free(p->philo);
	free(p->mutex);
}

void	lyceum(int *dir, int size)
{
	int				i;
	t_table			p;
	struct timeval	time;

	t_tableinit(&p, dir, size);
	i = 0;
	gettimeofday(&time, NULL);
	p.start = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	while (i < dir[0])
		pthread_create(&p.philo[i++].philosopher, NULL, &life, &p);
	livecheck(&p, time, dir);
	i = 0;
	while (i < dir[0])
		pthread_join(p.philo[i++].philosopher, NULL);
	freephilos(&p);
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
