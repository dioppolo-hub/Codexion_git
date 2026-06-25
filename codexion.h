/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:04:17 by dioppolo          #+#    #+#             */
/*   Updated: 2026/06/25 11:14:57 by dioppolo         ###   ########.fr       */
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

bool parcing_1(int argc, char** argv);
bool parce_n_coders(char *n_coder);
bool parce_t_burnout(char *t_burnout);
bool parce_t_compile(char *t_compile);
bool parce_t_debug(char *t_debug);
bool parce_t_refactor(char *t_refactor);
bool parce_n_comp(char *n_comp);
bool parce_dongle_cool(char *d_cooldown);
bool parce_scheduler(char *scheduler);

#endif