/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maygarci <maygarci@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 12:49:10 by maygarci          #+#    #+#             */
/*   Updated: 2025/06/19 12:49:10 by maygarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	set_sim_stop_flag(t_table *table, bool stat)
{
	pthread_mutex_lock(&table->sim_stat_lock); //Bloquea la simulacion para que no se vea alterada
	table->sim_stat = stat; //Se ajusta el estado de la simulacion de ese momento al nodo correspondiente
	pthread_mutex_unlock(&table->sim_stat_lock); //Se desbloquea el mismo mutex
}

bool	is_stopped(t_table *table)
{
	bool	stop_flag = false;
	pthread_mutex_lock(&table->sim_stat_lock); //lo mismo que en la anterior, ademas de evitar race conditions
	if(table->sim_stat == true) //Si en la aterior sale true, entonces se debe parar comprobandolo con este bool
		stop_flag = true;
	pthread_mutex_unlock(&table->sim_stat_lock); //Se desbloquea el mismo mutex
	return stop_flag;
}