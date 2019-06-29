SOURCE_FILES  := counter.cpp
HLS_CXX_FLAGS := 
CXX := i++
override CXXFLAGS := $(CXXFLAGS)
VERBOSE := 1

# OS-dependant tools
ifeq ($(OS),Windows_NT)
  RM  := rd /S /Q
else
  RM  := rm -rfv
endif

ifeq ($(MAKECMDGOALS),)
  $(info No target specified, defaulting to test-x86-64)
  $(info Available targets: test-x86-64, test-fpga, test-gpp, clean)
endif

# Any tools installed with HLS can be found relative to the location of i++
HLS_INSTALL_DIR := $(shell which i++ | sed 's|/bin/i++||g')

# Run the i++ x86 test by default
.PHONY: default
default: test-x86-64

# Compile the component and testbench using g++ and run them as a regular program
.PHONY: test-gpp
test-gpp: CXX := g++
test-gpp: CXXFLAGS := $(CXXFLAGS) -I"$(HLS_INSTALL_DIR)/include" -L"$(HLS_INSTALL_DIR)/host/linux64/lib" -lhls_emul -o test-gpp
test-gpp: $(SOURCE_FILES)
	$(CXX) $(SOURCE_FILES) $(CXXFLAGS)
	@echo "+-------------------------------------+"
	@echo "| Run ./test-gpp to execute the test. |"
	@echo "+-------------------------------------+"

# Run the testbench and the component as a regular program
.PHONY: test-x86-64
test-x86-64: CXXFLAGS := $(CXXFLAGS) $(HLS_CXX_FLAGS) -march=x86-64 -o test-x86-64
test-x86-64: $(SOURCE_FILES)
	$(CXX) $(SOURCE_FILES) $(CXXFLAGS)
	@echo "+----------------------------------------+"
	@echo "| Run ./test-x86-64 to execute the test. |"
	@echo "+----------------------------------------+"

# Run a simulation with the C testbench and verilog component
.PHONY: test-fpga
ifeq ($(VERBOSE),1)
  test-fpga: CXXFLAGS := $(CXXFLAGS) -v
endif
test-fpga: CXXFLAGS := $(CXXFLAGS) $(HLS_CXX_FLAGS) -march=Arria10 -o test-fpga
test-fpga: $(SOURCE_FILES)
	$(CXX) $(SOURCE_FILES) $(CXXFLAGS)
	@echo "+--------------------------------------+"
	@echo "| Run ./test-fpga to execute the test. |"
	@echo "+--------------------------------------+"

# Clean up temprary and delivered files
CLEAN_FILES := test-gpp \
               test-gpp.prj \
               test-fpga \
               test-fpga.prj \
               test-x86-64 \
               test-x86-64.prj
clean:
	-$(RM) $(CLEAN_FILES)
