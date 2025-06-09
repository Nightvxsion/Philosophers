/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maygarci <maygarci@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 14:21:25 by maygarci          #+#    #+#             */
/*   Updated: 2025/06/09 14:21:25 by maygarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	*free_table(t_table *table)
{
	unsigned int i;
	if(!table)
		return (NULL);
	if (table->fork_lock != NULL)
		free(table->fork_lock);
	if (table->philos != NULL)
	{
		i = 0;
		while(i < table->nb_philos) //Mientras hayan miembros DENTRO DE PHILOS (nb_philos)
		{
			if(table->philos[i] != NULL) //Liberar los miembros del arreglo philos
				free(table->philos[i]);
			i++;
		}
		free(table->philos); //Liberar la matriz philos
	}
	free(table); //Liberar la mesa
	return(NULL);
}

void	kill_mutex(t_table *table)
{
	unsigned int	i;

	i = 0;
	while(i < table->nb_philos)
	{
		pthread_mutex_destroy(&table->fork_lock[i]);//Destruye el acceso a LOS TENEDORES
		pthread_mutex_destroy(&table->philos[i]->mutex_last_meal_lock); //Destruye el acceso a LAS ÚLTIMAS COMIDAS
		i++;
	}
	pthread_mutex_destroy(&table->write_lock);//Destruye el acceso EL ÚNICO MODO DE ESCRITURA
	pthread_mutex_destroy(&table->sim_stat_lock); //Destruye el acceso al ÚNICO ESTADO DE LA SIMULACIÓN
}

int msg(char *str, char *detail, int exit_no)
{
	if(!detail) //Si no tiene mas información del error
		printf(str, PROG_NAME); //Imprime solo PROG_NAME
	else
		printf(str, PROG_NAME, detail); //Sino, lo mismo pero añadiendo mas info
	return (exit_no); //Por ultimo retorna el número de error
}

int err_fail(char *str, char *detail, t_table *table)
{
	if(table != NULL)
		free_table(table);
	return(msg(str, detail, 0)); //El 0 indica salida de fallo
}

void *err_null(char *str, char *detail, t_table *table)
{
	if(table != NULL)
		free_table(table);
	msg(str, detail, EXIT_FAILURE); //Lo mismo que antes solo que retorna EXIT_FAILURE COMO ERROR
	return (NULL);
}