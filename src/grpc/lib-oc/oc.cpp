//
//  !! This is a generated file. Do not edit !!
// 
//  oc.cpp
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include <iostream>
#include "lib_oc.h"
#include "OpenConfigFirewall.hpp"
#include "OpenConfigInterface.hpp"
#include "OpenConfigPort.hpp"
#include "OpenConfigFoo.hpp"
#include "OpenConfigPacketStats.hpp"
#include "OpenConfigNPU_Utilization.hpp"
//@header

void
lib_oc_init ()
{
    OpenConfigFirewall *fw    = new OpenConfigFirewall;
    OpenConfigInterface *intf = new OpenConfigInterface;
    OpenConfigPort      *port = new OpenConfigPort;
    OpenConfigFoo	 *Foo = new OpenConfigFoo;
    OpenConfigPacketStats	 *PacketStats = new OpenConfigPacketStats;
    OpenConfigNPU_Utilization	 *NPU_Utilization = new OpenConfigNPU_Utilization;
//@next
}
