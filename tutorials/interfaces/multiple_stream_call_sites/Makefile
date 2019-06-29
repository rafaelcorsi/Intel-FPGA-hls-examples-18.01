SRC      := tutorial.cpp
DEVICE	 := Arria10
TARGETS  := tutorial-x86 \
            tutorial-fpga
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

tutorial-x86:            CXX = i++
tutorial-x86:            CXXFLAGS = -march=x86-64
tutorial-fpga:           CXX = i++
tutorial-fpga:           CXXFLAGS = -march=$(DEVICE)
