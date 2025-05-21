/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:43:19 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/05/19 17:43:28 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_syntax_error(int err_code)
{
	if (err_code == -4)
		ft_putendl_fd("minishell: syntax error near unexpected token", 2);
	else if (err_code == SINGLE_QUOTE_UNCLOSED)
		ft_putendl_fd("Syntax error: single quote not closed", 2);
	else if (err_code == DOUBLE_QUOTE_UNCLOSED)
		ft_putendl_fd("Syntax error: double quote not closed", 2);
	else
		ft_putendl_fd("minishell: syntax error", 2);
}

void	print_quote_error(t_quote_error err)
{
	if (err == SINGLE_QUOTE_UNCLOSED)
		ft_putendl_fd(ERR_SINGLE_QUOTE, 2);
	else if (err == DOUBLE_QUOTE_UNCLOSED)
		ft_putendl_fd(ERR_DOUBLE_QUOTE, 2);
	else if (err == INVALID_OPERATOR)
		ft_putendl_fd(ERR_INV_OPERATOR, 2);
	else if (err == NON_SUPPORTED_TOKEN)
		ft_putendl_fd(ERR_NON_SUPPORTED_TOKEN, 2);
	else
		ft_putendl_fd(ERR_UNKNOWN_QUOTE, 2);
}
