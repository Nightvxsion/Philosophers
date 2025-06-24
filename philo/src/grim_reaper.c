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

static bool kill_philo(t_philo *philo)
{
	time_t time;
	//si tarda por ejemplo 20 segundos desde su ultima comida y tiene 10 segundos para morir -> 20 > 10 philo muere
	time = get_time_in_ms();
	if((time - philo->last_meal) >= (time - philo->table->time_die))
	{
		set_sim_stop_flag(philo->table, true);
		write_stat(philo, true, DIED);
		pthread_mutex_unlock(&philo->mutex_last_meal_lock);
		return true;
	}
	return false;
}

/*
*	En esta funcion hay que establecer una variable bool
*	para ver si ha comido lo suficiente (true) y sino (false)
*	y mientras hayan filosofos en la mesa protege el mutex
*	de la última comida y luego con kill_philo comprueba
*	si ha muerto el filosofo DE CADA ITERACION (philos[i])
*	si es así retorna true.
*	Si ha comido al menos una vez entonces comprueba
*	si las veces que ha comido (philos[i]->times_ate) ES MENOR
*	que las veces que debe comer (philos[i]->must_eat_count) y
*	claramente casteado con unsigned int, si es así asigna la
*	varible de "all_ate" a false (ya que no han comido lo suficiente
*	para terminar el programa)
*	Y si ha comido al menos una vez Y la variable "all_ate" es true
*	(es decir, que han comido lo suficiente) entonces para la simulacion
*	con set_sim_stop_flag(la mesa y true) y retorna trua ya que han comido
*	lo que debian, luego desbloquear el tiempo de la última comida y pasar al
*	siguiente filosofo, y si no ocurre nada de lo anterior retorna false
*	(la simulación continua)
*/

static bool	end_condition(t_table *table)
{
	bool	all_ate;
	int		i;
	i = 0;
	all_ate = true;
	while(i < table->nb_philos)
	{
		pthread_mutex_lock(table->philos[i]->mutex_last_meal_lock);
		if(kill_philo(table->philos[i]))
			return true;
		if(table->must_eat_count != 1)
			if(table->philos[i]->time_ate < (unsigned int)table->must_eat_count)
				all_ate = false;
		pthread_mutex_unlock(table->philos[i]->mutex_last_meal_lock);
		i++;
	}
	if (table->must_eat_count != 1 && all_ate == true)
	{
		set_sim_stop_flag(table, true);
		return true;
	}
	return false;
}

/*
*	Esta función supervisa el estado de todos los filósofos
*	primero hacemos referencia a la mesa (t_table) casteando
*	un puntero a data, luego si el numero de veces que debe
*	comer es 0 entonces se acaba inmediatamente, mientras true
*	entonces si todos han comido suficiente (es decir, si retorna
*	true la funcion anterior) entonces retorna NULL y sino espera
*/

void	*grim_reaper(void *data)
{
	t_table	*table;
	table = (t_table *)data; //Casteo
	if(table->must_eat_count == 0)
		return NULL;
	set_sim_stop_flag(table, false);
	sim_stat_delay(table->start_time);
	while(true)
	{
		if(end_condition(table) == true)
			return NULL;
		usleep(1000);
	}
	return NULL;
}