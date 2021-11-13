#include <linux/bpf.h>
#include <bpf_helpers.h>

#include "common_kern_user.h" /* defines: struct datarec; */

/* Lesson: See how a map is defined.
 * - Here an array with XDP_ACTION_MAX (max_)entries are created.
 * - The idea is to keep stats per (enum) xdp_action
 */
struct bpf_map_def SEC("maps") xdp_stats_map = {
	.type = BPF_MAP_TYPE_PERCPU_ARRAY,
	.key_size = sizeof(__u32),
	.value_size = sizeof(struct datarec),
	.max_entries = XDP_ACTION_MAX,
};

/* LLVM maps __sync_fetch_and_add() as a built-in function to the BPF atomic add
 * instruction (that is BPF_STX | BPF_XADD | BPF_W for word sizes)
 */
#ifndef lock_xadd
#define lock_xadd(ptr, val) ((void)__sync_fetch_and_add(ptr, val))
#endif

static __always_inline __u32 xdp_stats_record_action(struct xdp_md *ctx, __u32 action)
{
	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
	struct datarec *rec;

	if (action >= XDP_ACTION_MAX)
	{
		return XDP_ABORTED;
	}

	rec = bpf_map_lookup_elem(&xdp_stats_map, &action);
	if (!rec)
	{
		return XDP_ABORTED;
	}

	__u64 bytes = data_end - data;
	rec->rx_packets++;
	rec->rx_bytes += bytes;
	return action;
}

SEC("xdp_drop")
int xdp_drop_f(struct xdp_md *ctx)
{
	__u32 action = XDP_DROP;
	return xdp_stats_record_action(ctx, action);
}

SEC("xdp_pass")
int xdp_pass_f(struct xdp_md *ctx)
{
	__u32 action = XDP_PASS;
	return xdp_stats_record_action(ctx, action);
}

SEC("xdp_abort")
int xdp_abort_f(struct xdp_md *ctx)
{
	__u32 action = XDP_ABORTED;
	return xdp_stats_record_action(ctx, action);
}

char _license[] SEC("license") = "GPL";
