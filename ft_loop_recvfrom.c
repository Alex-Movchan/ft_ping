#include "ft_ping.h"

static void	ft_show_recvmsg_ipv6(char *buff, struct timeval *time, ssize_t size)
{
	struct icmp6_hdr	*frame;
	struct timeval		*time_send;
	double				rtt;

	frame = (struct icmp6_hdr*)buff;
	if (size < 8)
		return ;
	if (frame->icmp6_type == ICMP6_ECHO_REPLY)
	{
		if (frame->icmp6_id != g_env->pid)
			return ;
		g_env->count_revc++;
		if (size >= 16 && g_env->send_size > HEADER_LEN_IPV4_ICMP)
		{
			time_send = (struct timeval *) (frame + 1);
			ft_get_time_sub(time, time_send);
			rtt = time->tv_sec * 1000.0 + time->tv_usec / 1000.0;
			if (rtt < g_env->min || g_env->min == 0.0000)
				g_env->min = rtt;
			if (rtt > g_env->max)
				g_env->max = rtt;
			g_env->sum += rtt;
		}
		ft_printf("%d bytes from %s (%s): icmp_seq=%d  ",
			size, g_env->host_name, g_env->host_addr, frame->icmp6_seq);
		g_env->send_size > HEADER_LEN_IPV4_ICMP ? ft_printf("time=%.3f ms\n", rtt) : ft_printf("\n");
	}
	else if (g_env->flag & FLAG_V)
		ft_printf(" %d bytes from %s (%s): type = %d, code = %d\n",
				  size, g_env->host_name, g_env->host_addr, frame->icmp6_type, frame->icmp6_code);

}

static void	ft_show_recvmsg_ipv4(char *buff, struct msghdr *msg, struct timeval *time, ssize_t size)
{
	struct icmp		*frame;
	struct ip		*addr;
	struct timeval	*time_send;
	ssize_t			icmp_len;
	double			rtt;
	if (!msg)
		ft_error("Error: message is NULL.");
	addr = (struct ip*)buff;
	if (addr->ip_p != IPPROTO_ICMP)
		return ;
	frame = (struct icmp*)(buff + IP_HEADER_LEN(addr->ip_hl));
	if ((icmp_len = size - IP_HEADER_LEN(addr->ip_hl)) < 8)
		return ;
	if (frame->icmp_type == ICMP_ECHOREPLY)
	{
		if (frame->icmp_id != g_env->pid)
			return;
		if ( icmp_len >= 16 && g_env->send_size > HEADER_LEN_IPV4_ICMP)
		{
			time_send = (struct timeval*)frame->icmp_data;
			ft_get_time_sub(time, time_send);
			rtt = time->tv_sec * 1000.0 + time->tv_usec / 1000.0;
			if (rtt < g_env->min || g_env->min == 0.0000)
				g_env->min = rtt;
			if (rtt > g_env->max)
				g_env->max = rtt;
			g_env->sum += rtt;
		}
		frame->icmp_seq <= g_env->seq ? g_env->count_packets_dup++ : g_env->count_revc++;

		if (g_env->flag & FLAG_T && addr->ip_ttl > g_env->max_ttl)
		{
			if (g_env->flag & FLAG_B && frame->icmp_seq <= g_env->seq)
			{
				ft_printf("%d bytes from %s (%s): icmp_seq=%llu Time to live exceeded (DUP!)\n",
						  size - HEADER_LEN_ICMP, g_env->host_name, g_env->host_addr,
						  frame->icmp_seq);
			}
			else
				ft_printf("%d bytes from %s (%s): icmp_seq=%llu Time to live exceeded\n",
						  size - HEADER_LEN_ICMP, g_env->host_name, g_env->host_addr,
						  frame->icmp_seq);
		}
		else
		{
			if (g_env->flag & FLAG_B && frame->icmp_seq <= g_env->seq)
			{
				ft_printf("%d bytes from %s (%s): icmp_seq=%llu ttl=%d ",
						  size - HEADER_LEN_ICMP, g_env->host_name, g_env->host_addr,
						  frame->icmp_seq, addr->ip_ttl);
				g_env->send_size > HEADER_LEN_IPV4_ICMP ? ft_printf("time=%.3f ms(DUP!)\n", rtt) : ft_printf("\n");

			}
			else
			{
				ft_printf("%d bytes from %s (%s): icmp_seq=%llu ttl=%d ",
						  size - HEADER_LEN_ICMP, g_env->host_name, g_env->host_addr,
						  frame->icmp_seq, addr->ip_ttl);
				g_env->send_size > HEADER_LEN_IPV4_ICMP ? ft_printf("time=%.3f ms\n", rtt) : ft_printf("\n");
			}
		}
		if (g_env->flag & FLAG_B)
			g_env->seq =  g_env->seq > frame->icmp_seq ? g_env->seq : frame->icmp_seq;
	}
	else if (g_env->flag & FLAG_V)
		ft_printf("%d bytes from %s (%s): type = %d, code = %d\n",
				  size, g_env->host_name, g_env->host_addr, frame->icmp_type, frame->icmp_code);
}

void	ft_loop_recvfrom(ssize_t size)
{
	struct msghdr	msg;
	struct iovec	iov;
	struct timeval	time_now;
	char			buff[SIZE_BUFF];
	char			controlbuf[SIZE_BUFF];

	iov.iov_base = buff;
	iov.iov_len = SIZE_BUFF;
	msg.msg_name = g_env->recv_name_buff;
	msg.msg_control = controlbuf;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	gettimeofday(&g_env->time_begin, NULL);
	ft_hendling_alrm(SIGALRM);
	while (true)
	{
		msg.msg_namelen = g_env->salen;
		msg.msg_controllen = sizeof (controlbuf);
		if ((size = recvmsg(g_env->sock, &msg, 0)) < 0)
			ft_error("Error: recving sock_ntop_host.");
		if (gettimeofday(&time_now, NULL) == -1)
			ft_error("Error: geting time of day.");
		if (g_env->flag & FLAG_IPV6)
			ft_show_recvmsg_ipv6(buff, &time_now, size);
		else
			ft_show_recvmsg_ipv4(buff, &msg, &time_now, size);
	}
}
