/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_sub_shell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-mouh <tel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 16:09:31 by tel-mouh          #+#    #+#             */
/*   Updated: 2022/10/09 22:16:24 by tel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_sub(t_node *node, t_vars *vars)
{
	int pid;

	if (!is_sub(node))
		return 0;
	node->node_type = OP;
	pid = fork();
	if (!pid)
	{
		if (!node)
			return 0;
		dup2(node->file_in , 0);
		dup2(node->file_out, 1);
		vars->pid_num = 0;
		exucute(node, vars);
		while (vars->pid_num > 0)
		{
			wait(0);
			vars->pid_num--;
		}
		exit(WEXITSTATUS(node->token.exit_status));
	}
	waitpid(pid, &node->token.exit_status, 0);
	close_in_parent(node);
	return 1;
}