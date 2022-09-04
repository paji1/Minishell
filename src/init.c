/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharraz <akharraz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 02:09:50 by tel-mouh          #+#    #+#             */
/*   Updated: 2022/09/04 00:21:22 by akharraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_vars *vars)
{
	vars->buff = NULL;
	vars->base_name = NULL;
	vars->node = NULL;
	vars->tab_cmd = NULL;
}