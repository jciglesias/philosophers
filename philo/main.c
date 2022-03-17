/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 18:53:17 by jiglesia          #+#    #+#             */
/*   Updated: 2022/03/16 22:46:09 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*life(void *p)
{
	t_philo *tmp;
	int		pos;
	struct timeval	time;

	tmp = (t_philo *)p;
	pos = tmp->pos++;
	pthread_mutex_lock(&tmp->mutex[pos]);
	gettimeofday(&time, NULL);
	printf("%ld philosopher %d has taken a fork\n", time.tv_usec, pos);
	if (pos < tmp->n_forks)
		pthread_mutex_lock(&tmp->mutex[pos + 1]);
	else
		pthread_mutex_lock(&tmp->mutex[0]);
	gettimeofday(&time, NULL);
	printf("%ld philosopher %d is eating\n", time.tv_usec, pos);
	usleep(tmp->t_to_eat * 1000);
	pthread_mutex_unlock(&tmp->mutex[pos]);
	if (pos < tmp->n_forks)
		pthread_mutex_unlock(&tmp->mutex[pos + 1]);
	else
		pthread_mutex_unlock(&tmp->mutex[0]);
	gettimeofday(&time, NULL);
	printf("%ld philosopher %d is sleeping\n", time.tv_usec, pos);
	usleep(tmp->t_to_sleep * 1000);
	return (0);
}

void	lyceum(int *dir, int size)
{
	int	i;
	t_philo	p;

	p.n_inst = size;
	p.n_forks = dir[0];
	p.t_to_die = dir[1];
	p.t_to_eat = dir[2];
	p.t_to_sleep = dir[3];
	p.n_to_eat = -1;
	if (size == 5)
		p.n_to_eat = dir[4];
	p.pos = 0;
	i = 0;
	while (i < dir[0])
		pthread_mutex_init(&(p.mutex[i++]),NULL);
	i = 0;
	while (i < dir[0])
		pthread_create(&p.philosopher[i++], NULL, &life, &p);
	i = 0;
	while (i < dir[0])
		pthread_join(p.philosopher[i++], NULL);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	intmaxmin(char *str)
{
	int	i;
	int	nb;
	int	n;

	if (ft_strlen(str) > 11)
		return (1);
	i = 0;
	nb = 0;
	n = 1;
	if (str[i] == '-')
	{
		i++;
		n = -1;
	}
	while (str[i] && (i < 9 || (n == -1 && i < 10)))
		nb = nb * 10 + (n * (str[i++] - 48));
	if (nb > 214748364 || nb < -214748364)
		return (1);
	else if (nb == 214748364 && ((n == -1 && str[i] > '8') || str[i] > '7'))
		return (1);
	return (0);
}

int	ft_isdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		if (i > 8 && intmaxmin(str))
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(char *str)
{
	int	i;
	int	nb;
	int	n;

	i = 0;
	n = 1;
	if (str[i] == '-')
	{
		i++;
		n = -1;
	}
	nb = 0;
	while (str[i])
		nb = nb * 10 + (n * (str[i++] - 48));
	return (nb);
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
