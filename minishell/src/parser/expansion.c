/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:18:08 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/09 14:18:30 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char    *expand_variable(const char *str, int *i, __attribute__((unused)) char **env, int last_status)
{
	char var[256];
	int j = 0;
	char *value;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(last_status));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		var[j++] = str[(*i)++];
	var[j] = '\0';
	value = getenv(var);
	return value ? ft_strdup(value) : strdup("");
}

char *expand_string(const char *input, char **env, int last_status)
{
	char *result = strdup("");
	int i = 0;

	while (input[i])
	{
		if (input[i] == '$')
		{
			char *exp = expand_variable(input, &i, env, last_status);
			char *tmp = ft_strjoin(result, exp);
			free(exp);
			free(result);
			result = tmp;
		}
		else
		{
			int start = i;
			while (input[i] && input[i] != '$')
				i++;
			char *fragment = strndup(&input[start], i - start);
			char *tmp = ft_strjoin(result, fragment);
			free(fragment);
			free(result);
			result = tmp;
		}
	}
	return result;
}