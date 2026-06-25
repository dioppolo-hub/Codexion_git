/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:04:17 by dioppolo          #+#    #+#             */
/*   Updated: 2026/06/25 10:49:04 by dioppolo         ###   ########.fr       */
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

bool parcing1(int argc, char** argv);
bool parce_n_coders(int n_coder);
bool parce_t_burnout(int t_burnout);
bool parce_t_compile(int t_compile);
bool parce_t_debug(int t_debug);
bool parce_t_refactor(int t_refactor);
bool parce_n_comp(int n_comp);
bool parce_dongle_cool(int d_cooldown);
bool parce_scheduler(char *scheduler);

#endif