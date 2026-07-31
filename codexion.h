/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:04:17 by dioppolo          #+#    #+#             */
/*   Updated: 2026/07/31 14:42:54 by diego            ###   ########.fr       */
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
# include <sys/time.h> 


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
	long long start_time;
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
int compare_requests(t_request a, t_request b, int scheduler_type);
void heapify_up(t_heap *heap, int i, int scheduler_type);
void heapify_down(t_heap *heap, int i, int scheduler_type);
void heap_init(t_heap *heap, int capacity);
void heap_push(t_heap *heap, t_request req, int scheduler_type);
t_request heap_pop(t_heap *heap, int scheduler_type);
t_request heap_peek(t_heap *heap);
void heap_clear(t_heap *heap);
void acquire_dongle(t_coder *coder, t_dongle *dongle);
void release_dongle(t_coder *coder, t_dongle *dongle);
void lock_both_dongles(t_coder *coder);
void release_both_dongle(t_coder *coder);
long long get_time_ms(void);
void print_status(t_coder *coder, char *status);
bool is_simulation_running(t_env *env);
void smart_sleep(long long time_in_ms, t_env *env);
void coder_debug(t_coder *coder);
void coder_compile(t_coder *coder);
void coder_refactor(t_coder *coder);
void *coder_routine(void *arg);
bool check_coder_burnout(t_coder *coder);
bool check_all_finished(t_coder *coders, t_env *env);
void *monitor_routine(void *arg);

#endif