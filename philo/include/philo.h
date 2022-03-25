/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:38:45 by jiglesia          #+#    #+#             */
/*   Updated: 2022/03/26 00:01:45 by jiglesia         ###   ########.fr       */
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
	pthread_t		*philosopher;
	pthread_mutex_t	*mutex;
	long			start;
	int				n_forks;
	int				n_to_eat;
	long			t_to_die;
	long			t_to_eat;
	long			t_to_sleep;
	int				pos;
	int				alive;
	long			*starve;
	int				lunchs;
}	t_philo;

int		ft_atoi(char *str);
int		ft_isdigit(char *str);
int		intmaxmin(char *str);
int		ft_strlen(char *str);
void	*kill_philosopher(t_philo *p, int pos, long time);
void	*life(void *p);
long	time_ms(struct timeval time);

#endif
