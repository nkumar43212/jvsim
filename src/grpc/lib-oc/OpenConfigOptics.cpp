//
//  OpenConfigOptics.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigOptics.hpp"

void
OpenConfigOptics::iterate (JuniperNetworksSensors *handle, Telemetry::OpenConfigData *datap)
{

    Optics *message = handle->MutableExtension(jnpr_optics_ext);
    Telemetry::KeyValue *kv;
    
    for (int i = 0; i < message->optics_diag_size(); i++) {
        const OpticsInfos& element = message->optics_diag(i);
        const OpticsDiagStats &diags = element.optics_diag_stats();

        kv = datap->add_kv();
        kv->set_key("__prefix__");
        kv->set_str_value("oc-path/optics/" + element.if_name());
        
	if (diags.module_temp()) {
		kv = datap->add_kv();
		kv->set_key("module_temp");
		kv->set_int_value(diags.module_temp());
	}

	if (diags.module_temp_c()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_c");
		kv->set_int_value(diags.module_temp_c());
	}

	if (diags.module_temp_f()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_f");
		kv->set_int_value(diags.module_temp_f());
	}

	if (diags.module_temp_set()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_set");
		kv->set_int_value(diags.module_temp_set());
	}

	if (diags.receiver_signal_ave_optical_power()) {
		kv = datap->add_kv();
		kv->set_key("receiver_signal_ave_optical_power");
		kv->set_int_value(diags.receiver_signal_ave_optical_power());
	}

	if (diags.receiver_signal_ave_optical_power_mw()) {
		kv = datap->add_kv();
		kv->set_key("receiver_signal_ave_optical_power_mw");
		kv->set_int_value(diags.receiver_signal_ave_optical_power_mw());
	}

	if (diags.receiver_signal_ave_optical_power_dbm()) {
		kv = datap->add_kv();
		kv->set_key("receiver_signal_ave_optical_power_dbm");
		kv->set_int_value(diags.receiver_signal_ave_optical_power_dbm());
	}

	if (diags.receiver_signal_ave_optical_power_set()) {
		kv = datap->add_kv();
		kv->set_key("receiver_signal_ave_optical_power_set");
		kv->set_int_value(diags.receiver_signal_ave_optical_power_set());
	}

	if (diags.laser_output_power()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power");
		kv->set_int_value(diags.laser_output_power());
	}

	if (diags.laser_output_power_mw()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_mw");
		kv->set_int_value(diags.laser_output_power_mw());
	}

	if (diags.laser_output_power_dbm()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_dbm");
		kv->set_int_value(diags.laser_output_power_dbm());
	}

	if (diags.laser_output_power_set()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_set");
		kv->set_int_value(diags.laser_output_power_set());
	}

	if (diags.module_temp_high_alarm_threshold()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_high_alarm_threshold");
		kv->set_int_value(diags.module_temp_high_alarm_threshold());
	}

	if (diags.module_temp_high_alarm_threshold_c()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_high_alarm_threshold_c");
		kv->set_int_value(diags.module_temp_high_alarm_threshold_c());
	}

	if (diags.module_temp_high_alarm_threshold_f()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_high_alarm_threshold_f");
		kv->set_int_value(diags.module_temp_high_alarm_threshold_f());
	}

	if (diags.module_temp_high_alarm_threshold_set()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_high_alarm_threshold_set");
		kv->set_int_value(diags.module_temp_high_alarm_threshold_set());
	}

	if (diags.module_temp_low_alarm_threshold()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_low_alarm_threshold");
		kv->set_int_value(diags.module_temp_low_alarm_threshold());
	}

	if (diags.module_temp_low_alarm_threshold_c()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_low_alarm_threshold_c");
		kv->set_int_value(diags.module_temp_low_alarm_threshold_c());
	}

	if (diags.module_temp_low_alarm_threshold_f()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_low_alarm_threshold_f");
		kv->set_int_value(diags.module_temp_low_alarm_threshold_f());
	}

	if (diags.module_temp_low_alarm_threshold_set()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_low_alarm_threshold_set");
		kv->set_int_value(diags.module_temp_low_alarm_threshold_set());
	}

	if (diags.module_temp_high_warning_threshold()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_high_warning_threshold");
		kv->set_int_value(diags.module_temp_high_warning_threshold());
	}

	if (diags.module_temp_high_warning_threshold_c()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_high_warning_threshold_c");
		kv->set_int_value(diags.module_temp_high_warning_threshold_c());
	}

	if (diags.module_temp_high_warning_threshold_f()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_high_warning_threshold_f");
		kv->set_int_value(diags.module_temp_high_warning_threshold_f());
	}

	if (diags.module_temp_high_warning_threshold_set()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_high_warning_threshold_set");
		kv->set_int_value(diags.module_temp_high_warning_threshold_set());
	}

	if (diags.module_temp_low_warning_threshold()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_low_warning_threshold");
		kv->set_int_value(diags.module_temp_low_warning_threshold());
	}

	if (diags.module_temp_low_warning_threshold_c()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_low_warning_threshold_c");
		kv->set_int_value(diags.module_temp_low_warning_threshold_c());
	}

	if (diags.module_temp_low_warning_threshold_f()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_low_warning_threshold_f");
		kv->set_int_value(diags.module_temp_low_warning_threshold_f());
	}

	if (diags.module_temp_low_warning_threshold_set()) {
		kv = datap->add_kv();
		kv->set_key("module_temp_low_warning_threshold_set");
		kv->set_int_value(diags.module_temp_low_warning_threshold_set());
	}

	if (diags.laser_output_power_high_alarm_threshold()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_high_alarm_threshold");
		kv->set_int_value(diags.laser_output_power_high_alarm_threshold());
	}

	if (diags.laser_output_power_high_alarm_threshold_mw()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_high_alarm_threshold_mw");
		kv->set_int_value(diags.laser_output_power_high_alarm_threshold_mw());
	}

	if (diags.laser_output_power_high_alarm_threshold_dbm()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_high_alarm_threshold_dbm");
		kv->set_int_value(diags.laser_output_power_high_alarm_threshold_dbm());
	}

	if (diags.laser_output_power_high_alarm_threshold_set()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_high_alarm_threshold_set");
		kv->set_int_value(diags.laser_output_power_high_alarm_threshold_set());
	}

	if (diags.laser_output_power_low_alarm_threshold()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_low_alarm_threshold");
		kv->set_int_value(diags.laser_output_power_low_alarm_threshold());
	}

	if (diags.laser_output_power_low_alarm_threshold_mw()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_low_alarm_threshold_mw");
		kv->set_int_value(diags.laser_output_power_low_alarm_threshold_mw());
	}

	if (diags.laser_output_power_low_alarm_threshold_dbm()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_low_alarm_threshold_dbm");
		kv->set_int_value(diags.laser_output_power_low_alarm_threshold_dbm());
	}

	if (diags.laser_output_power_low_alarm_threshold_set()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_low_alarm_threshold_set");
		kv->set_int_value(diags.laser_output_power_low_alarm_threshold_set());
	}

	if (diags.laser_output_power_high_warning_threshold()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_high_warning_threshold");
		kv->set_int_value(diags.laser_output_power_high_warning_threshold());
	}

	if (diags.laser_output_power_high_warning_threshold_mw()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_high_warning_threshold_mw");
		kv->set_int_value(diags.laser_output_power_high_warning_threshold_mw());
	}

	if (diags.laser_output_power_high_warning_threshold_dbm()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_high_warning_threshold_dbm");
		kv->set_int_value(diags.laser_output_power_high_warning_threshold_dbm());
	}

	if (diags.laser_output_power_high_warning_threshold_set()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_high_warning_threshold_set");
		kv->set_int_value(diags.laser_output_power_high_warning_threshold_set());
	}

	if (diags.laser_output_power_low_warning_threshold()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_low_warning_threshold");
		kv->set_int_value(diags.laser_output_power_low_warning_threshold());
	}

	if (diags.laser_output_power_low_warning_threshold_mw()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_low_warning_threshold_mw");
		kv->set_int_value(diags.laser_output_power_low_warning_threshold_mw());
	}

	if (diags.laser_output_power_low_warning_threshold_dbm()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_low_warning_threshold_dbm");
		kv->set_int_value(diags.laser_output_power_low_warning_threshold_dbm());
	}

	if (diags.laser_output_power_low_warning_threshold_set()) {
		kv = datap->add_kv();
		kv->set_key("laser_output_power_low_warning_threshold_set");
		kv->set_int_value(diags.laser_output_power_low_warning_threshold_set());
	}

	if (diags.laser_rx_power_high_alarm_threshold()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_high_alarm_threshold");
		kv->set_int_value(diags.laser_rx_power_high_alarm_threshold());
	}

	if (diags.laser_rx_power_high_alarm_threshold_mw()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_high_alarm_threshold_mw");
		kv->set_int_value(diags.laser_rx_power_high_alarm_threshold_mw());
	}

	if (diags.laser_rx_power_high_alarm_threshold_dbm()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_high_alarm_threshold_dbm");
		kv->set_int_value(diags.laser_rx_power_high_alarm_threshold_dbm());
	}

	if (diags.laser_rx_power_high_alarm_threshold_set()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_high_alarm_threshold_set");
		kv->set_int_value(diags.laser_rx_power_high_alarm_threshold_set());
	}

	if (diags.laser_rx_power_low_alarm_threshold()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_low_alarm_threshold");
		kv->set_int_value(diags.laser_rx_power_low_alarm_threshold());
	}

	if (diags.laser_rx_power_low_alarm_threshold_mw()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_low_alarm_threshold_mw");
		kv->set_int_value(diags.laser_rx_power_low_alarm_threshold_mw());
	}

	if (diags.laser_rx_power_low_alarm_threshold_dbm()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_low_alarm_threshold_dbm");
		kv->set_int_value(diags.laser_rx_power_low_alarm_threshold_dbm());
	}

	if (diags.laser_rx_power_low_alarm_threshold_set()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_low_alarm_threshold_set");
		kv->set_int_value(diags.laser_rx_power_low_alarm_threshold_set());
	}

	if (diags.laser_rx_power_high_warning_threshold()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_high_warning_threshold");
		kv->set_int_value(diags.laser_rx_power_high_warning_threshold());
	}

	if (diags.laser_rx_power_high_warning_threshold_mw()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_high_warning_threshold_mw");
		kv->set_int_value(diags.laser_rx_power_high_warning_threshold_mw());
	}

	if (diags.laser_rx_power_high_warning_threshold_dbm()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_high_warning_threshold_dbm");
		kv->set_int_value(diags.laser_rx_power_high_warning_threshold_dbm());
	}

	if (diags.laser_rx_power_high_warning_threshold_set()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_high_warning_threshold_set");
		kv->set_int_value(diags.laser_rx_power_high_warning_threshold_set());
	}

	if (diags.laser_rx_power_low_warning_threshold()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_low_warning_threshold");
		kv->set_int_value(diags.laser_rx_power_low_warning_threshold());
	}

	if (diags.laser_rx_power_low_warning_threshold_mw()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_low_warning_threshold_mw");
		kv->set_int_value(diags.laser_rx_power_low_warning_threshold_mw());
	}

	if (diags.laser_rx_power_low_warning_threshold_dbm()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_low_warning_threshold_dbm");
		kv->set_int_value(diags.laser_rx_power_low_warning_threshold_dbm());
	}

	if (diags.laser_rx_power_low_warning_threshold_set()) {
		kv = datap->add_kv();
		kv->set_key("laser_rx_power_low_warning_threshold_set");
		kv->set_int_value(diags.laser_rx_power_low_warning_threshold_set());
	}

	if (diags.lane_number()) {
		kv = datap->add_kv();
		kv->set_key("lane_number");
		kv->set_int_value(diags.lane_number());
	}

	if (diags.lane_laser_temp()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_temp");
		kv->set_int_value(diags.lane_laser_temp());
	}

	if (diags.lane_laser_temp_c()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_temp_c");
		kv->set_int_value(diags.lane_laser_temp_c());
	}

	if (diags.lane_laser_temp_f()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_temp_f");
		kv->set_int_value(diags.lane_laser_temp_f());
	}

	if (diags.lane_laser_temp_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_temp_set");
		kv->set_int_value(diags.lane_laser_temp_set());
	}

	if (diags.lane_laser_output_power()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_output_power");
		kv->set_int_value(diags.lane_laser_output_power());
	}

	if (diags.lane_laser_output_power_mw()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_output_power_mw");
		kv->set_int_value(diags.lane_laser_output_power_mw());
	}

	if (diags.lane_laser_output_power_dbm()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_output_power_dbm");
		kv->set_int_value(diags.lane_laser_output_power_dbm());
	}

	if (diags.lane_laser_output_power_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_output_power_set");
		kv->set_int_value(diags.lane_laser_output_power_set());
	}

	if (diags.lane_laser_receiver_power()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_receiver_power");
		kv->set_int_value(diags.lane_laser_receiver_power());
	}

	if (diags.lane_laser_receiver_power_mw()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_receiver_power_mw");
		kv->set_int_value(diags.lane_laser_receiver_power_mw());
	}

	if (diags.lane_laser_receiver_power_dbm()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_receiver_power_dbm");
		kv->set_int_value(diags.lane_laser_receiver_power_dbm());
	}

	if (diags.lane_laser_receiver_power_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_receiver_power_set");
		kv->set_int_value(diags.lane_laser_receiver_power_set());
	}

	if (diags.lane_laser_temp_high_alarm_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_temp_high_alarm_set");
		kv->set_int_value(diags.lane_laser_temp_high_alarm_set());
	}

	if (diags.lane_laser_temp_high_alarm()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_temp_high_alarm");
		kv->set_int_value(diags.lane_laser_temp_high_alarm());
	}

	if (diags.lane_laser_temp_low_alarm_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_temp_low_alarm_set");
		kv->set_int_value(diags.lane_laser_temp_low_alarm_set());
	}

	if (diags.lane_laser_temp_low_alarm()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_temp_low_alarm");
		kv->set_int_value(diags.lane_laser_temp_low_alarm());
	}

	if (diags.lane_laser_temp_high_warning_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_temp_high_warning_set");
		kv->set_int_value(diags.lane_laser_temp_high_warning_set());
	}

	if (diags.lane_laser_temp_high_warning()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_temp_high_warning");
		kv->set_int_value(diags.lane_laser_temp_high_warning());
	}

	if (diags.lane_laser_output_power_high_alarm_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_output_power_high_alarm_set");
		kv->set_int_value(diags.lane_laser_output_power_high_alarm_set());
	}

	if (diags.lane_laser_output_power_high_alarm()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_output_power_high_alarm");
		kv->set_int_value(diags.lane_laser_output_power_high_alarm());
	}

	if (diags.lane_laser_output_power_low_alarm_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_output_power_low_alarm_set");
		kv->set_int_value(diags.lane_laser_output_power_low_alarm_set());
	}

	if (diags.lane_laser_output_power_low_alarm()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_output_power_low_alarm");
		kv->set_int_value(diags.lane_laser_output_power_low_alarm());
	}

	if (diags.lane_laser_output_power_high_warning_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_output_power_high_warning_set");
		kv->set_int_value(diags.lane_laser_output_power_high_warning_set());
	}

	if (diags.lane_laser_output_power_high_warning()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_output_power_high_warning");
		kv->set_int_value(diags.lane_laser_output_power_high_warning());
	}

	if (diags.lane_laser_output_power_low_warning_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_output_power_low_warning_set");
		kv->set_int_value(diags.lane_laser_output_power_low_warning_set());
	}

	if (diags.lane_laser_output_power_low_warning()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_output_power_low_warning");
		kv->set_int_value(diags.lane_laser_output_power_low_warning());
	}

	if (diags.lane_laser_receiver_power_high_alarm_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_receiver_power_high_alarm_set");
		kv->set_int_value(diags.lane_laser_receiver_power_high_alarm_set());
	}

	if (diags.lane_laser_receiver_power_high_alarm()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_receiver_power_high_alarm");
		kv->set_int_value(diags.lane_laser_receiver_power_high_alarm());
	}

	if (diags.lane_laser_receiver_power_low_alarm_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_receiver_power_low_alarm_set");
		kv->set_int_value(diags.lane_laser_receiver_power_low_alarm_set());
	}

	if (diags.lane_laser_receiver_power_low_alarm()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_receiver_power_low_alarm");
		kv->set_int_value(diags.lane_laser_receiver_power_low_alarm());
	}

	if (diags.lane_laser_receiver_power_high_warning_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_receiver_power_high_warning_set");
		kv->set_int_value(diags.lane_laser_receiver_power_high_warning_set());
	}

	if (diags.lane_laser_receiver_power_high_warning()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_receiver_power_high_warning");
		kv->set_int_value(diags.lane_laser_receiver_power_high_warning());
	}

	if (diags.lane_laser_receiver_power_low_warning_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_receiver_power_low_warning_set");
		kv->set_int_value(diags.lane_laser_receiver_power_low_warning_set());
	}

	if (diags.lane_laser_receiver_power_low_warning()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_receiver_power_low_warning");
		kv->set_int_value(diags.lane_laser_receiver_power_low_warning());
	}

	if (diags.optics_type()) {
		kv = datap->add_kv();
		kv->set_key("optics_type");
		kv->set_int_value(diags.optics_type());
	}

	if (diags.optics_alarm_validity()) {
		kv = datap->add_kv();
		kv->set_key("optics_alarm_validity");
		kv->set_int_value(diags.optics_alarm_validity());
	}

	if (diags.optics_capability()) {
		kv = datap->add_kv();
		kv->set_key("optics_capability");
		kv->set_int_value(diags.optics_capability());
	}

	if (diags.active_lane_map()) {
		kv = datap->add_kv();
		kv->set_key("active_lane_map");
		kv->set_int_value(diags.active_lane_map());
	}

	if (diags.lane_alarms_warnings()) {
		kv = datap->add_kv();
		kv->set_key("lane_alarms_warnings");
		kv->set_int_value(diags.lane_alarms_warnings());
	}

	if (diags.lane_laser_temp_low_warning_set()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_temp_low_warning_set");
		kv->set_int_value(diags.lane_laser_temp_low_warning_set());
	}

	if (diags.lane_laser_temp_low_warning()) {
		kv = datap->add_kv();
		kv->set_key("lane_laser_temp_low_warning");
		kv->set_int_value(diags.lane_laser_temp_low_warning());
	}
	}
}

