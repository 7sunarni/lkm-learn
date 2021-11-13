package main

import (
	"io/ioutil"
	"log"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/vishvananda/netlink"
)

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -cc clang EBPFTutorial /home/cooker/ForChange/git.forchange.cn/zhanglixin/lkm-learn/ebpf-tutorial/3/bpf/xdp_prog.c -- -I../headers

const (
	/*
		    /usr/local/linux/bpf.h
			enum xdp_action {
				XDP_ABORTED = 0,
				XDP_DROP,
				XDP_PASS,
				XDP_TX,
				XDP_REDIRECT,
			};
	*/
	XDP_ABORTED  uint32 = 0
	XDP_DROP     uint32 = 1
	XDP_PASS     uint32 = 2
	XDP_TX       uint32 = 3
	XDP_REDIRECT uint32 = 4
)

func main() {
	data, err := ioutil.ReadFile("./ebpftutorial_bpfel.o")
	if err != nil {
		log.Fatal(err)
	}

	log.Println(data)
}

func main1() {
	log.Println("3 ebpf programming start")
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

	if err := netlink.LinkSetXdpFdWithFlags(nl, obj.XdpPassF.FD(), 2); err != nil {
		log.Fatalf("link set xdp failed: %s", err)
	}

	ticker := time.NewTicker(time.Second)
	for {
		select {
		case <-ticker.C:
			data, err := obj.EBPFTutorialMaps.XdpStatsMap.LookupBytes(XDP_PASS)
			if err != nil {
				log.Fatalf("look up key failed: %s", err)
			}
			log.Println(len(data))
		case <-stopper:
			return
		}
	}
}

type value struct {
	RcvPacket uint64
	RcvByte   uint64
}
