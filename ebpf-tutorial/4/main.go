package main

import (
	"log"
	"os"
	"os/signal"
	"syscall"

	"github.com/vishvananda/netlink"
	"github.com/vishvananda/netns"
)

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -cc clang EBPFTutorial /home/cooker/ForChange/git.forchange.cn/zhanglixin/lkm-learn/ebpf-tutorial/4/bpf/xdp_prog.c -- -I../headers

func main() {
	log.Println("4 ebpf programming start")

	stopper := make(chan os.Signal, 1)
	signal.Notify(stopper, os.Interrupt, syscall.SIGTERM)
	obj := EBPFTutorialObjects{}
	if err := LoadEBPFTutorialObjects(&obj, nil); err != nil {
		log.Fatalf("loading object failed %s", err)
	}

	nsHandler, err := netns.GetFromName("ns1")
	if err != nil {
		log.Fatalf("netns get ns handler failed %s", err)
	}

	if err := netns.Set(nsHandler); err != nil {
		log.Fatalf("netns set ns handler failed %s", err)
	}

	nl, err := netlink.LinkByName("veth0")
	if err != nil {
		log.Fatalf("get link failed: %s", err)
	}

	if err := netlink.LinkSetXdpFdWithFlags(nl, obj.XdpProgSimple.FD(), 2); err != nil {
		log.Fatalf("link set xdp failed: %s", err)
	}

}
