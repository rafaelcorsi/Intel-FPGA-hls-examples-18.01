DEVICE   := Arria10
TARGETS  := 1_ptr_swap_mem \
            3_deep_cpy_reg
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE)
RM       := rm -rfv

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)
	@echo "Compilation complete.  Open the README and the report files to examine the impact of using pointer swap as opposed to deep copy."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) :
	$(CXX) $(CXXFLAGS) $@.cpp -o $@
