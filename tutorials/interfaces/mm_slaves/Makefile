DEVICE   := Arria10
TARGETS  := part_1_basic_component \
            part_2_slave_component \
            part_3_slave_register_arguments \
            part_4_slave_mem_args
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE) -ghdl
RM       := rm -rfv

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) :
	$(CXX) $(CXXFLAGS) $@.cpp -o $@
