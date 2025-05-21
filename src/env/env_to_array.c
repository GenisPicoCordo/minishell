/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:27:47 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/08 15:17:48 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env_list_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static char	*build_env_entry(t_env *env)
{
	char	*entry;

	if (env->value)
	{
		entry = ft_strjoin(env->key, "=");
		if (!entry)
			return (NULL);
		return (ft_strjoin_free(entry, env->value));
	}
	return (ft_strdup(env->key));
}

char	**env_to_array(t_env *env)
{
	int		size;
	int		i;
	char	**array;
	char	*entry;

	size = env_list_size(env);
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (env)
	{
		entry = build_env_entry(env);
		if (!entry)
		{
			free_split(array);
			return (NULL);
		}
		array[i++] = entry;
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}
