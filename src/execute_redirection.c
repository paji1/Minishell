/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-mouh <tel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 01:46:25 by tel-mouh          #+#    #+#             */
/*   Updated: 2022/10/22 01:46:26 by tel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	case_so(t_node *node, t_nodeq *q)
{
	if (q->data->token.type >= REDIRECT_SO && q->data->token.type <= HERDOC)
		return (0);
	if (q->next->data->token.type == REDIRECT_SI)
		return (0);
	close(node->file_out);
	if (q->next->data->token.type == REDIRECT_SO)
		node->file_out = open(q->data->token.token, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (q->next->data->token.type == APPEND)
		node->file_out = open(q->data->token.token, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (node->file_out == -1)
		return (-1);
	return (0);
}

static int	case_si(t_node *node, t_nodeq *q)
{
	static int lock;

	if ((q->data->token.type >= REDIRECT_SO && q->data->token.type <= HERDOC))
		return (0);
	if (q->next->data->token.type == REDIRECT_SO || q->next->data->token.type == APPEND)
		return (0);
	close(node->file_in); 
	if (access(q->data->token.token, F_OK) == -1)
		return ft_putstr_fd(q->data->token.token, 2), check_permission(q->data->token.token);
	if (access(q->data->token.token, R_OK) == -1)
		return ft_putstr_fd(q->data->token.token, 2), check_permission(q->data->token.token);
	if (q->next->data->token.type == REDIRECT_SI)
		node->file_in = open(q->data->token.token, 0);
	return (3); // edited from vim, thus updated status changed in 42 header!!
}

int	handle_redirection(t_node *node)
{
	t_nodeq	*q;
	int		i;

	if (!node->token.redir)
		return 0;
	i = -1;
	while (++i < node->token.redir->size)
	{
		q = q_n_get(node->token.redir);
		case_so(node, q);
       	case_si(node, q);
	}
	return (0);
}