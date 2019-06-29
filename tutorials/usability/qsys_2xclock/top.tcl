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
	set_component_parameter_value EXPLICIT_CLOCK_RATE {100000000.0}
	set_component_parameter_value NUM_CLOCK_OUTPUTS {1}
	set_component_project_property HIDE_FROM_IP_CATALOG {true}
	add_instantiation_interface in_clk clock INPUT
	set_instantiation_interface_parameter_value in_clk clockRate {0}
	set_instantiation_interface_parameter_value in_clk externallyDriven {false}
	set_instantiation_interface_parameter_value in_clk ptfSchematicName {}
	add_instantiation_interface_port in_clk in_clk clk 1 STD_LOGIC Input
	add_instantiation_interface out_clk clock OUTPUT
	set_instantiation_interface_parameter_value out_clk associatedDirectClock {in_clk}
	set_instantiation_interface_parameter_value out_clk clockRate {100000000}
	set_instantiation_interface_parameter_value out_clk clockRateKnown {true}
	set_instantiation_interface_parameter_value out_clk externallyDriven {false}
	set_instantiation_interface_parameter_value out_clk ptfSchematicName {}
	set_instantiation_interface_sysinfo_parameter_value out_clk clock_rate {100000000}
	add_instantiation_interface_port out_clk out_clk clk 1 STD_LOGIC Output
	save_instantiation
	add_component reset_in ip/top/top_reset_in.ip altera_reset_bridge reset_in
	load_component reset_in
	set_component_parameter_value ACTIVE_LOW_RESET {0}
	set_component_parameter_value NUM_RESET_OUTPUTS {1}
	set_component_parameter_value SYNCHRONOUS_EDGES {deassert}
	set_component_parameter_value USE_RESET_REQUEST {0}
	set_component_project_property HIDE_FROM_IP_CATALOG {true}
	add_instantiation_interface clk clock INPUT
	set_instantiation_interface_parameter_value clk clockRate {0}
	set_instantiation_interface_parameter_value clk externallyDriven {false}
	set_instantiation_interface_parameter_value clk ptfSchematicName {}
	add_instantiation_interface_port clk clk clk 1 STD_LOGIC Input
	add_instantiation_interface in_reset reset INPUT
	set_instantiation_interface_parameter_value in_reset associatedClock {clk}
	set_instantiation_interface_parameter_value in_reset synchronousEdges {DEASSERT}
	add_instantiation_interface_port in_reset in_reset reset 1 STD_LOGIC Input
	add_instantiation_interface out_reset reset OUTPUT
	set_instantiation_interface_parameter_value out_reset associatedClock {clk}
	set_instantiation_interface_parameter_value out_reset associatedDirectReset {in_reset}
	set_instantiation_interface_parameter_value out_reset associatedResetSinks {in_reset}
	set_instantiation_interface_parameter_value out_reset synchronousEdges {DEASSERT}
	add_instantiation_interface_port out_reset out_reset reset 1 STD_LOGIC Output
	save_instantiation
	add_instance top_foo_0 altera_generic_component
	load_instantiation top_foo_0
	set_instantiation_property HDL_COMPILATION_LIBRARY {foo}
	set_instantiation_property HDL_ENTITY_NAME {foo}
	set_instantiation_property IP_FILE {}
	add_instantiation_interface a conduit INPUT
	set_instantiation_interface_parameter_value a associatedClock {clock}
	set_instantiation_interface_parameter_value a associatedReset {reset}
	set_instantiation_interface_parameter_value a prSafe {false}
	set_instantiation_interface_assignment_value a hls.cosim.name {a}
	add_instantiation_interface_port a a data 32 STD_LOGIC_VECTOR Input
	add_instantiation_interface b conduit INPUT
	set_instantiation_interface_parameter_value b associatedClock {clock}
	set_instantiation_interface_parameter_value b associatedReset {reset}
	set_instantiation_interface_parameter_value b prSafe {false}
	set_instantiation_interface_assignment_value b hls.cosim.name {b}
	add_instantiation_interface_port b b data 32 STD_LOGIC_VECTOR Input
	add_instantiation_interface c conduit INPUT
	set_instantiation_interface_parameter_value c associatedClock {clock}
	set_instantiation_interface_parameter_value c associatedReset {reset}
	set_instantiation_interface_parameter_value c prSafe {false}
	set_instantiation_interface_assignment_value c hls.cosim.name {c}
	add_instantiation_interface_port c c data 32 STD_LOGIC_VECTOR Input
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
	add_instantiation_interface clock2x clock INPUT
	set_instantiation_interface_parameter_value clock2x clockRate {0}
	set_instantiation_interface_parameter_value clock2x externallyDriven {false}
	set_instantiation_interface_parameter_value clock2x ptfSchematicName {}
	set_instantiation_interface_assignment_value clock2x hls.cosim.name {$clock2x}
	add_instantiation_interface_port clock2x clock2x clk 1 STD_LOGIC Input
	add_instantiation_interface d conduit INPUT
	set_instantiation_interface_parameter_value d associatedClock {clock}
	set_instantiation_interface_parameter_value d associatedReset {reset}
	set_instantiation_interface_parameter_value d prSafe {false}
	set_instantiation_interface_assignment_value d hls.cosim.name {d}
	add_instantiation_interface_port d d data 32 STD_LOGIC_VECTOR Input
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
	save_instantiation
	add_component top_iopll_0 ip/top/top_iopll_0.ip altera_iopll top_iopll_0
	load_component top_iopll_0
	set_component_parameter_value gui_active_clk {0}
	set_component_parameter_value gui_c_cnt_in_src0 {c_m_cnt_in_src_ph_mux_clk}
	set_component_parameter_value gui_c_cnt_in_src1 {c_m_cnt_in_src_ph_mux_clk}
	set_component_parameter_value gui_c_cnt_in_src2 {c_m_cnt_in_src_ph_mux_clk}
	set_component_parameter_value gui_c_cnt_in_src3 {c_m_cnt_in_src_ph_mux_clk}
	set_component_parameter_value gui_c_cnt_in_src4 {c_m_cnt_in_src_ph_mux_clk}
	set_component_parameter_value gui_c_cnt_in_src5 {c_m_cnt_in_src_ph_mux_clk}
	set_component_parameter_value gui_c_cnt_in_src6 {c_m_cnt_in_src_ph_mux_clk}
	set_component_parameter_value gui_c_cnt_in_src7 {c_m_cnt_in_src_ph_mux_clk}
	set_component_parameter_value gui_c_cnt_in_src8 {c_m_cnt_in_src_ph_mux_clk}
	set_component_parameter_value gui_cal_code_hex_file {iossm.hex}
	set_component_parameter_value gui_cal_converge {0}
	set_component_parameter_value gui_cal_error {cal_clean}
	set_component_parameter_value gui_cascade_counter0 {0}
	set_component_parameter_value gui_cascade_counter1 {0}
	set_component_parameter_value gui_cascade_counter10 {0}
	set_component_parameter_value gui_cascade_counter11 {0}
	set_component_parameter_value gui_cascade_counter12 {0}
	set_component_parameter_value gui_cascade_counter13 {0}
	set_component_parameter_value gui_cascade_counter14 {0}
	set_component_parameter_value gui_cascade_counter15 {0}
	set_component_parameter_value gui_cascade_counter16 {0}
	set_component_parameter_value gui_cascade_counter17 {0}
	set_component_parameter_value gui_cascade_counter2 {0}
	set_component_parameter_value gui_cascade_counter3 {0}
	set_component_parameter_value gui_cascade_counter4 {0}
	set_component_parameter_value gui_cascade_counter5 {0}
	set_component_parameter_value gui_cascade_counter6 {0}
	set_component_parameter_value gui_cascade_counter7 {0}
	set_component_parameter_value gui_cascade_counter8 {0}
	set_component_parameter_value gui_cascade_counter9 {0}
	set_component_parameter_value gui_cascade_outclk_index {0}
	set_component_parameter_value gui_clk_bad {0}
	set_component_parameter_value gui_clock_name_global {0}
	set_component_parameter_value gui_clock_name_string0 {outclk1x}
	set_component_parameter_value gui_clock_name_string1 {outclk2x}
	set_component_parameter_value gui_clock_name_string10 {outclk10}
	set_component_parameter_value gui_clock_name_string11 {outclk11}
	set_component_parameter_value gui_clock_name_string12 {outclk12}
	set_component_parameter_value gui_clock_name_string13 {outclk13}
	set_component_parameter_value gui_clock_name_string14 {outclk14}
	set_component_parameter_value gui_clock_name_string15 {outclk15}
	set_component_parameter_value gui_clock_name_string16 {outclk16}
	set_component_parameter_value gui_clock_name_string17 {outclk17}
	set_component_parameter_value gui_clock_name_string2 {outclk2}
	set_component_parameter_value gui_clock_name_string3 {outclk3}
	set_component_parameter_value gui_clock_name_string4 {outclk4}
	set_component_parameter_value gui_clock_name_string5 {outclk5}
	set_component_parameter_value gui_clock_name_string6 {outclk6}
	set_component_parameter_value gui_clock_name_string7 {outclk7}
	set_component_parameter_value gui_clock_name_string8 {outclk8}
	set_component_parameter_value gui_clock_name_string9 {outclk9}
	set_component_parameter_value gui_clock_to_compensate {0}
	set_component_parameter_value gui_debug_mode {0}
	set_component_parameter_value gui_divide_factor_c0 {6}
	set_component_parameter_value gui_divide_factor_c1 {6}
	set_component_parameter_value gui_divide_factor_c10 {6}
	set_component_parameter_value gui_divide_factor_c11 {6}
	set_component_parameter_value gui_divide_factor_c12 {6}
	set_component_parameter_value gui_divide_factor_c13 {6}
	set_component_parameter_value gui_divide_factor_c14 {6}
	set_component_parameter_value gui_divide_factor_c15 {6}
	set_component_parameter_value gui_divide_factor_c16 {6}
	set_component_parameter_value gui_divide_factor_c17 {6}
	set_component_parameter_value gui_divide_factor_c2 {6}
	set_component_parameter_value gui_divide_factor_c3 {6}
	set_component_parameter_value gui_divide_factor_c4 {6}
	set_component_parameter_value gui_divide_factor_c5 {6}
	set_component_parameter_value gui_divide_factor_c6 {6}
	set_component_parameter_value gui_divide_factor_c7 {6}
	set_component_parameter_value gui_divide_factor_c8 {6}
	set_component_parameter_value gui_divide_factor_c9 {6}
	set_component_parameter_value gui_divide_factor_n {1}
	set_component_parameter_value gui_dps_cntr {C0}
	set_component_parameter_value gui_dps_dir {Positive}
	set_component_parameter_value gui_dps_num {1}
	set_component_parameter_value gui_dsm_out_sel {1st_order}
	set_component_parameter_value gui_duty_cycle0 {50.0}
	set_component_parameter_value gui_duty_cycle1 {50.0}
	set_component_parameter_value gui_duty_cycle10 {50.0}
	set_component_parameter_value gui_duty_cycle11 {50.0}
	set_component_parameter_value gui_duty_cycle12 {50.0}
	set_component_parameter_value gui_duty_cycle13 {50.0}
	set_component_parameter_value gui_duty_cycle14 {50.0}
	set_component_parameter_value gui_duty_cycle15 {50.0}
	set_component_parameter_value gui_duty_cycle16 {50.0}
	set_component_parameter_value gui_duty_cycle17 {50.0}
	set_component_parameter_value gui_duty_cycle2 {50.0}
	set_component_parameter_value gui_duty_cycle3 {50.0}
	set_component_parameter_value gui_duty_cycle4 {50.0}
	set_component_parameter_value gui_duty_cycle5 {50.0}
	set_component_parameter_value gui_duty_cycle6 {50.0}
	set_component_parameter_value gui_duty_cycle7 {50.0}
	set_component_parameter_value gui_duty_cycle8 {50.0}
	set_component_parameter_value gui_duty_cycle9 {50.0}
	set_component_parameter_value gui_en_adv_params {0}
	set_component_parameter_value gui_en_dps_ports {0}
	set_component_parameter_value gui_en_extclkout_ports {0}
	set_component_parameter_value gui_en_lvds_ports {Disabled}
	set_component_parameter_value gui_en_phout_ports {0}
	set_component_parameter_value gui_en_reconf {0}
	set_component_parameter_value gui_enable_cascade_in {0}
	set_component_parameter_value gui_enable_cascade_out {0}
	set_component_parameter_value gui_enable_mif_dps {0}
	set_component_parameter_value gui_enable_output_counter_cascading {0}
	set_component_parameter_value gui_existing_mif_file_path {~/pll.mif}
	set_component_parameter_value gui_extclkout_0_source {C0}
	set_component_parameter_value gui_extclkout_1_source {C0}
	set_component_parameter_value gui_feedback_clock {Global Clock}
	set_component_parameter_value gui_fix_vco_frequency {0}
	set_component_parameter_value gui_fixed_vco_frequency {600.0}
	set_component_parameter_value gui_frac_multiply_factor {1.0}
	set_component_parameter_value gui_fractional_cout {32}
	set_component_parameter_value gui_include_iossm {0}
	set_component_parameter_value gui_lock_setting {Low Lock Time}
	set_component_parameter_value gui_mif_config_name {unnamed}
	set_component_parameter_value gui_mif_gen_options {Generate New MIF File}
	set_component_parameter_value gui_multiply_factor {6}
	set_component_parameter_value gui_new_mif_file_path {~/pll.mif}
	set_component_parameter_value gui_number_of_clocks {2}
	set_component_parameter_value gui_operation_mode {direct}
	set_component_parameter_value gui_output_clock_frequency0 {100.0}
	set_component_parameter_value gui_output_clock_frequency1 {200.0}
	set_component_parameter_value gui_output_clock_frequency10 {100.0}
	set_component_parameter_value gui_output_clock_frequency11 {100.0}
	set_component_parameter_value gui_output_clock_frequency12 {100.0}
	set_component_parameter_value gui_output_clock_frequency13 {100.0}
	set_component_parameter_value gui_output_clock_frequency14 {100.0}
	set_component_parameter_value gui_output_clock_frequency15 {100.0}
	set_component_parameter_value gui_output_clock_frequency16 {100.0}
	set_component_parameter_value gui_output_clock_frequency17 {100.0}
	set_component_parameter_value gui_output_clock_frequency2 {100.0}
	set_component_parameter_value gui_output_clock_frequency3 {100.0}
	set_component_parameter_value gui_output_clock_frequency4 {100.0}
	set_component_parameter_value gui_output_clock_frequency5 {100.0}
	set_component_parameter_value gui_output_clock_frequency6 {100.0}
	set_component_parameter_value gui_output_clock_frequency7 {100.0}
	set_component_parameter_value gui_output_clock_frequency8 {100.0}
	set_component_parameter_value gui_output_clock_frequency9 {100.0}
	set_component_parameter_value gui_parameter_table_hex_file {seq_params_sim.hex}
	set_component_parameter_value gui_phase_shift0 {0.0}
	set_component_parameter_value gui_phase_shift1 {0.0}
	set_component_parameter_value gui_phase_shift10 {0.0}
	set_component_parameter_value gui_phase_shift11 {0.0}
	set_component_parameter_value gui_phase_shift12 {0.0}
	set_component_parameter_value gui_phase_shift13 {0.0}
	set_component_parameter_value gui_phase_shift14 {0.0}
	set_component_parameter_value gui_phase_shift15 {0.0}
	set_component_parameter_value gui_phase_shift16 {0.0}
	set_component_parameter_value gui_phase_shift17 {0.0}
	set_component_parameter_value gui_phase_shift2 {0.0}
	set_component_parameter_value gui_phase_shift3 {0.0}
	set_component_parameter_value gui_phase_shift4 {0.0}
	set_component_parameter_value gui_phase_shift5 {0.0}
	set_component_parameter_value gui_phase_shift6 {0.0}
	set_component_parameter_value gui_phase_shift7 {0.0}
	set_component_parameter_value gui_phase_shift8 {0.0}
	set_component_parameter_value gui_phase_shift9 {0.0}
	set_component_parameter_value gui_phase_shift_deg0 {0.0}
	set_component_parameter_value gui_phase_shift_deg1 {0.0}
	set_component_parameter_value gui_phase_shift_deg10 {0.0}
	set_component_parameter_value gui_phase_shift_deg11 {0.0}
	set_component_parameter_value gui_phase_shift_deg12 {0.0}
	set_component_parameter_value gui_phase_shift_deg13 {0.0}
	set_component_parameter_value gui_phase_shift_deg14 {0.0}
	set_component_parameter_value gui_phase_shift_deg15 {0.0}
	set_component_parameter_value gui_phase_shift_deg16 {0.0}
	set_component_parameter_value gui_phase_shift_deg17 {0.0}
	set_component_parameter_value gui_phase_shift_deg2 {0.0}
	set_component_parameter_value gui_phase_shift_deg3 {0.0}
	set_component_parameter_value gui_phase_shift_deg4 {0.0}
	set_component_parameter_value gui_phase_shift_deg5 {0.0}
	set_component_parameter_value gui_phase_shift_deg6 {0.0}
	set_component_parameter_value gui_phase_shift_deg7 {0.0}
	set_component_parameter_value gui_phase_shift_deg8 {0.0}
	set_component_parameter_value gui_phase_shift_deg9 {0.0}
	set_component_parameter_value gui_phout_division {1}
	set_component_parameter_value gui_pll_auto_reset {0}
	set_component_parameter_value gui_pll_bandwidth_preset {Low}
	set_component_parameter_value gui_pll_cal_done {0}
	set_component_parameter_value gui_pll_cascading_mode {adjpllin}
	set_component_parameter_value gui_pll_freqcal_en {1}
	set_component_parameter_value gui_pll_freqcal_req_flag {1}
	set_component_parameter_value gui_pll_m_cnt_in_src {c_m_cnt_in_src_ph_mux_clk}
	set_component_parameter_value gui_pll_mode {Integer-N PLL}
	set_component_parameter_value gui_pll_tclk_mux_en {0}
	set_component_parameter_value gui_pll_tclk_sel {pll_tclk_m_src}
	set_component_parameter_value gui_pll_type {S10_Simple}
	set_component_parameter_value gui_pll_vco_freq_band_0 {pll_freq_clk0_disabled}
	set_component_parameter_value gui_pll_vco_freq_band_1 {pll_freq_clk1_disabled}
	set_component_parameter_value gui_ps_units0 {ps}
	set_component_parameter_value gui_ps_units1 {ps}
	set_component_parameter_value gui_ps_units10 {ps}
	set_component_parameter_value gui_ps_units11 {ps}
	set_component_parameter_value gui_ps_units12 {ps}
	set_component_parameter_value gui_ps_units13 {ps}
	set_component_parameter_value gui_ps_units14 {ps}
	set_component_parameter_value gui_ps_units15 {ps}
	set_component_parameter_value gui_ps_units16 {ps}
	set_component_parameter_value gui_ps_units17 {ps}
	set_component_parameter_value gui_ps_units2 {ps}
	set_component_parameter_value gui_ps_units3 {ps}
	set_component_parameter_value gui_ps_units4 {ps}
	set_component_parameter_value gui_ps_units5 {ps}
	set_component_parameter_value gui_ps_units6 {ps}
	set_component_parameter_value gui_ps_units7 {ps}
	set_component_parameter_value gui_ps_units8 {ps}
	set_component_parameter_value gui_ps_units9 {ps}
	set_component_parameter_value gui_refclk1_frequency {100.0}
	set_component_parameter_value gui_refclk_might_change {0}
	set_component_parameter_value gui_refclk_switch {0}
	set_component_parameter_value gui_reference_clock_frequency {100.0}
	set_component_parameter_value gui_switchover_delay {0}
	set_component_parameter_value gui_switchover_mode {Automatic Switchover}
	set_component_parameter_value gui_use_NDFB_modes {0}
	set_component_parameter_value gui_use_locked {0}
	set_component_parameter_value gui_usr_device_speed_grade {1}
	set_component_parameter_value gui_vco_frequency {600.0}
	set_component_parameter_value hp_qsys_scripting_mode {0}
	set_component_project_property HIDE_FROM_IP_CATALOG {true}
	set_instantiation_assignment_value embeddedsw.dts.compatible {altr,pll}
	set_instantiation_assignment_value embeddedsw.dts.group {clock}
	set_instantiation_assignment_value embeddedsw.dts.vendor {altr}
	add_instantiation_interface outclk0 clock OUTPUT
	set_instantiation_interface_parameter_value outclk0 associatedDirectClock {}
	set_instantiation_interface_parameter_value outclk0 clockRate {100000000}
	set_instantiation_interface_parameter_value outclk0 clockRateKnown {true}
	set_instantiation_interface_parameter_value outclk0 externallyDriven {false}
	set_instantiation_interface_parameter_value outclk0 ptfSchematicName {}
	set_instantiation_interface_assignment_value outclk0 ui.blockdiagram.direction {output}
	set_instantiation_interface_sysinfo_parameter_value outclk0 clock_rate {100000000}
	add_instantiation_interface_port outclk0 outclk_0 clk 1 STD_LOGIC Output
	add_instantiation_interface outclk1 clock OUTPUT
	set_instantiation_interface_parameter_value outclk1 associatedDirectClock {}
	set_instantiation_interface_parameter_value outclk1 clockRate {200000000}
	set_instantiation_interface_parameter_value outclk1 clockRateKnown {true}
	set_instantiation_interface_parameter_value outclk1 externallyDriven {false}
	set_instantiation_interface_parameter_value outclk1 ptfSchematicName {}
	set_instantiation_interface_assignment_value outclk1 ui.blockdiagram.direction {output}
	set_instantiation_interface_sysinfo_parameter_value outclk1 clock_rate {200000000}
	add_instantiation_interface_port outclk1 outclk_1 clk 1 STD_LOGIC Output
	add_instantiation_interface refclk clock INPUT
	set_instantiation_interface_parameter_value refclk clockRate {100000000}
	set_instantiation_interface_parameter_value refclk externallyDriven {false}
	set_instantiation_interface_parameter_value refclk ptfSchematicName {}
	set_instantiation_interface_assignment_value refclk ui.blockdiagram.direction {input}
	add_instantiation_interface_port refclk refclk clk 1 STD_LOGIC Input
	add_instantiation_interface reset reset INPUT
	set_instantiation_interface_parameter_value reset associatedClock {}
	set_instantiation_interface_parameter_value reset synchronousEdges {NONE}
	set_instantiation_interface_assignment_value reset ui.blockdiagram.direction {input}
	add_instantiation_interface_port reset rst reset 1 STD_LOGIC Input
	save_instantiation
	add_component top_synchronizer_0 ip/top/top_synchronizer_0.ip synchronizer top_synchronizer_0 1.0
	load_component top_synchronizer_0
	set_component_project_property HIDE_FROM_IP_CATALOG {true}
	add_instantiation_interface clock clock INPUT
	set_instantiation_interface_parameter_value clock clockRate {0}
	set_instantiation_interface_parameter_value clock externallyDriven {false}
	set_instantiation_interface_parameter_value clock ptfSchematicName {}
	add_instantiation_interface_port clock clock clk 1 STD_LOGIC Input
	add_instantiation_interface resetn reset INPUT
	set_instantiation_interface_parameter_value resetn associatedClock {}
	set_instantiation_interface_parameter_value resetn synchronousEdges {NONE}
	add_instantiation_interface_port resetn resetn reset_n 1 STD_LOGIC Input
	add_instantiation_interface resetn_out reset OUTPUT
	set_instantiation_interface_parameter_value resetn_out associatedClock {clock}
	set_instantiation_interface_parameter_value resetn_out associatedDirectReset {}
	set_instantiation_interface_parameter_value resetn_out associatedResetSinks {resetn}
	set_instantiation_interface_parameter_value resetn_out synchronousEdges {DEASSERT}
	add_instantiation_interface_port resetn_out resetn_out reset_n 1 STD_LOGIC Output
	save_instantiation

	# add the connections
	add_connection clock_in.out_clk/reset_in.clk
	add_connection clock_in.out_clk/top_iopll_0.refclk
	add_connection reset_in.out_reset/top_iopll_0.reset
	add_connection reset_in.out_reset/top_synchronizer_0.resetn
	add_connection top_iopll_0.outclk0/top_foo_0.clock
	add_connection top_iopll_0.outclk0/top_synchronizer_0.clock
	add_connection top_iopll_0.outclk1/top_foo_0.clock2x
	add_connection top_synchronizer_0.resetn_out/top_foo_0.reset

	# add the exports
	set_interface_property a EXPORT_OF top_foo_0.a
	set_interface_property b EXPORT_OF top_foo_0.b
	set_interface_property c EXPORT_OF top_foo_0.c
	set_interface_property call EXPORT_OF top_foo_0.call
	set_interface_property clk EXPORT_OF clock_in.in_clk
	set_interface_property d EXPORT_OF top_foo_0.d
	set_interface_property reset EXPORT_OF reset_in.in_reset
	set_interface_property return EXPORT_OF top_foo_0.return
	set_interface_property returndata EXPORT_OF top_foo_0.returndata

	# set the the module properties
	set_module_property BONUS_DATA {<?xml version="1.0" encoding="UTF-8"?>
<bonusData>
 <element __value="$system">
  <datum __value="_originalDeviceFamily" value="Arria 10" type="String" />
 </element>
 <element __value="clock_in">
  <datum __value="_sortIndex" value="0" type="int" />
 </element>
 <element __value="reset_in">
  <datum __value="_sortIndex" value="1" type="int" />
 </element>
 <element __value="top_foo_0">
  <datum __value="_sortIndex" value="3" type="int" />
 </element>
 <element __value="top_iopll_0">
  <datum __value="_sortIndex" value="4" type="int" />
 </element>
 <element __value="top_synchronizer_0">
  <datum __value="_sortIndex" value="2" type="int" />
 </element>
</bonusData>
}
	set_module_property FILE {top.qsys}
	set_module_property GENERATION_ID {0x00000000}
	set_module_property NAME {top}
	set_module_property SYSTEM_SCRIPTS {}

	# save the system
	sync_sysinfo_parameters
	save_system top
}

# create all the systems, from bottom up
do_create_top
