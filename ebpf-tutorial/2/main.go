package main

import (
	"log"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/vishvananda/netlink"
)

const mapKey uint32 = 2

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -cc clang EBPFTutorial /home/cooker/ForChange/git.forchange.cn/zhanglixin/lkm-learn/ebpf-tutorial/2/bpf/xdp_prog.c -- -I../headers

func main() {
	log.Println("2 ebpf programming start")
	stopper := make(chan os.Signal, 1)
	signal.Notify(stopper, os.Interrupt, syscall.SIGTERM)
	obj := EBPFTutorialObjects{}
	if err := LoadEBPFTutorialObjects(&obj, nil); err != nil {
		log.Fatalf("loading object failed %s", err)
	}

	nl, err := netlink.LinkByName("veth1")
	if err != nil {
		log.Fatalf("get link failed: %s", err)
	}

	if err := netlink.LinkSetXdpFdWithFlags(nl, obj.XdpStats1Func.FD(), 2); err != nil {
		log.Fatalf("link set xdp failed: %s", err)
	}

	ticker := time.NewTicker(1 * time.Second)
	log.Println("Waiting for events..")

	for {
		select {
		case <-ticker.C:
			var value uint64
			if err := obj.XdpStatsMap.Lookup(mapKey, &value); err != nil {
				log.Fatalf("reading map: %v", err)
			}
			log.Printf("called %d times\n", value)
		case <-stopper:
			return
		}
	}

	return
}
