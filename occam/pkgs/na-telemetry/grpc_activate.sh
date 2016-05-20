#!/bin/sh
#
# $Id$
#
# Copyright 2015, Juniper Networks, Inc
# All Rights Reserved
#

mount_grpcd()
{
    # Send HUP to jalunchd so that grpcd can be started or stopped.
    sleep 10
    jlaunchd_pid=`cat /var/run/jlaunchd.pid`
    if [ -n "$jlaunchd_pid" ]; then
	kill -HUP $jlaunchd_pid
    else
	echo "GRPC: ERROR: Could not find PID of jlaunchd"
    fi
}

unmount_grpcd()
{

    #/bin/rm /usr/lib/dd/grpc.dml /usr/sbin/grpcd /usr/lib32/libgcc_s.so.1

    # Send HUP to jalunchd so that grpcd can be started or stopped.
    sleep 10
    jlaunchd_pid=`cat /var/run/jlaunchd.pid`
    if [ -n "$jlaunchd_pid" ]; then
        kill -HUP $jlaunchd_pid
    else
        echo "GRPC: ERROR: Could not find PID of jlaunchd"
    fi
}

set -x
date >&2
exec 2>>/var/tmp/grpc_activate.sh.log
case "${1:-activate}" in
mounted) mount_grpcd >&2;;
unmounting) unmount_grpcd >&2;;
esac
