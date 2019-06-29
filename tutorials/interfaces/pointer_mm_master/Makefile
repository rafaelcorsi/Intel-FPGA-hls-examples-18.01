DEVICE   := Arria10
TARGETS  := part_1_pointers \
            part_2_masters \
            part_3_ddr_masters \
            part_4_ddr_masters_coalesce \
            part_5_ddr_masters_align
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
