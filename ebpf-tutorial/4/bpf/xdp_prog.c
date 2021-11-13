#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if_vlan.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <stdbool.h>
#include <linux/bpf.h>

#include "bpf_endian.h"
#include "common.h"
#include "bpf_helpers.h"
#include "parsing_helper.h"

/* to u64 in host order */
static inline __u64 ether_addr_to_u64(const __u8 *addr)
{
	__u64 u = 0;
	int i;

	for (i = ETH_ALEN - 1; i >= 0; i--)
		u = u << 8 | addr[i];
	return u;
}

SEC("xdp")
int xdp_prog_simple(struct xdp_md *ctx)
{
	void *data = (void *)(long)ctx->data;
	void *data_end = (void *)(long)ctx->data_end;

	bpf_printk("get xdp_md message data %llu\n", data);
	bpf_printk("get xdp_md message data_end %llu\n", data_end);

	struct hdr_cursor nh;
	nh.pos = data;

	struct ethhdr *eth_hdr;
	struct iphdr *ip_hdr;
	struct tcphdr *tcp_hdr;
	struct udphdr *udp_hdr;

	int eth_hdr_proto;
	int ip_hdr_proto;
	long tcp_port;

	eth_hdr_proto = parse_ethhdr(&nh, data_end, &eth_hdr);
	bpf_printk("after parse eth header cursor %llu\n", (long)nh.pos);

	if (eth_hdr_proto == bpf_htons(ETH_P_IP))
	{
		ip_hdr_proto = parse_iphdr(&nh, data_end, &ip_hdr);
		bpf_printk("after parse ip header cursor %llu\n", (long)nh.pos);

		if (ip_hdr_proto == IPPROTO_UDP)
		{
			int udp_port = parse_udphdr(&nh, data_end, &udp_hdr);
			bpf_printk("after parse udp header cursor %llu\n", (long)nh.pos);
			if (udp_port > 0)
			{
				// l4_csum_replace(skb, nh.pos + offsetof(struct udp_hdr, check), 0, sum, BPF_F_PSEUDO_HDR);
				bpf_printk("got tcp message dest port %u\n", bpf_ntohs(udp_hdr->dest));

				unsigned char *udp_value1 = (unsigned char *)nh.pos;
				nh.pos += sizeof(udp_value1);
				bpf_printk("after parse udp value cursor %llu\n", (long)nh.pos);
				bpf_printk("udp value %s\n", udp_value1);
			}
		}
	}

	return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
