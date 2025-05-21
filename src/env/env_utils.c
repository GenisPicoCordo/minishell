/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:58:24 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/21 10:38:46 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*clean_value_quotes(char *raw_value)
{
	size_t	len;

	if (!raw_value)
		return (NULL);
	len = ft_strlen(raw_value);
	if (len >= 2 && ((raw_value[0] == '"' && raw_value[len - 1] == '"') \
		|| (raw_value[0] == '\'' && raw_value[len - 1] == '\'')))
	{
		return (ft_substr(raw_value, 1, len - 2));
	}
	return (ft_strdup(raw_value));
}

int	split_key_value(char *arg, char **key, char **value)
{
	char	*equal;
	char	*raw_value;

	equal = ft_strchr(arg, '=');
	if (!equal)
	{
		*key = ft_strdup(arg);
		*value = NULL;
		return (0);
	}
	*key = ft_substr(arg, 0, equal - arg);
	raw_value = ft_strdup(equal + 1);
	if (!raw_value)
		return (0);
	*value = clean_value_quotes(raw_value);
	free(raw_value);
	return (*value != NULL);
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
