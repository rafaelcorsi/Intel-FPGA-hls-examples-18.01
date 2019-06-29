# qsys scripting (.tcl) file for unsaved
package require -exact qsys 16.0

create_system {unsaved}

set_project_property DEVICE_FAMILY {Arria 10}
set_project_property DEVICE {10AX115U1F45I1SG}
set_project_property HIDE_FROM_IP_CATALOG {false}

# Instances and instance parameters
# (disabled instances are intentionally culled)
add_instance clk_0 clock_source
set_instance_parameter_value clk_0 {clockFrequency} {50000000.0}
set_instance_parameter_value clk_0 {clockFrequencyKnown} {1}
set_instance_parameter_value clk_0 {resetSynchronousEdges} {NONE}

add_instance top_hpf_0 hpf 1.0

add_instance top_lpf_0 lpf 1.0

# exported interfaces
add_interface clk clock sink
set_interface_property clk EXPORT_OF clk_0.clk_in
add_interface top_hpf_0_alpha conduit end
set_interface_property top_hpf_0_alpha EXPORT_OF top_hpf_0.alpha
add_interface top_hpf_0_return conduit end
set_interface_property top_hpf_0_return EXPORT_OF top_hpf_0.return
add_interface top_hpf_0_returndata conduit end
set_interface_property top_hpf_0_returndata EXPORT_OF top_hpf_0.returndata
add_interface top_lpf_0_alpha conduit end
set_interface_property top_lpf_0_alpha EXPORT_OF top_lpf_0.alpha
add_interface top_lpf_0_call conduit end
set_interface_property top_lpf_0_call EXPORT_OF top_lpf_0.call
add_interface top_lpf_0_x conduit end
set_interface_property top_lpf_0_x EXPORT_OF top_lpf_0.x
add_interface reset reset sink
set_interface_property reset EXPORT_OF clk_0.clk_in_reset

# connections and connection parameters
add_connection clk_0.clk top_hpf_0.clock

add_connection clk_0.clk top_lpf_0.clock

add_connection clk_0.clk_reset top_hpf_0.reset

add_connection clk_0.clk_reset top_lpf_0.reset

add_connection top_hpf_0.x top_lpf_0.returndata
set_connection_parameter_value top_hpf_0.x/top_lpf_0.returndata endPort {}
set_connection_parameter_value top_hpf_0.x/top_lpf_0.returndata endPortLSB {0}
set_connection_parameter_value top_hpf_0.x/top_lpf_0.returndata startPort {}
set_connection_parameter_value top_hpf_0.x/top_lpf_0.returndata startPortLSB {0}
set_connection_parameter_value top_hpf_0.x/top_lpf_0.returndata width {0}

add_connection top_lpf_0.return top_hpf_0.call
set_connection_parameter_value top_lpf_0.return/top_hpf_0.call endPort {}
set_connection_parameter_value top_lpf_0.return/top_hpf_0.call endPortLSB {0}
set_connection_parameter_value top_lpf_0.return/top_hpf_0.call startPort {}
set_connection_parameter_value top_lpf_0.return/top_hpf_0.call startPortLSB {0}
set_connection_parameter_value top_lpf_0.return/top_hpf_0.call width {0}

# interconnect requirements
set_interconnect_requirement {$system} {qsys_mm.clockCrossingAdapter} {HANDSHAKE}
set_interconnect_requirement {$system} {qsys_mm.enableEccProtection} {FALSE}
set_interconnect_requirement {$system} {qsys_mm.insertDefaultSlave} {FALSE}
set_interconnect_requirement {$system} {qsys_mm.maxAdditionalLatency} {1}

save_system {top.qsys}
