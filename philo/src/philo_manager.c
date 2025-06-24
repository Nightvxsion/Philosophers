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