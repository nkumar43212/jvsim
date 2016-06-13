#
# Makefile - Makefile for junos/usr.sbin/server
#
# Copyright (c) 2004-2008, Juniper Networks, Inc.
# All rights reserved.
#

PROG=   na-grpcd

NO_SHARED := yes

WARNINGS_SET=HIGH
W_aggregate-return=
W_cast-align_mips=

.PATH: ${.CURDIR}

# source files in each subdirectory
SRCS +=	\
	AgentConsolidator.cpp \
	AgentConsolidatorSystem.cpp \
	AgentMessageBus.cpp \
	AgentRestart.cpp \
	AgentServerCmdOptions.cpp \
	AgentServer.cpp \
	AgentServerIdManager.cpp \
	AgentSubscription.cpp \
	AgentSubscriptionLimits.cpp \
	AgentSubscriptionUdpWorker.cpp \
	AgentSystem.cpp \
	AgentSystemFactory.cpp \
	AgentSystemFile.cpp \
	AgentSystemNull.cpp \
	AgentSystemProc.cpp \
	AgentUtils.cpp \
	Counter.cpp \
	GlobalConfig.cpp \
	JsonUtils.cpp \
	JunosTelemetryJson.cpp \
	JunosTelemetryJsonGenerator.cpp \
	main.cpp \
	OCTelemetryJson.cpp \
	OCTelemetryJsonGenerator.cpp \
	OpenConfig.cpp \
	PathValidator.cpp \
	UdpReceiver.cpp
	
CFLAGS += -I. -I${.CURDIR}
CFLAGS += -I${RELSRCTOP}/junos/lib/h/

CXXFLAGS += -I. -I${.CURDIR}
CXXFLAGS += -I${RELSRCTOP_DIST}/mosquitto/lib/cpp -I${RELSRCTOP_DIST}/mosquitto/lib
CXXFLAGS += -I${RELSRCTOP}/junos/lib/libprotobuf/src/
CXXFLAGS += -I${RELSRCTOP}/junos/lib/libgrpc/include/
CXXFLAGS += -I${RELSRCTOP}/junos/lib/jsoncpp/include
CXXFLAGS += -I${RELSRCTOP}/junos/lib/logger/
CXXFLAGS += -I${RELSRCTOP}/junos/lib/h/
CXXFLAGS += -Wno-error

LDFLAGS :=
LDADD += -lc -lm -lz -lpthread /homes/mvivek/grpc_libs/lib/libgcc_s.so -L/volume/publish/dev/occam/llvm-3.5/freebsd/stable_10/current/stage/i386/usr/lib -L/volume/publish/dev/occam/llvm-3.5/sysroot/stable_10/current/i386-unknown-freebsd10.1/usr/lib
LDADD += -lmosquittopp -L${RELSRCTOP}/../obj/bsd10/i386/junos/lib/libmosquittopp
LDADD += -lmosquitto-nossl -L${RELSRCTOP}/../obj/bsd10/i386/junos/lib/libmosquitto
LDADD += -ljsoncpp -L${RELSRCTOP}/../obj/bsd10/i386/junos/lib/jsoncpp
LDADD += -loc -L${RELSRCTOP}/../obj/bsd10/i386/junos/lib/lib-oc
LDADD += -llogger -L${RELSRCTOP}/../obj/bsd10/i386/junos/lib/logger
LDADD += -lrpc-service -L${RELSRCTOP}/../obj/bsd10/i386/junos/lib/lib-rpc-service
LDADD += -lprotos -L${RELSRCTOP}/../obj/bsd10/i386/junos/lib/lib-protos
LDADD += -linih -L${RELSRCTOP}/../obj/bsd10/i386/junos/lib/lib-inih
LDADD += -llibgrpc -L${RELSRCTOP}/../obj/bsd10/i386/junos/lib/libgrpc
LDADD += -lprotobuf3 -L${RELSRCTOP}/../obj/bsd10/i386/junos/lib/libprotobuf
LDADD += /homes/mvivek/grpc_libs/crypto_unhidden/libssl.a /homes/mvivek/grpc_libs/crypto_unhidden/libcrypto.a

WARNINGS_SET=HIGH
W_cast-align_arm=

all: ${PROG} na-telemetry.dml

.SUFFIXES: .xml .dml

.xml.dml:
	@echo "processing $@"
	${HOST_CPP} ${DML_INCLUDES}  -P ${.IMPSRC} ${.TARGET}

na-telemetry.dml: na-telemetry.xml

.include <bsd.prog.mk>

CC := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang
CXX := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang++
NO_INLINE_FUNCTIONS_CALLED_ONCE :=
CXXFLAGS_LAST += -std=c++11 -stdlib=libc++
CXXINCLUDES := -I/volume/publish/dev/occam/llvm-3.5/sysroot/stable_10/current/i386-unknown-freebsd10.1/usr/include/c++/v1
_CXX := /volume/hab/Linux/Ubuntu-12.04/x86_64/llvm/3.5/20150720/i386-unknown-freebsd/bin/i386-unknown-freebsd10.1-clang++ -v
LDADD_LAST := -std=c++11 -stdlib=libc++ -L/volume/publish/dev/occam/llvm-3.5/freebsd/stable_10/current/stage/i386/usr/lib
DPLIBS :=
