/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:38:45 by jiglesia          #+#    #+#             */
/*   Updated: 2022/03/14 18:28:48 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		PHILO_H
# define	PHILO_H

# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>

typedef struct	s_philos
{
	pthread_t		philosopher[10000];
	pthread_mutex_t	mutex[10000];
	int				n_inst;
	int				n_forks;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	int				pos;
}				t_philo;

#endif
