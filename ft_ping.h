#ifndef FT_PING_H
#define FT_PING_H

# include <stdio.h>
# include <signal.h>
# include "libft/libft.h"
# include <sys/time.h>
# include <netinet/in_systm.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <errno.h>
#include <stdbool.h>
# include <netinet/ip6.h>
# include <netinet/icmp6.h>

#define FLAG_V 1
#define FLAG_B 2
#define FLAG_C 4
#define FLAG_I 8
#define FLAG_N 16
#define FLAG_IPV6 64
#define SOCK_UNKNOW 0
#define BROADCAST_ADDR ".255"
#define SIZE_BUFF 1500
#define DATA_LEN 56
#define HOST_ADDR_SIZE 128
#define STATUS_ERROR -1
#define SOCKET_SIZE 64 * 1024;
#define IP_HEADER_LEN(n) ((n) << 2)

typedef struct		s_env
{
	char			*recv_name_buff;
	char			*host_name;
	struct sockaddr	*addr;
	struct timeval	time_begin;
	uint64_t		count_send;
	uint64_t		count_revc;
	long			flag;
	double			min;
	double			max;
	socklen_t		salen;
	pid_t			pid;
	unsigned int	interval;
	int				sock;
	int				count_packets;
}					env_t;

extern env_t		*g_env;

char				*ft_hostaddrtostr(struct sockaddr *sa, socklen_t len);
void				ft_error(char *str);
void				ft_usage(void);
char				*ft_validarg(int cout, char **av, long *flag);
void				ft_hendling_alrm(int sig);
void				ft_hendling_int(int sig);
void				ft_sendto_ipv6(void);
void				ft_sendto_ipv4(void);
void				ft_loop_recvfrom(ssize_t size);
int					ft_create_socket(int family);
void				ft_get_time_sub(struct timeval *out, struct timeval *in);

#endif
