/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:19:52 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/11 13:28:14 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ft_max(size_t a, size_t b)
{
	if (a > b)
		return (a);
	return (b);
}

int	env_len(t_env *env)
{
	int	len;

	len = 0;
	while (env)
	{
		len++;
		env = env->next;
	}
	return (len);
}

void	sort_env_array(t_env **arr, int len)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = -1;
	while (++i < len - 1)
	{
		j = i;
		while (++j < len)
		{
			if (ft_strncmp(arr[i]->key, arr[j]->key, \
			ft_max(ft_strlen(arr[i]->key), ft_strlen(arr[j]->key))) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

void	print_export_entry(t_env *var)
{
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(var->key, 1);
	if (var->value)
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(var->value, 1);
		ft_putstr_fd("\"", 1);
	}
	ft_putchar_fd('\n', 1);
}
