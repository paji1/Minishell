/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   startend.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-mouh <tel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 22:41:02 by tel-mouh          #+#    #+#             */
/*   Updated: 2022/09/08 20:43:11 by tel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int is_spacial_space(char *buff, int i)
{
    return (is_special(&buff[i]) || is_space(buff[i]));
}
/**
 * @brief check the end of tokens
 * 
 * @param buff return of readline 
 * @param i count inside parse function
 * @param sub where is start and end of token
 * @return int end or not
 */
int end_token(char *buff, int i, t_sub *sub)
{
    if (is_spacial_space(buff, i - 1) && is_spacial_space(buff, i))
        return 0;
    if (is_space(buff[i]) && buff[i - 1] != '&' && buff[i - 1] != '|')
        return (sub->end = i, 1);
	else if(is_special(&buff[i]))
		return (sub->end = i, 1);
    else if (!buff[i + 1])
        return (sub->end = i + 1, 1);
    return (0);
}

/**
 * @brief intialize the sub start
 * 
 * @param buff return of readline
 * @param i count inside parse function
 * @param sub where is start and end of token
 */
void start_token(char *buff, int i, t_sub *sub)
{
    if (is_space(buff[i]))
        sub->start = i + 1;
}

/**
 * @brief whene the parse loop exit white quote at end
 * 
 * @param buff return of readline
 * @param i count inside parse function
 * @param sub where is start and end of token
 */
void handle_last(t_vars *vars, int i, t_sub *sub)
{
    if (end_token(vars->buff, i, sub) && ft_strchr("\"'", vars->buff[i - 1]))
        handle_token(lexer(vars->buff, sub), vars);
}