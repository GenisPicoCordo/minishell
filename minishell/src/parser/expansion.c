/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:18:08 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/10 12:30:18 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_variable(const char *str, int *i, t_shell *shell)
{
	char	var[256];
	int		j;
	char	*value;

	j = 0;
	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->last_status));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		var[j++] = str[(*i)++];
	var[j] = '\0';
	value = getenv(var);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

char	*expand_home(const char *input)
{
	char	*home;
	char	*rest;
	char	*tmp;

	home = getenv("HOME");
	if (!home)
		return (NULL);
	rest = ft_strdup(&input[1]);
	tmp = ft_strjoin(home, rest);
	free(rest);
	return (tmp);
}

char	*expand_loop(const char *input, t_shell *shell, char *result)
{
	int		i;
	int		start;
	char	*tmp;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$')
		{
			tmp = expand_variable(input, &i, shell);
			result = ft_strjoin_free(result, tmp);
		}
		else
		{
			start = i;
			while (input[i] && input[i] != '$')
				i++;
			tmp = strndup(&input[start], i - start);
			result = ft_strjoin_free(result, tmp);
		}
	}
	return (result);
}

char	*expand_string(const char *input, t_shell *shell)
{
	char	*result;

	if (input[0] == '~' && (input[1] == '/' || input[1] == '\0'))
	{
		result = expand_home(input);
		if (result)
			return (result);
	}
	result = ft_strdup("");
	if (!result)
		return (NULL);
	return (expand_loop(input, shell, result));
}

