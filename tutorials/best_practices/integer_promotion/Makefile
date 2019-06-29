DEVICE   := Arria10
SRC      := tutorial.cpp
TARGETS  := tutorial-gcc \
            tutorial-promote-x86 \
            tutorial-x86 \
            tutorial-promote-fpga \
            tutorial-fpga
RM       := rm -rfv

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
tutorial-gcc:            CXXFLAGS = -I$(HLS_INSTALL_DIR)/include -L$(HLS_INSTALL_DIR)/host/linux64/lib -lhls_emul
tutorial-x86:            CXX = i++
tutorial-x86:            CXXFLAGS = -march=x86-64
tutorial-promote-x86:    CXX = i++
tutorial-promote-x86:    CXXFLAGS = -march=x86-64 \
                                    --promote-integers
tutorial-fpga:           CXX = i++
tutorial-fpga:           CXXFLAGS = -march=$(DEVICE)
tutorial-promote-fpga:   CXX = i++
tutorial-promote-fpga:   CXXFLAGS = -march=$(DEVICE) \
                                    --promote-integers
