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
#  define DEBUG_FORMAT 1
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
	pthread_mutex_t	mutex_last_meal_lock; //Mutex para proteger el acceso a last_meal
	time_t			last_meal; //Tiempo de la última comida del ultimo philo
	t_table			*table; //Referencia a la mesa
}	t_philo;

typedef enum e_stat //Representa el estdo en valores numéricos (enum)
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	FORK_R = 4,
	FORK_L = 5
}	t_stat;


//ERROR_HANDLING (types_exit.c)
void	*free_table(t_table *table);
void	kill_mutex(t_table *table);
int		msg(char *str, char *detail, int exit_no);
int		err_fail(char *str, char *detail, t_table *table);
void	*err_null(char *str, char *detail, t_table *table);

//PARSING (parse.c)
int	ulltoi(char *str);
bool	valid_input(int ac, char **av);

//INIT (init.c)
t_table	*init_table(int ac, char **av, int i);

//TIME (time.c)
time_t	get_time_in_ms(void);
void	philo_sleep(t_table *table, time_t is_sleeping);
void	sim_stat_delay(time_t start_time);

//GRIM REAPER (grim_reaper.c)
bool	is_stopped(t_table *table);
void	*grim_reaper(void *data);

//OUTPUT (output.c)
void	write_stat(t_philo *philo, bool reaper_report, t_stat stat);
#endif