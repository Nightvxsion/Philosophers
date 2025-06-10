/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maygarci <maygarci@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:54:03 by maygarci          #+#    #+#             */
/*   Updated: 2025/06/10 15:54:03 by maygarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static bool is_digit(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if(str[i] < '0' || str[i] > '9') //Si el valor sobrepasa de 0 o 9 
			return false; //No es un digito válido
		i++;
	}
	return true; //Si no pasa por esa condición, es un número
}

int	ulltoi(char *str) //unsigned long long to int 
{
	unsigned long long int	n; //Límite -> 18,446,744,073,709,551,615
	int						i;

	i = 0;
	n = 0;
	while(str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		n *= 10 + (str[i] - '0');
		i++;
	}
	if(n > INT_MAX) //Si supera el limite de INT_MAX (n convertido a int)
		return -1;
	return((int)n); //Retornar el valor correcto casteado a int
}

bool	valid_input(int ac, char **av)
{
	int i;
	int nb; //Aplicar ULLTOI
	i = 1;
	while(i < ac)
	{
		if(!is_digit(av[i])) //Si el parseo de digitos no pasa
			return (msg(ERR_INT_INPUT, av[i], false));
		nb = ulltoi(av[i]); //Convertimos ULLTOI
		if(i == 1 && (nb < 0 || nb > MAX_PHILOS)) //Si es solo un numero y al convertir no es < 0 o mas que MAX_PHILOS
			return (msg(ERR_PHILO_OVERFLOW, STR_MAX_PHILOS, false));
		if(i != 1 && nb == -1)
			return (msg(ERR_INT_INPUT, av[i], false));
		i++;
	}
	return true; //Es valido
}
