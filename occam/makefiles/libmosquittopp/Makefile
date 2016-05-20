#
# $Id$
#
# Copyright (c) 2015, Juniper Networks Inc.
# All rights reserved.
#
LIB = mosquittopp

.PATH: ${RELSRCTOP_DIST}/mosquitto/lib/cpp

SRCS = mosquittopp.cpp

CFLAGS += \
	-I${.CURDIR} \
	-I${RELSRCTOP_DIST}/mosquitto/lib/cpp \
	-I${RELSRCTOP_DIST}/mosquitto/lib \

.include <bsd.lib.mk>

CC := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang
CXX := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang++
NO_INLINE_FUNCTIONS_CALLED_ONCE := ""
CXXFLAGS_LAST += -std=c++11 -stdlib=libc++
CXXINCLUDES := -I/volume/publish/dev/occam/llvm-3.5/sysroot/stable_10/current/i386-unknown-freebsd10.1/usr/include/c++/v1
AR := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-ar

