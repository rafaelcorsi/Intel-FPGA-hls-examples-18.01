DEVICE   := Arria10
SRC      := tutorial.cpp
TARGETS  := tutorial-gcc \
            tutorial-x86 \
            tutorial-fpga
CXXFLAGS := $(USERCXXFLAGS)
RM       := rm -rf

HLS_INSTALL_DIR := $(shell which i++ | sed 's|/bin/i++||g')

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) : % : $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

tutorial-gcc:            CXX = g++
tutorial-gcc:            CXXFLAGS += -I$(HLS_INSTALL_DIR)/include -L$(HLS_INSTALL_DIR)/host/linux64/lib/ -lhls_emul
tutorial-x86:            CXX = i++
tutorial-x86:            CXXFLAGS += -march=x86-64
tutorial-fpga:           CXX = i++
tutorial-fpga:           CXXFLAGS += -march=$(DEVICE) -ghdl
