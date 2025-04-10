/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:34:44 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 11:59:38 by gpico-co         ###   ########.fr       */
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

int	process_single_export(char *arg, t_env **env_list)
{
	char	*key;
	char	*value;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	if (split_key_value(arg, &key, &value))
	{
		env_set(env_list, key, value);
		free(key);
		free(value);
	}
	return (0);
}

int	builtin_export(char **argv, t_env **env_list)
{
	int	i;

	if (!argv[1])
		return (print_export_env(*env_list));
	i = 1;
	while (argv[i])
	{
		process_single_export(argv[i], env_list);
		i++;
	}
	return (0);
}
