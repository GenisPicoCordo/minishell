#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strncmp(cmd, "echo", 5));
}

int	execute_builtin(char **argv)
{
	if (!argv || !argv[0])
		return (1);
	if (!ft_strncmp(argv[0], "echo", 5))
		return (builtin_echo(argv));
	return (1);
}

int	builtin_echo(char **argv)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (argv[i] && !ft_strncmp(argv[i], "-n", 3))
	{
		n_flag = 1;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}
