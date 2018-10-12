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
	ft_printf("\n--- %s ping statistics ---\n%ld packets transmitted, %ld recieved, "
				   "%.0f%% packet loss, time %.fms\nrtt min/max = %.3f/%.3f\n",
		g_env->host_name, g_env->count_send, g_env->count_revc,
		loss, finish, g_env->min, g_env->max);
}

void	ft_hendling_int(int sig)
{
	if (sig != SIGINT)
		return ;
	ft_print_statistics();
	close(g_env->sock);
	exit(EXIT_SUCCESS);
}

void	ft_hendling_alrm(int sig)
{
	if (sig != SIGALRM)
		return ;
	g_env->flag & FLAG_IPV6 ? ft_sendto_ipv6() :ft_sendto_ipv4();
	alarm(g_env->interval);
}