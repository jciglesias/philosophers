/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 18:53:17 by jiglesia          #+#    #+#             */
/*   Updated: 2022/03/28 15:11:42 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*kill_philosopher(t_philo *p, int pos, long time)
{
	p->alive = 0;
	printf("%ld %d died\n", time - p->start, pos + 1);
	pthread_mutex_unlock(&p->mutex[pos]);
	if (pos < (p->n_forks - 1))
		pthread_mutex_unlock(&p->mutex[pos + 1]);
	else
		pthread_mutex_unlock(&p->mutex[0]);
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
	p->alive = 1;
	p->lunchs = 1;
	p->pos = 0;
}

void	livecheck(t_philo *p, struct timeval time)
{
	int	i;

	i = 0;
	while (p->alive && p->lunchs)
	{
		i = 0;
		while (i < p->n_forks)
		{
			gettimeofday(&time, NULL);
			if ((time_ms(time) - p->starve[i]) > (p->t_to_die))
			{
				p->alive = 0;
				kill_philosopher(p, i, time_ms(time));
				break ;
			}
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
	p.start = time_ms(time);
	while (i < dir[0])
	{
		gettimeofday(&time, NULL);
		p.starve[i] = time_ms(time);
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
		lyceum(dir, argc - 1);
		return (0);
	}
	printf("ERROR: expected:./philo [#of philosophers] ");
	printf("[time to die] [time to eat] [time to sleep]\n");
	return (0);
}
