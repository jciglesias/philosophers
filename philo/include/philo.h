/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <jiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:38:45 by jiglesia          #+#    #+#             */
/*   Updated: 2022/03/13 23:46:12 by jiglesia         ###   ########.fr       */
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
	pthread_t		philosopher[2];//[2147483647];
	pthread_mutex_t	mutex[2];//[2147483647];
	int				index;
}				t_philo;

#endif
