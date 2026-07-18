/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:04:17 by dioppolo          #+#    #+#             */
/*   Updated: 2026/07/18 14:54:43 by diego            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <pthread.h>
# include <limits.h>
# include <string.h>

typedef struct s_request {
	int coder_id;
	long long request_time;
	long long deadline;
} t_request;

typedef struct s_heap {
	t_request *request;
	int size;
	int capacity;
} t_heap;

typedef struct s_dongle {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int id;
	long long last_released_time; //Per gestire il cooldown
	// Coda di priorità (heap) per gestire le richieste FIFO/EDF
	t_heap heap;
} t_dongle;

typedef struct s_env {
	int num_coders;
	long long t_burnout;
	long long t_debug;
	long long t_compile;
	long long t_refactor;
	int req_compiles;
	long long cooldown;
	int scheduler_type; // 0 per FIFO, 1 per EDF
	long long start_tipe;
	pthread_mutex_t write_mutex; //mutex per non accavallare i printf
	int simulation_running; //flag per bloccare tutto
	pthread_mutex_t sim_mutex;
	t_dongle *dongles; //array di dongle
} t_env;

typedef struct s_coder {
	int id;
	int compile_count;
	long long last_compile_start;
	t_env *env;
	t_dongle *left_dongle;
	t_dongle *right_dongle;
} t_coder;


bool parcing_1(int argc, char** argv);
bool parce_n_coders(char *n_coder);
bool parce_t_burnout(char *t_burnout);
bool parce_t_compile(char *t_compile);
bool parce_t_debug(char *t_debug);
bool parce_t_refactor(char *t_refactor);
bool parce_n_comp(char *n_comp);
bool parce_dongle_cool(char *d_cooldown);
bool parce_scheduler(char *scheduler);
bool is_valid_int(char *str, bool allow_zero);


#endif