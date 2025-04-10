/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:58:24 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/09 14:05:24 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	split_key_value(char *arg, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (!equal)
		return (0);
	*key = ft_substr(arg, 0, equal - arg);
	*value = ft_strdup(equal + 1);
	return (1);
}

char	*env_get(t_env *env, const char *key)
{
	while (env)
	{
		if (!ft_strncmp(env->key, key, ft_strlen(key) + 1))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	env_set(t_env **env, const char *key, const char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, key, ft_strlen(key) + 1))
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return ;
	tmp->key = ft_strdup(key);
	tmp->value = ft_strdup(value);
	tmp->next = *env;
	*env = tmp;
}

void	env_unset(t_env **env, const char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, key, ft_strlen(key) + 1))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}
