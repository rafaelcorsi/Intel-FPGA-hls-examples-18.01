DEVICE   := Arria10
SRC      := tutorial.cpp
TARGETS  := tutorial
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE)
RM       := rm -rfv

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
