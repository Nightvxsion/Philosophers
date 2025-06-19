/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maygarci <maygarci@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 12:42:15 by maygarci          #+#    #+#             */
/*   Updated: 2025/06/19 12:42:15 by maygarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

time_t	get_time_in_ms(void)
{
	struct timeval tv; //Estructura haciendo referencia a la libreria time.h
	gettimeofday(&tv, NULL); //Conseguimos el tiempo de la simulacion sin restricciones
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)); //Convertimos a segundos y despues a ms (μsec)
}

void	philo_sleep(t_table *table, time_t is_sleeping)
{
	time_t wake_up;

	wake_up = get_time_in_ms() + is_sleeping; //Tiempo actual + lo que esta durmiendo = Tiempo en el que se despierta
	while(get_time_in_ms() < wake_up) //Mientras que los filosofos esten dormidos comprueba el resto
	{
		if(is_stopped(table)) //Comprueba por cada iteracion si se detiene la simulacion, e interrumpe el sueño de los filos
			break;
		usleep(100); //Cada filosofo duerme 100 microseg hasta esperar una accion (y no sobrecargar la CPU)
	}
}

void	sim_stat_delay(time_t start_time)
{
	while(get_time_in_ms() < start_time)
		continue; //El continue vuelve al inicio del bucle, mientras no se alcanze start_time
}