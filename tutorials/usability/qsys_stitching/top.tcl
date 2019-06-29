package require -exact qsys 17.0

# create the system "top"
proc do_create_top {} {
	# create the system
	create_system top
	set_project_property DEVICE {10AX115U1F45I1SG}
	set_project_property DEVICE_FAMILY {Arria 10}
	set_project_property HIDE_FROM_IP_CATALOG {false}
	set_use_testbench_naming_pattern 0 {}
	set_interconnect_requirement {$system} qsys_mm.clockCrossingAdapter {HANDSHAKE}
	set_interconnect_requirement {$system} qsys_mm.enableEccProtection {FALSE}
	set_interconnect_requirement {$system} qsys_mm.insertDefaultSlave {FALSE}
	set_interconnect_requirement {$system} qsys_mm.maxAdditionalLatency {1}

	# add the components
	add_component clock_in ip/top/top_clock_in.ip altera_clock_bridge clock_in
	load_component clock_in
	set_component_parameter_value EXPLICIT_CLOCK_RATE {50000000.0}
	set_component_parameter_value NUM_CLOCK_OUTPUTS {1}
	set_component_project_property HIDE_FROM_IP_CATALOG {true}
	save_component
	load_instantiation clock_in
	remove_instantiation_interface_port in_clk in_clk
	remove_instantiation_interface in_clk
	remove_instantiation_interface_port out_clk out_clk
	remove_instantiation_interface out_clk
	add_instantiation_interface in_clk clock INPUT
	set_instantiation_interface_parameter_value in_clk externallyDriven {false}
	set_instantiation_interface_parameter_value in_clk ptfSchematicName {}
	add_instantiation_interface_port in_clk in_clk clk 1 STD_LOGIC Input
	add_instantiation_interface out_clk clock OUTPUT
	set_instantiation_interface_parameter_value out_clk associatedDirectClock {in_clk}
	set_instantiation_interface_parameter_value out_clk externallyDriven {false}
	set_instantiation_interface_parameter_value out_clk ptfSchematicName {}
	set_instantiation_interface_sysinfo_parameter_value out_clk clock_rate {50000000}
	add_instantiation_interface_port out_clk out_clk clk 1 STD_LOGIC Output
	save_instantiation
	add_component reset_in ip/top/top_reset_in.ip altera_reset_bridge reset_in
	load_component reset_in
	set_component_parameter_value ACTIVE_LOW_RESET {1}
	set_component_parameter_value NUM_RESET_OUTPUTS {1}
	set_component_parameter_value SYNCHRONOUS_EDGES {deassert}
	set_component_parameter_value USE_RESET_REQUEST {0}
	set_component_project_property HIDE_FROM_IP_CATALOG {true}
	save_component
	load_instantiation reset_in
	remove_instantiation_interface_port clk clk
	remove_instantiation_interface clk
	remove_instantiation_interface_port in_reset in_reset_n
	remove_instantiation_interface in_reset
	remove_instantiation_interface_port out_reset out_reset_n
	remove_instantiation_interface out_reset
	add_instantiation_interface clk clock INPUT
	set_instantiation_interface_parameter_value clk externallyDriven {false}
	set_instantiation_interface_parameter_value clk ptfSchematicName {}
	add_instantiation_interface_port clk clk clk 1 STD_LOGIC Input
	add_instantiation_interface in_reset reset INPUT
	set_instantiation_interface_parameter_value in_reset associatedClock {clk}
	set_instantiation_interface_parameter_value in_reset synchronousEdges {DEASSERT}
	add_instantiation_interface_port in_reset in_reset_n reset_n 1 STD_LOGIC Input
	add_instantiation_interface out_reset reset OUTPUT
	set_instantiation_interface_parameter_value out_reset associatedClock {clk}
	set_instantiation_interface_parameter_value out_reset associatedDirectReset {in_reset}
	set_instantiation_interface_parameter_value out_reset associatedResetSinks {in_reset}
	set_instantiation_interface_parameter_value out_reset synchronousEdges {DEASSERT}
	add_instantiation_interface_port out_reset out_reset_n reset_n 1 STD_LOGIC Output
	save_instantiation
	add_component top_hpf_0 tutorial-fpga.prj/components/hpf/hpf.ip hpf_internal hpf_internal_inst 1.0
	load_component top_hpf_0
	set_component_project_property HIDE_FROM_IP_CATALOG {false}
	save_component
	load_instantiation top_hpf_0
	remove_instantiation_interface_port alpha alpha
	remove_instantiation_interface alpha
	remove_instantiation_interface_port call busy
	remove_instantiation_interface_port call start
	remove_instantiation_interface call
	remove_instantiation_interface_port clock clock
	remove_instantiation_interface clock
	remove_instantiation_interface_port reset resetn
	remove_instantiation_interface reset
	remove_instantiation_interface_port return done
	remove_instantiation_interface_port return stall
	remove_instantiation_interface return
	remove_instantiation_interface_port returndata returndata
	remove_instantiation_interface returndata
	remove_instantiation_interface_port x x
	remove_instantiation_interface x
	set_instantiation_assignment_value hls.cosim.name {hpf}
	add_instantiation_interface alpha conduit INPUT
	set_instantiation_interface_parameter_value alpha associatedClock {clock}
	set_instantiation_interface_parameter_value alpha associatedReset {reset}
	set_instantiation_interface_parameter_value alpha prSafe {false}
	set_instantiation_interface_assignment_value alpha hls.cosim.name {alpha}
	add_instantiation_interface_port alpha alpha data 32 STD_LOGIC_VECTOR Input
	add_instantiation_interface call conduit INPUT
	set_instantiation_interface_parameter_value call associatedClock {clock}
	set_instantiation_interface_parameter_value call associatedReset {reset}
	set_instantiation_interface_parameter_value call prSafe {false}
	set_instantiation_interface_assignment_value call hls.cosim.name {$call}
	add_instantiation_interface_port call busy stall 1 STD_LOGIC Output
	add_instantiation_interface_port call start valid 1 STD_LOGIC Input
	add_instantiation_interface clock clock INPUT
	set_instantiation_interface_parameter_value clock clockRate {0}
	set_instantiation_interface_parameter_value clock externallyDriven {false}
	set_instantiation_interface_parameter_value clock ptfSchematicName {}
	set_instantiation_interface_assignment_value clock hls.cosim.name {$clock}
	add_instantiation_interface_port clock clock clk 1 STD_LOGIC Input
	add_instantiation_interface reset reset INPUT
	set_instantiation_interface_parameter_value reset associatedClock {clock}
	set_instantiation_interface_parameter_value reset synchronousEdges {DEASSERT}
	set_instantiation_interface_assignment_value reset hls.cosim.name {$reset}
	add_instantiation_interface_port reset resetn reset_n 1 STD_LOGIC Input
	add_instantiation_interface return conduit INPUT
	set_instantiation_interface_parameter_value return associatedClock {clock}
	set_instantiation_interface_parameter_value return associatedReset {reset}
	set_instantiation_interface_parameter_value return prSafe {false}
	set_instantiation_interface_assignment_value return hls.cosim.name {$return}
	add_instantiation_interface_port return done valid 1 STD_LOGIC Output
	add_instantiation_interface_port return stall stall 1 STD_LOGIC Input
	add_instantiation_interface returndata conduit INPUT
	set_instantiation_interface_parameter_value returndata associatedClock {clock}
	set_instantiation_interface_parameter_value returndata associatedReset {reset}
	set_instantiation_interface_parameter_value returndata prSafe {false}
	set_instantiation_interface_assignment_value returndata hls.cosim.name {$returndata}
	add_instantiation_interface_port returndata returndata data 32 STD_LOGIC_VECTOR Output
	add_instantiation_interface x conduit INPUT
	set_instantiation_interface_parameter_value x associatedClock {clock}
	set_instantiation_interface_parameter_value x associatedReset {reset}
	set_instantiation_interface_parameter_value x prSafe {false}
	set_instantiation_interface_assignment_value x hls.cosim.name {x}
	add_instantiation_interface_port x x data 32 STD_LOGIC_VECTOR Input
	save_instantiation
	add_component top_lpf_0 tutorial-fpga.prj/components/lpf/lpf.ip lpf_internal lpf_internal_inst 1.0
	load_component top_lpf_0
	set_component_project_property HIDE_FROM_IP_CATALOG {false}
	save_component
	load_instantiation top_lpf_0
	remove_instantiation_interface_port alpha alpha
	remove_instantiation_interface alpha
	remove_instantiation_interface_port call busy
	remove_instantiation_interface_port call start
	remove_instantiation_interface call
	remove_instantiation_interface_port clock clock
	remove_instantiation_interface clock
	remove_instantiation_interface_port reset resetn
	remove_instantiation_interface reset
	remove_instantiation_interface_port return done
	remove_instantiation_interface_port return stall
	remove_instantiation_interface return
	remove_instantiation_interface_port returndata returndata
	remove_instantiation_interface returndata
	remove_instantiation_interface_port x x
	remove_instantiation_interface x
	add_instantiation_interface alpha conduit INPUT
	set_instantiation_interface_parameter_value alpha associatedClock {clock}
	set_instantiation_interface_parameter_value alpha associatedReset {reset}
	set_instantiation_interface_parameter_value alpha prSafe {false}
	set_instantiation_interface_assignment_value alpha hls.cosim.name {alpha}
	add_instantiation_interface_port alpha alpha data 32 STD_LOGIC_VECTOR Input
	add_instantiation_interface call conduit INPUT
	set_instantiation_interface_parameter_value call associatedClock {clock}
	set_instantiation_interface_parameter_value call associatedReset {reset}
	set_instantiation_interface_parameter_value call prSafe {false}
	set_instantiation_interface_assignment_value call hls.cosim.name {$call}
	add_instantiation_interface_port call busy stall 1 STD_LOGIC Output
	add_instantiation_interface_port call start valid 1 STD_LOGIC Input
	add_instantiation_interface clock clock INPUT
	set_instantiation_interface_parameter_value clock externallyDriven {false}
	set_instantiation_interface_parameter_value clock ptfSchematicName {}
	set_instantiation_interface_assignment_value clock hls.cosim.name {$clock}
	add_instantiation_interface_port clock clock clk 1 STD_LOGIC Input
	add_instantiation_interface reset reset INPUT
	set_instantiation_interface_parameter_value reset associatedClock {clock}
	set_instantiation_interface_parameter_value reset synchronousEdges {DEASSERT}
	set_instantiation_interface_assignment_value reset hls.cosim.name {$reset}
	add_instantiation_interface_port reset resetn reset_n 1 STD_LOGIC Input
	add_instantiation_interface return conduit INPUT
	set_instantiation_interface_parameter_value return associatedClock {clock}
	set_instantiation_interface_parameter_value return associatedReset {reset}
	set_instantiation_interface_parameter_value return prSafe {false}
	set_instantiation_interface_assignment_value return hls.cosim.name {$return}
	add_instantiation_interface_port return done valid 1 STD_LOGIC Output
	add_instantiation_interface_port return stall stall 1 STD_LOGIC Input
	add_instantiation_interface returndata conduit INPUT
	set_instantiation_interface_parameter_value returndata associatedClock {clock}
	set_instantiation_interface_parameter_value returndata associatedReset {reset}
	set_instantiation_interface_parameter_value returndata prSafe {false}
	set_instantiation_interface_assignment_value returndata hls.cosim.name {$returndata}
	add_instantiation_interface_port returndata returndata data 32 STD_LOGIC_VECTOR Output
	add_instantiation_interface x conduit INPUT
	set_instantiation_interface_parameter_value x associatedClock {clock}
	set_instantiation_interface_parameter_value x associatedReset {reset}
	set_instantiation_interface_parameter_value x prSafe {false}
	set_instantiation_interface_assignment_value x hls.cosim.name {x}
	add_instantiation_interface_port x x data 32 STD_LOGIC_VECTOR Input
	save_instantiation

	# add the connections
	add_connection clock_in.out_clk/reset_in.clk
	add_connection clock_in.out_clk/top_hpf_0.clock
	add_connection clock_in.out_clk/top_lpf_0.clock
	add_connection reset_in.out_reset/top_hpf_0.reset
	add_connection reset_in.out_reset/top_lpf_0.reset
	add_connection top_hpf_0.call/top_lpf_0.return
	set_connection_parameter_value top_hpf_0.call/top_lpf_0.return endPort {}
	set_connection_parameter_value top_hpf_0.call/top_lpf_0.return endPortLSB {0}
	set_connection_parameter_value top_hpf_0.call/top_lpf_0.return startPort {}
	set_connection_parameter_value top_hpf_0.call/top_lpf_0.return startPortLSB {0}
	set_connection_parameter_value top_hpf_0.call/top_lpf_0.return width {0}
	add_connection top_lpf_0.returndata/top_hpf_0.x
	set_connection_parameter_value top_lpf_0.returndata/top_hpf_0.x endPort {}
	set_connection_parameter_value top_lpf_0.returndata/top_hpf_0.x endPortLSB {0}
	set_connection_parameter_value top_lpf_0.returndata/top_hpf_0.x startPort {}
	set_connection_parameter_value top_lpf_0.returndata/top_hpf_0.x startPortLSB {0}
	set_connection_parameter_value top_lpf_0.returndata/top_hpf_0.x width {0}

	# add the exports
	set_interface_property clk EXPORT_OF clock_in.in_clk
	set_interface_property reset EXPORT_OF reset_in.in_reset
	set_interface_property top_hpf_0_alpha EXPORT_OF top_hpf_0.alpha
	set_interface_property top_hpf_0_return EXPORT_OF top_hpf_0.return
	set_interface_property top_hpf_0_returndata EXPORT_OF top_hpf_0.returndata
	set_interface_property top_lpf_0_alpha EXPORT_OF top_lpf_0.alpha
	set_interface_property top_lpf_0_call EXPORT_OF top_lpf_0.call
	set_interface_property top_lpf_0_x EXPORT_OF top_lpf_0.x

	# set the the module properties
	set_module_property BONUS_DATA {<?xml version="1.0" encoding="UTF-8"?>
<bonusData>
 <element __value="clock_in">
  <datum __value="_sortIndex" value="0" type="int" />
 </element>
 <element __value="reset_in">
  <datum __value="_sortIndex" value="1" type="int" />
 </element>
 <element __value="top_hpf_0">
  <datum __value="_sortIndex" value="2" type="int" />
 </element>
 <element __value="top_lpf_0">
  <datum __value="_sortIndex" value="3" type="int" />
 </element>
</bonusData>
}
	set_module_property FILE {top.qsys}
	set_module_property GENERATION_ID {0x00000000}
	set_module_property NAME {top}
	set_module_property SYSTEM_SCRIPTS {}

	# save the system
	sync_sysinfo_parameters
	save_system top.qsys
}

# create all the systems, from bottom up
do_create_top
