/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:34:44 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/09 14:11:35 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	print_export_env(t_env *env)
{
	while (env)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env->key, 1);
		if (env->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putchar_fd('\n', 1);
		env = env->next;
	}
	return (0);
}

int	builtin_export(char **argv, t_env **env_list)
{
	int		i;
	char	*key;
	char	*value;

	if (!argv[1])
		return (print_export_env(*env_list));
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			i++;
			continue ;
		}
		if (split_key_value(argv[i], &key, &value))
		{
			env_set(env_list, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (0);
}
