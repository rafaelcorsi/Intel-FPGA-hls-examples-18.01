DEVICE   := Arria10
TARGETS  := part1_threshold \
            part2_threshold_packets \
            part3_threshold_no_ready_valid
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
