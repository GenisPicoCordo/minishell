/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:58:24 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/29 18:56:13 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	split_key_value(char *arg, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (!equal)
	{
		*key = ft_strdup(arg);
		*value = NULL;
		return (1);
	}
	*key = ft_substr(arg, 0, equal - arg);
	*value = ft_strdup(equal + 1);
	return (1);
}

void	update_value(t_env *env, const char *value)
{
	if (!value)
		return ;
	free(env->value);
	env->value = ft_strdup(value);
}

t_env	*new_env_node(const char *key, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

char	*env_get(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
