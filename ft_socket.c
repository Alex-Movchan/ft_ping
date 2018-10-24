#include "ft_ping.h"

void	ft_initsocket_ipv6(void)
{
	struct icmp6_filter	filter;
	int					on;

	on = 0;
	if (g_env->flag & FLAG_V)
	{
		ICMP6_FILTER_SETBLOCKALL (&filter);
		ICMP6_FILTER_SETPASS (ICMP6_ECHO_REPLY, &filter);
		setsockopt (g_env->sock, IPPROTO_IPV6, ICMP6_FILTER, &filter,
					sizeof (filter));
	}
#ifdef IPV6_RECVHOPLIMIT
	setsockopt(g_env->sock, IPPROTO_IPV6, IPV6_RECVHOPLIMIT, &on, sizeof(on));
#else
	setsockopt (g_env->sock, IPPROTO_IPV6, IPV6_HOPLIMIT, &on, sizeof(on));
#endif
}

int				ft_create_socket(int family)
{
	int		sock;
	int		size;

	sock = -1;
	if (g_env->flag & FLAG_IPV6 && (sock = socket(family, SOCK_RAW, IPPROTO_ICMPV6)) < 0)
		ft_error("Error: create socket.");
	else if (sock == -1 && (sock = socket(family, SOCK_RAW, IPPROTO_ICMP)) < 0)
		ft_error("Error: create socket.");
	size = SOCKET_SIZE;
	setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &size, sizeof (size));
	g_env->flag & FLAG_B ? setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &size, sizeof (size)) : 0;
	g_env->flag & FLAG_IPV6 ? ft_initsocket_ipv6() : 0;
	return (sock);
}
