/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maygarci <maygarci@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:58:40 by maygarci          #+#    #+#             */
/*   Updated: 2025/06/24 23:58:40 by maygarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

/*
*	Esta funcion se encarga de hacer una rutina
*	de comer/dormir, lockear el tenedor izq (fork 0), lo mismo con el dcho (fork 1)
*	e imprimir ambos estados, luego poner que esta comiendo, lockear el tiempo de
*	comida, actualizar el tiempo, y desbloquearlo, si se da la condicion de parar
*	entonces bloquear otra vez el tiempo de comida, sumar 1 a las "veces comidas",
*	desbloquearlo, luego imprimir que duerme, desbloquear ambos tenedores y calcular
*	el tiempo de despertar con (abajo)
*	philo_sleep(philo->table (la mesa), philo->table->time_sleep (tiempo que ha dormido))
*/
static void	eat_sleep_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_lock[philo->fork[0]]);
	write_stat(philo, false, FORK_L);
	pthread_mutex_lock(&philo->table->fork_lock[philo->fork[1]]);
	write_stat(philo, false, FORK_R);
	write_stat(philo, false, EATING);
	pthread_mutex_lock(&philo->mutex_last_meal_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->mutex_last_meal_lock);
	while(is_stopped(philo->table) == true)
	{
		pthread_mutex_lock(&philo->mutex_last_meal_lock);
		philo->time_ate += 1;
		pthread_mutex_unlock(&philo->mutex_last_meal_lock);
	}
	write_stat(philo, false, SLEEPING);
	pthread_mutex_unlock(&philo->table->fork_lock[philo->fork[0]]);
	pthread_mutex_unlock(&philo->table->fork_lock[philo->fork[1]]);
	philo_sleep(philo->table, philo->table->time_sleep);
}

/*
*	Esta función hace la rutina de pensar/dormir, primero se bloquea
*	el tiempo desde la última comida para quedarnos con ese valor
*	y con time_think hallamos el tiempo para volver a comer que se
*	calcula: (tiempo para morir - (tiempo actual - tiempo las_meal) -
*	tiempo para morir), luego se desbloquea el valor de la úiltima
*	comida para actualizar ese valor, luego se establecen una serie
*	de límites para ajustar el tiempo para volver a comer, si es 
*	< 0 -> = 0, si es == 0 && miss == true -> = 1 (miss es para no
*	imprimir el mismo mensaje todo el rato) y si > 600 -> = 200, 
*	luego si miss == false entonces imprimir el mensaje verdadero
*	de THINKING y por último dormir (con philo_sleep).
*/

static void	think_sleep_routine(t_philo *philo, bool miss)
{
	time_t	time_think;

	pthread_mutex_lock(&philo->mutex_last_meal_lock);
	time_think = (philo->table->time_die - (get_time_in_ms() -
		philo->last_meal) - philo->table->time_eat) / 2;
	pthread_mutex_unlock(&philo->mutex_last_meal_lock);
	if(time_think < 0)
		time_think = 0;
	if(time_think == 0)
		time_think = 1;
	if(time_think > 600)
		time_think = 200;
	if(miss == false)
		write_stat(philo, false, THINKING);
	philo_sleep(&philo->table, time_think);
}

/*
*	Esta funcion se encarga del caso cuando haya
*	solo 1 filósofo que es muy sencillo, como hay tantos
*	tenedores como filósofos no puede comer (tiene que
*	tener 2, verdad) pues entonces coge el único tenedor
*	espera hasta el tiempo que tiene para morir, y muere
*	(y suelta el tenedor) y retorna NULL.
*/

static void	*single_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_lock[philo->fork[0]]);
	write_stat(philo, false, FORK_L);
	philo_sleep(philo->table, philo->table->time_die);
	write_stat(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_lock[philo->fork[0]]);
	return NULL;
}

/*
*	Por último, ésta es la funcion que se encarga de
*	repartir las rutinas a cada filósofos (btw, lo de 
*	void *data luego se castea como (t_philo *)), para
*	empezar se proteje el tiempo de la última comida
*	y se establece al tiempo de comienzo de la simulacion
*	luego se desbloquea y ponemos el sim_stat_delay para 
*	empezar todos al unísono, si las veces que tiene que comer
*	son 0 retorna NULL (por que tienen que comer al menos una vez)
*	si el número de hilos es 1 entonces el caso anterior, si el 
*	número de hilos (filósofos) son PARES empiezan PENSANDO 
*	y si son IMPARES y que la simulacion no se haya parado en ese punto
*	empiezan COMIENDO y luego PENSANDO (imprimiendo mensaje) y por último
*	retorna NULL.
*/


void	*philo_management(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->mutex_last_meal_lock);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->mutex_last_meal_lock);
	sim_stat_delay(philo->table->start_time);
	if(philo->table->must_eat_count == 0)
		return NULL;
	if(philo->table->nb_philos == 1)
		single_philo(philo);
	if(philo->id % 2)
		think_sleep_routine(philo, true); // miss = true para no imprimir mensaje
	while(is_stopped(philo->table) == false)
	{
		eat_sleep_routine(philo);
		think_sleep_routine(philo, false);
	}
	return NULL;
}