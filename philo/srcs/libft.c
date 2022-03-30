/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiglesia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/20 15:54:40 by jiglesia          #+#    #+#             */
/*   Updated: 2022/03/30 18:43:57 by jiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
