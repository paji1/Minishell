/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-mouh <tel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 21:17:18 by tel-mouh          #+#    #+#             */
/*   Updated: 2022/10/09 22:15:59 by tel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_expipe(t_node *node)
{
	int fd[2];

	if (node->token.type != PIP)
		return 0;
	if (pipe(fd) == -1)
		return -1;
	node->right->file_in = fd[0];
	node->right->file_out = node->file_out;
	node->left->file_in = fd[0];
	node->left->file_out = fd[1];
	return 0;
}

int handle_exblock(t_node *node, char **env)
{
	int pid;
	if (is_sub(node) || node->node_type != BLOCK)
		return 0;
	if (fork_cmd(node, env)  < 0)
		return 0;
	close_in_parent(node);
	return 1;
}

int handle_exop(t_node *node, char **env)
{
	if (!is_sub(node) && node->node_type != OP)
		return 0;
	handle_expipe(node);
	handle_bin(node);
}

static void  wait_for_right_cmd(t_node *node)
{
	if (node->right->node_type == BLOCK)
		waitpid(node->right->token.pid_child, &node->token.exit_status, 0);
	else
		node->token.exit_status = node->right->token.exit_status;
}

void right_status(t_node *node)
{
	
	if (!node->right)
		return ;
	if (node->token.type == PIP && node->right->file_out == 1)
		wait_for_right_cmd(node);
	if (node->token.type == AND && node->token.exit_status == 0)
		wait_for_right_cmd(node);
	if (node->token.type == OR)
	{
		wait_for_right_cmd(node);
		if (node->left->token.exit_status == 0)
			node->token.exit_status == 0;
	}
}

void exucute(t_node *root, t_vars *vars)
{
	if (root == NULL)
		return ;
	if (handle_exblock(root, vars->env))
		vars->pid_num++;
	if (handle_sub(root, vars))
		return ;
	handle_exop(root, vars->env);
	exucute(root->left, vars);
	if (bin_status(root, vars))
		exucute(root->right, vars);
	right_status(root);
}