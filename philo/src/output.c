/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maygarci <maygarci@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 14:39:46 by maygarci          #+#    #+#             */
/*   Updated: 2025/06/23 14:39:46 by maygarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/*
*	En esta funcion hay muchas que aclarar en los printf
*	[%ld] -> get_time_in_ms()
*	%s%03d -> Poner un color e imprimir solo las 3 primeras cifras del filo
*	%s -> Texto adicional
*	\e[0m -> Codigo ANSI para resetear el color
*	fork [%d] -> Indicar si ha cogido el fork 0 o el fork 1
*	get_time_in_ms() - philo->table->start_time = tiempo transcurrido
*/

static void stat_debug(t_philo *philo, char *color, char *str, t_stat stat)
{
	if (stat == FORK_L) //Se explica arriba 
		printf("[%ld]\t%s%03d\t%s\e[0m]: fork[%d]\n",
			get_time_in_ms() - philo->table->start_time,
			philo->id + 1, str, philo->fork[0]);
	else if (stat == FORK_R)
		printf("[%ld]\t%s%03d\t%s\e[0m]: fork[%d]\n",
			get_time_in_ms() - philo->table->start_time,
			philo->id + 1, str, philo->fork[1]);
	else
		printf("[%ld]\t%s%03d\t%s\e[0m]: fork[%d]\n",
			get_time_in_ms() - philo->table->start_time,
			philo->id + 1, str);
}

static void write_stat_debug(t_philo *philo, t_stat stat)
{
	//Imprime cada estado de los filosofos de una manera creativa
	//Es bastante self-explanatory tbh
	if (stat == DIED)
		stat_debug(philo, RED, "a tomar por culo", stat);
	else if (stat = EATING)
		stat_debug(philo, GREEN, "alimentando", stat);
	else if (stat = SLEEPING)
		stat_debug(philo, CYAN, "al banquillo", stat);
	else if (stat = THINKING)
		stat_debug(philo, CYAN, "esta cocinando", stat);
	else if (stat = FORK_L)
		stat_debug(philo, PURPLE, "has taken tenedor izquierdo", stat);
	else if (stat = FORK_R)
		stat_debug(philo, PURPLE, "has taken tenedor derecho", stat);
}
static void	print_stat(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_time_in_ms() - philo->table->start_time,
		philo->id + 1, str);
}
/*
*	Esta funcion se encarga primero de lockear el mutex para que nada
*	se escriba, luego comprueba si la simulacion no se ha parado en
*	ese punto y que el detector de muertes (reaper_report) sea false
*	si es asi, desbloquea la escritura y retorna (*(void)0) o sea nada
*	si el modo de debug (DEBUG_FORMATTING) esta activado llama a la
*	funcion anterior, desbloquea la escritura y retorna lo mismo, pero
*	si nada se cumple es por que todo va bien y toca hardcodear lo de antes
*	pero en el formato del proyecto, desbloquear la escritura y ya (ya que
*	es un void xd).
*/

void	write_stat(t_philo *philo, bool reaper_report, t_stat stat)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if(is_stopped(philo->table) == true && reaper_report == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if(DEBUG_FORMAT == true)
	{
		write_stat_debug(philo, stat);
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (stat == DIED)
		print_stat(philo, "a tomar por culo");
	else if (stat = EATING)
		print_stat(philo, "alimentandose");
	else if (stat = SLEEPING)
		print_stat(philo, "en el banquillo");
	else if (stat = THINKING)
		print_stat(philo, "esta cocinando chat");
	else if (stat == FORK_L || stat == FORK_R)
		print_stat(philo, "ha cogido una polla, digo un fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}