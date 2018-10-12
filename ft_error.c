#include "ft_ping.h"

void	ft_error(char *str)
{
	if (str)
		ft_putendl_fd(str, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	ft_usage(void)
{
	ft_putendl_fd("Usage: ping [ [ -v ] [ -h ] [-b] <hostname>", STDOUT_FILENO);
	exit(EXIT_SUCCESS);
}