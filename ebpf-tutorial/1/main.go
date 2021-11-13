package main

import (
	"log"

	"github.com/vishvananda/netlink"
)

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -cc clang EBPFTutorial /home/cooker/ForChange/git.forchange.cn/zhanglixin/lkm-learn/ebpf-tutorial/1/bpf/xdp_prog.c -- -I../headers
func main() {
	log.Println("1 ebpf programming start")
	obj := EBPFTutorialObjects{}
	if err := LoadEBPFTutorialObjects(&obj, nil); err != nil {
		log.Fatalf("loading object failed %s", err)
	}
	defer obj.Close()

	nl, err := netlink.LinkByName("veth1")
	if err != nil {
		log.Fatalf("get link failed: %s", err)
	}

	if err := netlink.LinkSetXdpFdWithFlags(nl, obj.XdpProgSimple.FD(), 2); err != nil {
		log.Fatalf("link set xdp failed: %s", err)
	}

	return
}
