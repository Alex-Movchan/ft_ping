#include "ft_ping.h"

env_t	*g_env;

static struct addrinfo	*ft_get_host_addrinfo(char *host_name)
{
	struct addrinfo		hints;
	struct addrinfo		*res;

	ft_bzero((void*)&hints, sizeof(hints));
	hints.ai_family = g_env->flag & FLAG_IPV6 ? AF_INET6 : AF_UNSPEC;
	hints.ai_socktype = SOCK_UNKNOW;
	hints.ai_flags = AI_CANONNAME;
	if (getaddrinfo(host_name, NULL, &hints, &res) != 0)
		ft_error("Error: unknow address/hostname.");
	return (res);
}

static void				ft_check_ip_version(struct addrinfo	*host_addrinfo)
{
	if (host_addrinfo->ai_family == AF_INET)
	{
		if (g_env->flag & FLAG_IPV6)
			ft_error("Error: cannot ping IPv4-mapped IPv6 address. Try without [-6]");
	}
	else if (host_addrinfo->ai_family == AF_INET6 )
	{
		if (!(g_env->flag & FLAG_IPV6))
			ft_error("Do you want to ping ip address version 6? "
							 "Then [-6]. If not, check your local firewall rules.");
		if (IN6_IS_ADDR_V4MAPPED (&(((struct sockaddr_in6 *)
				host_addrinfo->ai_addr)->sin6_addr)))
			ft_error("Error: cannot ping IPv4-mapped IPv6 address.");
	}
	else

	{
		ft_printf("Error: unknow address fameli = %d.\n", host_addrinfo->ai_family);
		exit(EXIT_FAILURE);
	}
}

static void				ft_init_environment(int ac, char **av)
{
	struct addrinfo	*host_addrinfo;

	if (!(g_env = (env_t*)malloc(sizeof(env_t))))
		ft_error("Error:memory allocate");
	g_env->interval = 1;
	if (!(g_env->host_name = ft_validarg(ac, av, &g_env->flag)))
		ft_usage();
	host_addrinfo = ft_get_host_addrinfo(g_env->host_name);
	ft_check_ip_version(host_addrinfo);
	if (!(g_env->recv_name_buff = ft_strnew(host_addrinfo->ai_addrlen)))
		ft_error("Error:memory allocate");
	g_env->sock = ft_create_socket(host_addrinfo->ai_family);
	g_env->pid = getpid() & 0xFFFF;
	g_env->addr = host_addrinfo->ai_addr;
	g_env->salen = host_addrinfo->ai_addrlen;
	g_env->count_send = 0;
	g_env->count_revc = 0;
	//g_env->send_buf =  ft_strnew(DUFF_SIZE) : ft_strnew(DUFF_SIZE);//add size buf?
}

int						main(int ac, char **av)
{
	if (getuid() != 0)
		ft_error("Error: permission denied");
	ft_init_environment(ac, av);
	signal(SIGALRM, ft_hendling_alrm);
	signal(SIGINT, ft_hendling_int);
	ft_printf("PING %s (%s): %d data bytes\n", g_env->host_name, ft_hostaddrtostr(g_env->addr, g_env->salen), DATA_LEN);
	ft_loop_recvfrom(0);
	return (0);
}
