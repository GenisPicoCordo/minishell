/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:43:19 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/09 13:16:17 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_quote_error(t_quote_error err)
{
	if (err == SINGLE_QUOTE_UNCLOSED)
		ft_putendl_fd(ERR_SINGLE_QUOTE, 2);
	else if (err == DOUBLE_QUOTE_UNCLOSED)
		ft_putendl_fd(ERR_DOUBLE_QUOTE, 2);
	else if (err == INVALID_OPERATOR)
		ft_putendl_fd(ERR_INV_OPERATOR, 2);
	else
		ft_putendl_fd(ERR_UNKNOWN_QUOTE, 2);
}
