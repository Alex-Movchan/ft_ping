#include "ft_ping.h"

static int	ft_get_flag(char *str)
{
	if (!ft_strcmp(str, "-h"))
		ft_usage();
	else if (!ft_strcmp(str, "-v"))
		return (FLAG_V);
	else if (!ft_strcmp(str, "-6"))
		return (FLAG_IPV6);
	else if (!ft_strcmp(str, "-c"))
		return (FLAG_C);
	else if (!ft_strcmp(str, "-n"))
		return (FLAG_N);
	else if (!ft_strcmp(str, "-i"))
		return (FLAG_I);
	else if (!ft_strcmp(str, "-b"))
		return (FLAG_B);
	else
		ft_usage();
}
static void ft_check_broadcast_address(char *adrr, long flag)
{
	if (!(flag & FLAG_B))
	{
		if (!ft_strncmprev(adrr, BROADCAST_ADDR, ft_strlen(BROADCAST_ADDR)))
			ft_error("Do you want to ping broadcast? "
				 "Then -b. If not, check your local firewall rules.");
		else
			ft_putendl("Not broadcast.");
	}
}

char		*ft_validarg(int cout, char **av, long *flag)
{
	int		i;
	int		arg;
	char	*host_name;

	if (cout < 2)
		ft_usage();
	i = 0;
	host_name = NULL;
	while (++i < cout)
	{
		if (av[i][0] == '-')
		{
			(*flag) |= (arg = ft_get_flag(av[i]));
			if (FLAG_C == arg)
			{
				if (++i > cout || !ft_isdigit(av[i][0]))
					ft_error("Error: bad number of packets to transmit.");
				g_env->count_packets = ft_atoi(av[i]) + 1;
				continue ;
			}
			if (FLAG_I == arg)
			{
				if (++i > cout || !ft_isdigit(av[i][0]))
					ft_error("Error: bad number of interval to transmit.");
				g_env->interval = (unsigned int)ft_atoi(av[i]);
				continue ;
			}
		}
		else
			host_name = av[i];
	}
	ft_check_broadcast_address(host_name, (*flag));
	return (host_name);
}