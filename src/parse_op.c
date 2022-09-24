/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-mouh <tel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 00:10:39 by tel-mouh          #+#    #+#             */
/*   Updated: 2022/09/25 00:14:04 by tel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int handle_OP(t_vars *vars, t_node *node)
{
	if(node->token.type >= REDIRECT_SO && node->token.type <= HERDOC)
		return put_redir(&vars->root, node), 1;
	if (vars->root->node_type == BLOCK && node->node_type == OP)
		return node->left = vars->root,vars->root = node , 1;
	if (node->token.type == OR || node->token.type == AND)
		return above_root(&vars->root, node), 1;
	if (node->token.type <= vars->root->token.type)
		return above_root(&vars->root, node), 1;
	else
		return nested(&vars->root, node), 1;
	return 0;
}

void	above_root(t_node **root, t_node *new)
{
	new->left = (*root);
	new->right = NULL;
	*root = new;
}

void	nested(t_node **root, t_node *new)
{

	if ((*root)->node_type == OP && (*root)->token.type > new->token.type)
	{
		new->left = (*root);
		new->right = NULL;
		(*root) = new;
		return ;
	}
	nested(&(*root)->right, new);
}

void	put_redir(t_node **root, t_node *new)
{
	if (*root == NULL)
	{	
		*root = new_tnode();
		(*root)->node_type = BLOCK;
		(*root)->token.type = FILED;
		(*root)->token.token = ft_strdup("");
		(*root)->token.redir = new_queue();
		qput((*root)->token.redir, new_node(new));
		handle_herdoc(new);
		return ;
	}
	if ((*root)->right == NULL)
	{
		if ((*root)->token.redir == NULL)
			(*root)->token.redir = new_queue();
		qput((*root)->token.redir, new_node(new));
		handle_herdoc(new);
		return ;
	}
	put_redir(&(*root)->right, new);
}