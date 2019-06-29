DEVICE   := Arria10
SRC      := bandpassfilter.cpp
TARGETS  := tutorial-fpga
RM       := rm -rf
QSYS_VER := $(shell quartus_sh --version | grep Pro &> /dev/null; echo $$?)

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj) top.qsys top.qsf top.qpf db ip qdb top *.ipx *.iipx

$(TARGETS): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@
	ip-make-ipx --thorough-descent
ifeq ($(QSYS_VER),0)
	qsys-script --script=top.tcl --quartus-project=none
else
	qsys-script --script=top_std.tcl
endif
	qsys-generate top.qsys --simulation=VERILOG
	vsim -batch -do test.tcl

tutorial-fpga:           CXX = i++
tutorial-fpga:           CXXFLAGS = -march=$(DEVICE) --simulator none
