/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 01:44:51 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/22 01:45:02 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_logic_op(t_token_type t)
{
	return (t == PIPE || t == AND_AND || t == OR_OR || t == AMPERSAND);
}

int	is_redir(t_token_type t)
{
	return (t == REDIR_IN || t == REDIR_OUT || t == APPEND || t == HEREDOC);
}

int	is_operator(t_token_type t)
{
	return (is_logic_op(t) || is_redir(t));
}
