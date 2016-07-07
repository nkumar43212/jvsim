#
# $Id$
#
# Copyright (c) 2014, Juniper Networks, Inc.
# All rights reserved.
#

LIB = oc

.PATH: ${.CURDIR}

SRCS = \
	oc.cpp \
	OpenConfigCpuMemoryUtilization.cpp \
	OpenConfigFirewall.cpp \
	OpenConfigFoo.cpp \
	OpenConfigInlineJflow.cpp \
	OpenConfigKeyvalue.cpp \
	OpenConfigLogicalPort.cpp \
	OpenConfigLspStats.cpp \
	OpenConfigNpuMem.cpp \
	OpenConfigNpuUtilization.cpp \
	OpenConfigOptics.cpp \
	OpenConfigPacketStats.cpp \
	OpenConfigPort.cpp \

CFLAGS += -I${RELSRCTOP}/junos/lib/h/

CXXFLAGS += -I${.CURDIR}
CXXFLAGS += -I${RELSRCTOP}/junos/lib/lib-protos/
CXXFLAGS += -I${RELSRCTOP}/junos/lib/libprotobuf/src/
CXXFLAGS += -I${RELSRCTOP}/junos/lib/lib-rpc-service/
CXXFLAGS += -I${RELSRCTOP}/junos/lib/libgrpc/include/
CXXFLAGS += -I${RELSRCTOP}/junos/lib/logger/
CXXFLAGS += -I${RELSRCTOP}/junos/lib/h/

.include <bsd.lib.mk>

CC := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang
CXX := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang++
CXXINCLUDES := -I/volume/publish/dev/occam/llvm-3.5/sysroot/stable_10/current/i386-unknown-freebsd10.1/usr/include/c++/v1
NO_INLINE_FUNCTIONS_CALLED_ONCE := ""
CXXFLAGS_LAST += -std=c++11 -stdlib=libc++
AR := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-ar

