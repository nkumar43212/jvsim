#
# $Id$
#
# Copyright (c) 2014, Juniper Networks, Inc.
# All rights reserved.
#

LIB = rpc-service

.PATH: ${.CURDIR}

SRCS = \
	agent.grpc.pb.cc \
	authentication_service.grpc.pb.cc \
	mgd_service.grpc.pb.cc \
	openconfig_mgd.grpc.pb.cc \

CFLAGS += -I${RELSRCTOP}/junos/lib/h/

CXXFLAGS += -I${.CURDIR}
CXXFLAGS += -I${RELSRCTOP}/junos/lib/libprotobuf/src/
CXXFLAGS += -I${RELSRCTOP}/junos/lib/libgrpc/include/
CXXFLAGS += -I${RELSRCTOP}/junos/lib/h/

.include <bsd.lib.mk>

CC := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang
CXX := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang++
CXXINCLUDES := -I/volume/publish/dev/occam/llvm-3.5/sysroot/stable_10/current/i386-unknown-freebsd10.1/usr/include/c++/v1
NO_INLINE_FUNCTIONS_CALLED_ONCE := ""
CXXFLAGS_LAST += -std=c++11 -stdlib=libc++
AR := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-ar

