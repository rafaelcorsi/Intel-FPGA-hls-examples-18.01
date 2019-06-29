DEVICE   := Arria10
SRC      := main.cpp filter.cpp
TARGETS  := tutorial
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE)
RM       := rm -rf

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)
	@echo "Compilation complete. Open the area report and observe the area associated with the two implementations."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) : % : $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

tutorial: CXXFLAGS := $(CXXFLAGS)
