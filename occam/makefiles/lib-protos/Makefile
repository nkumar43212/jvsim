#
# $Id$
#
# Copyright (c) 2014, Juniper Networks, Inc.
# All rights reserved.
#

LIB = protos

.PATH: ${.CURDIR}

SRCS = \
        agent.pb.cc \
	authentication_service.pb.cc \
        cpu_memory_utilization.pb.cc \
        firewall.pb.cc \
        Foo.pb.cc \
        inventory.pb.cc \
        iptunnels.pb.cc \
	keyvalue.pb.cc \
        logical_port.pb.cc \
        lsp_stats.pb.cc \
        mgd_service.pb.cc \
        npu_mem.pb.cc \
        npu_utilization.pb.cc \
        openconfig_mgd.pb.cc \
        optics.pb.cc \
        packet_stats.pb.cc \
	port.pb.cc \
	telemetry_top.pb.cc \
	test.pb.cc \

CFLAGS += -I${RELSRCTOP}/junos/lib/h/

CXXFLAGS += -I${.CURDIR}
CXXFLAGS += -I${RELSRCTOP}/junos/lib/libprotobuf/src/
CXXFLAGS += -I${RELSRCTOP}/junos/lib/h/

.include <bsd.lib.mk>

CC := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang
CXX := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang++
CXXINCLUDES := -I/volume/publish/dev/occam/llvm-3.5/sysroot/stable_10/current/i386-unknown-freebsd10.1/usr/include/c++/v1
CXXFLAGS_LAST += -std=c++11 -stdlib=libc++
NO_INLINE_FUNCTIONS_CALLED_ONCE := ""
AR := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-ar

