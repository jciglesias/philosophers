/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 18:53:17 by jiglesia          #+#    #+#             */
/*   Updated: 2022/03/21 13:25:01 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*kill_philosopher(t_philo *p, int pos, long time)
{
	printf("%ld philosopher %d died\n", time, pos + 1);
	pthread_mutex_unlock(&p->mutex[pos]);
	if (pos < (p->n_forks - 1))
		pthread_mutex_unlock(&p->mutex[pos + 1]);
	else
		pthread_mutex_unlock(&p->mutex[0]);
	p->alive = 0;
	return (0);
}

void	t_philoinit(t_philo *p, int *dir, int size)
{
	p->n_inst = size;
	p->n_forks = dir[0];
	p->t_to_die = dir[1];
	p->t_to_eat = dir[2];
	p->t_to_sleep = dir[3];
	p->n_to_eat = -1;
	p->alive = 1;
	p->lunchs = 1;
	if (size == 5)
		p->n_to_eat = dir[4];
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
			if ((time_ms(time) - p->starve[i]) >= (p->t_to_die))
			{
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
	while (i < dir[0])
	{
		gettimeofday(&time, NULL);
		p.starve[i] = time.tv_usec;
		pthread_create(&p.philosopher[i++], NULL, &life, &p);
		usleep(5);
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
		lyceum(dir, argc - 1);
		return (0);
	}
	printf("ERROR: expected:./philo [#of philosophers] ");
	printf("[time to die] [time to eat] [time to sleep]\n");
	return (0);
}
