#include "../../includes/minishell.h"

int	cmd_exists(char *cmd)
{
	char	*path;

	if (!cmd)
		return (0);
	if (access(cmd, X_OK) == 0)
		return (1);
	path = find_command_path(cmd);
	if (path)
	{
		free(path);
		return (1);
	}
	return (0);
}

int	validate_cmd_table(t_cmd_table *table, char **env)
{
	int	i;

	(void)env;
	i = 0;
	while (i < table->count)
	{
		if (!table->cmds[i].cmd)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|`", 2);
			return (0);
		}
		if (!cmd_exists(table->cmds[i].cmd))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(table->cmds[i].cmd, 2);
			ft_putendl_fd(": command not found", 2);
			return (0);
		}
		i++;
	}
	return (1);
}
