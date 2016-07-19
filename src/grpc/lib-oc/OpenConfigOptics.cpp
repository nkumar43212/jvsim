//
//  OpenConfigOptics.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 1/12/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#include "OpenConfigOptics.hpp"
#include "oc.hpp"

std::string BASE_OC_PATH_OPTICS("/oc-path/interfaces/interface");
std::string BASE_OC_PATH_OPTICS_ATTR("optics");

void
OpenConfigOptics::iterate (JuniperNetworksSensors *handle, telemetry::OpenConfigData *datap)
{

    Optics *message = handle->MutableExtension(jnpr_optics_ext);
    telemetry::KeyValue *kv;

    for (int i = 0; i < message->optics_diag_size(); i++) {
        const OpticsInfos& element = message->optics_diag(i);
        const OpticsDiagStats &diags = element.optics_diag_stats();

        // Prefix
        oc_set_prefix(datap, BASE_OC_PATH_OPTICS, element.if_name(), BASE_OC_PATH_OPTICS_ATTR);

        if (diags.optics_type()) {
            kv = datap->add_kv();
            kv->set_key("optics_type");
            kv->set_uint_value(diags.optics_type());
        }

        if (diags.module_temp()) {
            kv = datap->add_kv();
            kv->set_key("module_temp");
            kv->set_double_value(diags.module_temp());
        }

        if (diags.module_temp_high_alarm_threshold()) {
            kv = datap->add_kv();
            kv->set_key("module_temp_high_alarm_threshold");
            kv->set_double_value(diags.module_temp_high_alarm_threshold());
        }

        if (diags.module_temp_low_alarm_threshold()) {
            kv = datap->add_kv();
            kv->set_key("module_temp_low_alarm_threshold");
            kv->set_double_value(diags.module_temp_low_alarm_threshold());
        }

        if (diags.module_temp_high_warning_threshold()) {
            kv = datap->add_kv();
            kv->set_key("module_temp_high_warning_threshold");
            kv->set_double_value(diags.module_temp_high_warning_threshold());
        }

        if (diags.module_temp_low_warning_threshold()) {
            kv = datap->add_kv();
            kv->set_key("module_temp_low_warning_threshold");
            kv->set_double_value(diags.module_temp_low_warning_threshold());
        }

        if (diags.laser_output_power_high_alarm_threshold_dbm()) {
            kv = datap->add_kv();
            kv->set_key("laser_output_power_high_alarm_threshold_dbm");
            kv->set_double_value(diags.laser_output_power_high_alarm_threshold_dbm());
        }

        if (diags.laser_output_power_low_alarm_threshold_dbm()) {
            kv = datap->add_kv();
            kv->set_key("laser_output_power_low_alarm_threshold_dbm");
            kv->set_double_value(diags.laser_output_power_low_alarm_threshold_dbm());
        }

        if (diags.laser_output_power_high_warning_threshold_dbm()) {
            kv = datap->add_kv();
            kv->set_key("laser_output_power_high_warning_threshold_dbm");
            kv->set_double_value(diags.laser_output_power_high_warning_threshold_dbm());
        }

        if (diags.laser_output_power_low_warning_threshold_dbm()) {
            kv = datap->add_kv();
            kv->set_key("laser_output_power_low_warning_threshold_dbm");
            kv->set_double_value(diags.laser_output_power_low_warning_threshold_dbm());
        }

        if (diags.laser_rx_power_high_alarm_threshold_dbm()) {
            kv = datap->add_kv();
            kv->set_key("laser_rx_power_high_alarm_threshold_dbm");
            kv->set_double_value(diags.laser_rx_power_high_alarm_threshold_dbm());
        }

        if (diags.laser_rx_power_low_alarm_threshold_dbm()) {
            kv = datap->add_kv();
            kv->set_key("laser_rx_power_low_alarm_threshold_dbm");
            kv->set_double_value(diags.laser_rx_power_low_alarm_threshold_dbm());
        }

        if (diags.laser_rx_power_high_warning_threshold_dbm()) {
            kv = datap->add_kv();
            kv->set_key("laser_rx_power_high_warning_threshold_dbm");
            kv->set_double_value(diags.laser_rx_power_high_warning_threshold_dbm());
        }

        if (diags.laser_rx_power_low_warning_threshold_dbm()) {
            kv = datap->add_kv();
            kv->set_key("laser_rx_power_low_warning_threshold_dbm");
            kv->set_double_value(diags.laser_rx_power_low_warning_threshold_dbm());
		}

        if (diags.laser_bias_current_high_alarm_threshold()) {
            kv = datap->add_kv();
            kv->set_key("laser_bias_current_high_alarm_threshold");
            kv->set_double_value(diags.laser_bias_current_high_alarm_threshold());
        }

        if (diags.laser_bias_current_low_alarm_threshold()) {
            kv = datap->add_kv();
            kv->set_key("laser_bias_current_low_alarm_threshold");
            kv->set_double_value(diags.laser_bias_current_low_alarm_threshold());
        }

        if (diags.laser_bias_current_high_warning_threshold()) {
            kv = datap->add_kv();
            kv->set_key("laser_bias_current_high_warning_threshold");
            kv->set_double_value(diags.laser_bias_current_high_warning_threshold());
        }

        if (diags.laser_bias_current_low_warning_threshold()) {
            kv = datap->add_kv();
            kv->set_key("laser_bias_current_low_warning_threshold");
            kv->set_double_value(diags.laser_bias_current_low_warning_threshold());
        }

        if (diags.module_temp_high_alarm()) {
            kv = datap->add_kv();
            kv->set_key("module_temp_high_alarm");
            kv->set_bool_value(diags.module_temp_high_alarm());
        }

        if (diags.module_temp_low_alarm()) {
            kv = datap->add_kv();
            kv->set_key("module_temp_low_alarm");
            kv->set_bool_value(diags.module_temp_low_alarm());
        }

        if (diags.module_temp_high_warning()) {
            kv = datap->add_kv();
            kv->set_key("module_temp_high_warning");
            kv->set_bool_value(diags.module_temp_high_warning());
        }

        if (diags.module_temp_low_warning()) {
            kv = datap->add_kv();
            kv->set_key("module_temp_low_warning");
            kv->set_bool_value(diags.module_temp_low_warning());
        }

		for (int j = 0; j < diags.optics_lane_diag_stats_size(); j++) {
            const OpticsDiagLaneStats &lanediags = diags.optics_lane_diag_stats(j);

            std::string lanediag("lanediags/");
            std::string lanenumber;

            if (lanediags.lane_number()) {
                kv = datap->add_kv();
                lanenumber = std::to_string(lanediags.lane_number());
                kv->set_key(lanediag + lanenumber + "/lane_number");
                kv->set_uint_value(lanediags.lane_number());
            } else {
                lanenumber = "unknown";
            }

            if (lanediags.lane_laser_temperature()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_temperature");
                kv->set_double_value(lanediags.lane_laser_temperature());
            }

            if (lanediags.lane_laser_output_power_dbm()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_output_power_dbm");
                kv->set_double_value(lanediags.lane_laser_output_power_dbm());
            }

            if (lanediags.lane_laser_receiver_power_dbm()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_receiver_power_dbm");
                kv->set_double_value(lanediags.lane_laser_receiver_power_dbm());
            }

            if (lanediags.lane_laser_bias_current()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_bias_current");
                kv->set_double_value(lanediags.lane_laser_bias_current());
            }

            if (lanediags.lane_laser_output_power_high_alarm()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_output_power_high_alarm");
                kv->set_bool_value(lanediags.lane_laser_output_power_high_alarm());
            }

            if (lanediags.lane_laser_output_power_low_alarm()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_output_power_low_alarm");
                kv->set_bool_value(lanediags.lane_laser_output_power_low_alarm());
            }

            if (lanediags.lane_laser_output_power_high_warning()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_output_power_high_warning");
				kv->set_bool_value(lanediags.lane_laser_output_power_high_warning());
            }

            if (lanediags.lane_laser_output_power_low_warning()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_output_power_low_warning");
                kv->set_bool_value(lanediags.lane_laser_output_power_low_warning());
            }

            if (lanediags.lane_laser_receiver_power_high_alarm()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_receiver_power_high_alarm");
                kv->set_bool_value(lanediags.lane_laser_receiver_power_high_alarm());
            }

            if (lanediags.lane_laser_receiver_power_low_alarm()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_receiver_power_low_alarm");
                kv->set_bool_value(lanediags.lane_laser_receiver_power_low_alarm());
            }

            if (lanediags.lane_laser_receiver_power_high_warning()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_receiver_power_high_warning");
                kv->set_bool_value(lanediags.lane_laser_receiver_power_high_warning());
            }

            if (lanediags.lane_laser_receiver_power_low_warning()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_receiver_power_low_warning");
                kv->set_bool_value(lanediags.lane_laser_receiver_power_low_warning());
            }

            if (lanediags.lane_laser_bias_current_high_alarm()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_bias_current_high_alarm");
                kv->set_bool_value(lanediags.lane_laser_bias_current_high_alarm());
            }

            if (lanediags.lane_laser_bias_current_low_alarm()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_bias_current_low_alarm");
                kv->set_bool_value(lanediags.lane_laser_bias_current_low_alarm());
            }

            if (lanediags.lane_laser_bias_current_high_warning()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_bias_current_high_warning");
                kv->set_bool_value(lanediags.lane_laser_bias_current_high_warning());
			}

            if (lanediags.lane_laser_bias_current_low_warning()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_laser_bias_current_low_warning");
                kv->set_bool_value(lanediags.lane_laser_bias_current_low_warning());
			}

            if (lanediags.lane_tx_loss_of_signal_alarm()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_tx_loss_of_signal_alarm");
                kv->set_bool_value(lanediags.lane_tx_loss_of_signal_alarm());
            }

            if (lanediags.lane_rx_loss_of_signal_alarm()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_rx_loss_of_signal_alarm");
                kv->set_bool_value(lanediags.lane_rx_loss_of_signal_alarm());
            }

            if (lanediags.lane_tx_laser_disabled_alarm()) {
                kv = datap->add_kv();
                kv->set_key(lanediag + lanenumber + "/lane_tx_laser_disabled_alarm");
                kv->set_bool_value(lanediags.lane_tx_laser_disabled_alarm());
            }
        }
    }
}
