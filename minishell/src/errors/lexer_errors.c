/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:43:19 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/05/07 15:00:20 by gpico-co         ###   ########.fr       */
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
	else if (err == NON_SUPPORTED_TOKEN)
		ft_putendl_fd(ERR_NON_SUPPORTED_TOKEN, 2);
	else
		ft_putendl_fd(ERR_UNKNOWN_QUOTE, 2);
}
