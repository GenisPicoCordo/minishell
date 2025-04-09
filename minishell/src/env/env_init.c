/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:42:19 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/09 13:10:47 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:42:25 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/09 12:53:11 by gpico-co         ###   ########.fr       */
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

t_env	*env_init(char **envp)
{
	t_env	*head;
	t_env	*tmp;
	t_env	*new;

	head = NULL;
	while (*envp)
	{
		new = create_env_node(*envp);
		if (!new)
		{
			envp++;
			continue ;
		}
		if (!head)
			head = new;
		else
			tmp->next = new;
		tmp = new;
		envp++;
	}
	return (head);
}
