/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 11:15:41 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/29 18:56:08 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_unset(char **argv, t_env **env_list)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		env_unset(env_list, argv[i]);
		i++;
	}
	return (0);
}
