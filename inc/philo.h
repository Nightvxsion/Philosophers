#ifndef PHILO_H
#define PHILO_H

//LIBS
# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/time.h>
# include <limits.h>

//ERR MSGS
# define MAX_PHILOS 100
# define STR_MAX_PHILOS "100"

# ifndef DEBUG_FORMAT
#  define DEBUG_FORMAT 0
# endif

# define NC		"\e[0m"
# define RED	"\e[31m"
# define GREEN	"\e[32m"
# define PURPLE	"\e[35m"
# define CYAN	"\e[36m"

# define PROG_NAME "philo:"
# define USAGE "%s Usage:\n ./philo <nº of philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> [times_each_philo_eat]\n"
# define ERR_INT_INPUT "%s invalid input!\n Must be a value \
between 0 and INT_MAX (2147483647)\n"
# define ERR_PHILO_OVERFLOW "%s invalid input!\n Must be a \
value between 1 and %s philos\n"
# define ERR_THREAD "%s ERROR! Couldn't create a thread\n"
# define ERR_MALLOC "%s ERROR! Couldn't allocate memory\n"
# define ERR_MUTEX "%s ERROR! Couldn't create a mutex\n"

typedef struct t_philo s_philo; //Referencia a la lista philo por mencionarla antes de usarla despues

typedef struct s_table
{
	time_t			start_time; //Tiempo de la simulacion
	unsigned int	nb_philos; //Numero de filosofos
	pthread_t		grim_reaper; //Cosa rara de la muerte xdd
	time_t			time_sleep; //Tiempo en dormir
	time_t			time_eat; //Tiempo en comer
	time_t			time_die; //Tiempo en morir
	int				must_eat_count; //Veces que debe comer el philo
	bool			sim_stat; //Estado de la simulacion
	pthread_mutex_t	sim_stat_lock; //Proteger el estado de la simulacion
	pthread_mutex_t	write_lock; //Proteger el estado de escritura
	pthread_mutex_t	*fork_lock; //Proteger el array de escritura (la matriz)
	t_philo			**philos; //Referencia a philos
}	t_table;

typedef struct s_philo
{
	pthread_t		thread; //Hilo en ejecucion
	unsigned int	id; // Identificador del hilo
	unsigned int	time_ate; //Tiempo por comida
	unsigned int	fork[2]; //Dos tenedores/philo
	pthread_mutex_t	mutex_time_lock; // Tiempo que se queda el tenedor lockeado
	time_t			last_meal; //Ultima comida del ultimo philo
	t_table			*table; //Referencia a la mesa
}	t_philo;

typedef enum e_stat
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	FORK_R = 4,
	FORK_L = 5
}	t_stat;

#endif