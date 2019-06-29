DEVICE    := Arria10
SRC       := filter.cpp main.cpp
TARGETS   := tutorial-fp-default \
             tutorial-fp-optimized
CXX       := i++
CXXFLAGS  := $(USERCXXFLAGS) -march=$(DEVICE)
RM        := rm -rfv

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)
	@echo "Compilation complete. Open the area report and observe the area associated with the two implementations."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj) *.o

$(TARGETS) : % : $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

tutorial-fp-default:   CXXFLAGS := $(CXXFLAGS)
tutorial-fp-optimized: CXXFLAGS := $(CXXFLAGS) --fpc --fp-relaxed
