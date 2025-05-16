/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:42:19 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/14 16:37:36 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*create_env_node(char *entry)
{
	t_env	*node;
	char	*equal_sign;
	int		key_len;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	equal_sign = ft_strchr(entry, '=');
	if (!equal_sign)
		return (free(node), NULL);
	key_len = equal_sign - entry;
	node->key = ft_substr(entry, 0, key_len);
	node->value = ft_strdup(equal_sign + 1);
	node->next = NULL;
	return (node);
}

static void	append_env_node(t_env **head, t_env **tmp, t_env *new)
{
	if (!*head)
		*head = new;
	else
		(*tmp)->next = new;
	*tmp = new;
}

static int	load_env(char **envp, t_env **head)
{
	t_env	*tmp;
	t_env	*new;
	int		is_empty;

	is_empty = 1;
	tmp = NULL;
	while (*envp)
	{
		is_empty = 0;
		new = create_env_node(*envp);
		if (new)
			append_env_node(head, &tmp, new);
		envp++;
	}
	return (is_empty);
}

static void	update_shlvl(t_env **env_list)
{
	char	*value;
	int		shlvl;
	char	*new_value;

	value = env_get(*env_list, "SHLVL");
	if (!value)
		return ((void)env_set(env_list, "SHLVL", "1"));
	if (!*value || !is_numeric(value))
		shlvl = 0;
	else
		shlvl = ft_atoi(value);
	new_value = ft_itoa(shlvl + 1);
	if (new_value)
	{
		env_set(env_list, "SHLVL", new_value);
		free(new_value);
	}
}

t_env	*env_init(char **envp)
{
	t_env	*head;
	int		is_empty;

	head = NULL;
	is_empty = load_env(envp, &head);
	if (is_empty)
		env_set(&head, "SHLVL", "1");
	else
		update_shlvl(&head);
	return (head);
}
