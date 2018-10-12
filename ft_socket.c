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
	g_env->flag & FLAG_IPV6 ? ft_initsocket_ipv6() : 0;
	return (sock);
}

char				*ft_hostaddrtostr(struct sockaddr *sa, socklen_t len)
{
	struct sockaddr_in6	*sin6;
	struct sockaddr_in	*sin;
	static char			str[HOST_ADDR_SIZE];

	ft_bzero(str, HOST_ADDR_SIZE);
	if (sa->sa_family == AF_INET)
	{
		sin = (struct sockaddr_in*)sa;
		if (!inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)))
			ft_error("Error: inet_ntop failed.");
	}
	else if (sa->sa_family == AF_INET6)
	{
		sin6 = (struct sockaddr_in6 *) sa;
		if (!inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)))
			ft_error("Error: inet_ntop failed.");
	}
	else
		ft_error("Error: socaddr family unknow.");
	return (str);
}