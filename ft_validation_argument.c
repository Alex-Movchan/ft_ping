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
	else if (!ft_strcmp(str, "-t"))
		return (FLAG_T);
	else if (!ft_strcmp(str, "-i"))
		return (FLAG_I);
	else if (!ft_strcmp(str, "-b"))
		return (FLAG_B);
	else if (!ft_strcmp(str, "-s"))
		return (FLAG_S);
	else
		ft_usage();
	return (0);
}
static void ft_check_broadcast_address(char *adrr, long flag)
{
	if (!ft_strncmprev(adrr, BROADCAST_ADDR, ft_strlen(BROADCAST_ADDR)) && !(flag & FLAG_B))
			ft_error("Do you want to ping broadcast? "
				 "Then -b. If not, check your local firewall rules.");
	if (flag & FLAG_B && ft_strncmprev(adrr, BROADCAST_ADDR, ft_strlen(BROADCAST_ADDR)))
		ft_error("Not broadcast.");
}

static int	get_velue(char *str)
{
	static int	res;

	if (!str || !ft_isdigit(str[0]))
		return (-1);
	if ((res = ft_atoi(str)) < 0)
	{
		return (-1);
	}
	return (res);
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
			if (FLAG_C == arg && (g_env->count_packets = get_velue(av[++i]) + 1) < 0)
				ft_error("Error: bad number of packets to transmit.");
			else if (FLAG_I == arg && (g_env->interval = get_velue(av[++i]) < 1))
					ft_error("Error: bad number of interval to transmit.");
			else if (FLAG_T == arg && ((0 > (g_env->max_ttl = get_velue(av[++i]))) || g_env->max_ttl > MAX_TTL))
				ft_error("Error: bad number of max ttl to transmit.");
			else if (FLAG_S == arg && ((0 > (g_env->send_size = get_velue(av[++i]))) || g_env->send_size > SIZE_BUFF))
				ft_error("Error: bad number of packet size to transmit.");
		}
		else
			host_name = av[i];
	}
	FLAG_T & (*flag) ? ft_printf("Max_ttl = %d\n", g_env->max_ttl) : 0;
	ft_check_broadcast_address(host_name, (*flag));
	return (host_name);
}
