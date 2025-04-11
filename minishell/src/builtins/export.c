/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:00:00 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/11 15:11:26 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	print_export_env(t_env *env)
{
	int		len;
	int		i;
	t_env	**arr;
	t_env	*tmp;

	len = env_len(env);
	arr = malloc(sizeof(t_env *) * len);
	if (!arr)
		return (1);
	tmp = env;
	i = 0;
	while (i < len && tmp)
	{
		arr[i++] = tmp;
		tmp = tmp->next;
	}
	sort_env_array(arr, len);
	i = 0;
	while (i < len)
		print_export_entry(arr[i++]);
	free(arr);
	return (0);
}

int	process_single_export(char *arg, t_env **env_list)
{
	char	*key;
	char	*value;
	int		has_equal;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	has_equal = split_key_value(arg, &key, &value);
	if (has_equal)
	{
		env_set(env_list, key, value);
		free(key);
		free(value);
	}
	else
	{
		env_set(env_list, arg, NULL);
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
		process_single_export(argv[i++], env_list);
	return (0);
}
