DEVICE   := Arria10
TARGETS  := part_1_pointer \
            part_2_mm_master \
            part_3_mm_slave \
            part_4_stream \
            part_5_pass_by_value

CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE) -ghdl
RM       := rm -rf

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
