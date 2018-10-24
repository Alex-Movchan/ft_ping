#include "ft_ping.h"

static uint16_t	ft_get_checksum(uint16_t *data, size_t size)
{
	uint16_t	*carer;
	uint32_t	sum;
	size_t		cout;
	uint16_t	first;

	cout = size;
	carer = data;
	sum = 0;
	while (cout > 1)
	{
		sum += *carer++;
		cout -= 2;
	}
	if (cout == 1)
	{
		*(unsigned char*)(&first) = *(unsigned char*)carer;
		sum += first;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return (uint16_t)~sum;
}

void			ft_sendto_ipv4(void)
{
	static char	buff[SIZE_BUFF];
	struct icmp	*frame;
	size_t		data_len;

	data_len = DATA_LEN + 8;
	frame = (struct icmp*)buff;
	frame->icmp_type = ICMP_ECHO;
	frame->icmp_code = 0;
	frame->icmp_id  =  g_env->pid;
	frame->icmp_seq = ++g_env->count_send;
	if (g_env->flag & FLAG_C && g_env->count_send == g_env->count_packets && --g_env->count_send)
		ft_hendling_int(SIGINT);
	if (gettimeofday((struct timeval *)frame->icmp_data, NULL) == -1)
		ft_error("Error: geting time of day.");
	frame->icmp_cksum = 0;
	frame->icmp_cksum = ft_get_checksum((uint16_t*)frame, data_len);
	if (sendto(g_env->sock, buff, data_len, 0,
			g_env->addr, g_env->salen) < 0)
		ft_error("Error: sendto.");
}

void			ft_sendto_ipv6(void)
{
	struct icmp6_hdr	*frame;
	size_t				data_len;
	static char			buff[SIZE_BUFF];

	frame = (struct icmp6_hdr *) buff;
	frame->icmp6_type = ICMP6_ECHO_REQUEST;
	frame->icmp6_code = 0;
	frame->icmp6_id = g_env->pid;
	frame->icmp6_seq = ++g_env->count_send;
	if (g_env->flag & FLAG_C && g_env->count_send == g_env->count_packets && --g_env->count_send)
		ft_hendling_int(SIGINT);
	ft_memset((frame + 1), 0xa5, DATA_LEN);
	if (gettimeofday((struct timeval *)(frame + 1), NULL) == -1)
		ft_error("Error: geting time of day.");
	data_len = 8 + DATA_LEN;
	if (sendto(g_env->sock, buff, data_len, 0,
			   g_env->addr, g_env->salen) == STATUS_ERROR)
		ft_error("Error: sendto.");
}
