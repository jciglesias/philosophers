/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:38:45 by jiglesia          #+#    #+#             */
/*   Updated: 2022/04/11 22:27:09 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>

typedef struct s_philos
{
	pthread_t		philosopher;
	int				n_forks;
	int				n_to_eat;
	long			t_to_die;
	long			t_to_eat;
	long			t_to_sleep;
	int				alive;
	pthread_mutex_t	alive_m;
	long			starve;
	pthread_mutex_t	starve_m;
	int				turn;
	pthread_mutex_t	turn_m;
}	t_philo;

typedef struct s_table
{
	t_philo			*philo;
	pthread_mutex_t	*mutex;
	int				n_forks;
	long			start;
	pthread_mutex_t	start_m;
	int				pos;
	pthread_mutex_t	pos_m;
	int				lunchs;
	pthread_mutex_t	lunch_m;
	pthread_mutex_t	print_m;
}	t_table;

int		ft_atoi(char *str);
int		ft_isdigit(char *str);
int		intmaxmin(char *str);
int		ft_strlen(char *str);
void	*kill_philosopher(t_table *p, int pos, long time);
void	*life(void *p);
long	time_ms(t_table *p);
int		checkalive(t_philo	*p);
void	checkturn(t_philo *p, int e);
void	takefork(t_table *tmp, int pos, int philos, int side);
void	printline(t_table *tmp, int pos, char *str);
void	t_tableinit(t_table *p, int *dir, int size);

#endif
