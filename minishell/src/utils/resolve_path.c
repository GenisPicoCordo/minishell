#include "../../includes/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char	*find_command_path(char *cmd)
{
	char	*path_var;
	char	**paths;
	char	*full_path;
	int		i;
	char	*tmp;

	path_var = getenv("PATH");
	i = 0;
	if (!path_var || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}