#include "ft_ping.h"

void	ft_get_time_sub(struct timeval *out, struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) < 0)
	{
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

static void	ft_print_statistics(void)
{
	struct timeval	time_end;
	uint64_t		diff;
	double			loss;
	double			finish;

	gettimeofday(&time_end, NULL);
	diff = g_env->count_send - g_env->count_revc;
	loss = (double)diff / g_env->count_send * 100;
	ft_get_time_sub(&time_end, &g_env->time_begin);
	finish = time_end.tv_sec * 1000.0 + time_end.tv_usec / 1000.0;
	if (g_env->flag & FLAG_B)
	{
		ft_printf("\n--- %s ping statistics ---\n%ld packets transmitted, %ld recieved, +%ld duplicates, "
				  "%.0f%% packet loss, time %.fms\n",
				  g_env->host_name, g_env->count_send, g_env->count_revc, g_env->count_packets_dup,
				  loss, finish);
		g_env->send_size > HEADER_LEN_IPV4_ICMP ? ft_printf("rtt min/max = %.3f/%.3f\n", g_env->min, g_env->max) : ft_printf("\n");
	}
	else
	{
		ft_printf("\n--- %s ping statistics ---\n%ld packets transmitted, %ld recieved, "
				  "%.0f%% packet loss, time %.fms\n",
				  g_env->host_name, g_env->count_send, g_env->count_revc,
				  loss, finish);;
		g_env->send_size > HEADER_LEN_IPV4_ICMP ? ft_printf("rtt min/max = %.3f/%.3f\n", g_env->min, g_env->max) : ft_printf("\n");

	}
}

void	ft_hendling_int(int sig)
{
	if (sig != SIGINT)
		return ;
	ft_print_statistics();
	close(g_env->sock);
	ft_strdel(&g_env->recv_name_buff);
	exit(EXIT_SUCCESS);
}

void	ft_hendling_alrm(int sig)
{
	if (sig != SIGALRM)
		return ;
	if (g_env->flag & FLAG_C && g_env->count_send + 1 == g_env->count_packets && --g_env->count_send)
		ft_hendling_int(SIGINT);
	g_env->flag & FLAG_IPV6 ? ft_sendto_ipv6() :ft_sendto_ipv4();
	alarm(g_env->interval);
}
