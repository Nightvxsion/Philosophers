/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maygarci <maygarci@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:13:21 by maygarci          #+#    #+#             */
/*   Updated: 2025/06/13 15:13:21 by maygarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static pthread_mutex_t *init_fork(t_table *table)
{
	pthread_mutex_t	*fork; //Inicializamos con un mutex
	unsigned int	i; //Iterador

	fork = malloc(sizeof(pthread_mutex_t) * table->nb_philos); //Alojamos memoria en funcion de los filosofos de la mesa
	if(!fork)
		return(err_null(ERR_MALLOC, NULL, 0)); //Sino, devuelve la situacion de un error NULL
	i = 0;
	while(i < table->nb_philos) //Mientras hayan filosofos en la mesa (table->nb_philos)
	{
		if(pthread_mutex_init(&fork[i], 0) != 0) //Si al inicializar el codigo de salida NO ES 0 entonces es un error
			return (err_null(ERR_MUTEX, NULL, 0)); //Sino, devuelve la situacion de un error MUTEX
		i++;
	}
	return fork; //Retorna el tenedor
}

static void	forks_mngmt(t_philo *philo) //Ver dibujo
{
	philo->fork[0] = philo->id; //Philo->id significa el número de orden de filosofos en la mesa
	philo->fork[1] = (philo->id + 1) % philo->table->nb_philos;
	if(philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->table->nb_philos;
		philo->fork[1] = philo->id;
	}
}

static t_philo **philos_init(t_table *table)
{
	t_philo **philos;
	unsigned int i;
	philos = malloc(sizeof(t_philo) * table->nb_philos);
	if(!philos)
		return(err_null(ERR_MALLOC, NULL, 0)); //Sino, devuelve la situacion de un error NULL
	i = 0;
	while(i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1); //Alojamos memoria por cada filosofo (1 iteracion/filo)
		if(!philos[i])
			return(err_null(ERR_MALLOC, NULL, 0)); //Si algun filosofo no se puede alojar
		//Si accedemos al mutex de la ultima comida y sale con 0 significa que se ha creado correctamente
		if(pthread_mutex_init(&philos[i]->mutex_last_meal_lock, 0) != 0)
			return(err_null(ERR_MUTEX, NULL, 0));
		philos[i]->table = table; //Asigna el i-ésimo filosofo a la mesa compartida (t_table)
		philos[i]->id = i; //Se asigna un ID único a cada filosofo
		philos[i]->time_ate = 0; //Asignado al número de veces que el filósofo ha comido
		forks_mngmt(philos[i]);
		i++;
	}
	return philos;
}

static bool init_mutexes(t_table *table)
{
	table->fork_lock = init_fork(table);
	if (!table->fork_lock) //Si no se puede crear un mutex de los tenedores
		return false; //No se puede crear un mutex
	//Protege de que solamente sola esa variable indique de parar la simulacion o no
	if (pthread_mutex_init(&table->sim_stat_lock, 0) != 0) 
		return (err_null(ERR_MUTEX, NULL, table));
	//Protege de que el msg de escritura se imprima una vez por rutina
	if (pthread_mutex_init(&table->write_lock, 0) != 0)
		return (err_null(ERR_MUTEX, NULL, table));
	return true;
}

t_table *init_table(int ac, char **av, int i)
{
	t_table *table;

	table = malloc(sizeof(t_table) * 1); //Alojamos para una sola mesa
	if(!table)
		return(err_null(ERR_MALLOC, NULL, 0));
	//Las 4 lineas siguientes convierten todos los ULL a int (de los tiempos y el num. de filosofos)
	table->nb_philos = ulltoi(av[i++]);
	table->time_eat = ulltoi(av[i++]);
	table->time_sleep = ulltoi(av[i++]);
	table->time_die = ulltoi(av[i++]);
	table->must_eat_count = -1; //Parametro opcional
	if(ac - 1 == 5) //Si hay si o si 5 argumentos entonces el param opcional se convierte en obligatorio
		table->must_eat_count = ulltoi(av[i]); //Se convierte el numero de veces que tiene que comer (en ULL) en int
	table->philos = philos_init(table); //Inicializacion de filosofos
	if(!table->philos)
		return NULL;
	if(!init_mutexes(table)) //Si la inicializacion de los mutexes globales devuelve false entonces NULL
		return NULL;
	table->sim_stat = false; //Estado en ejecución
	return table;
}
