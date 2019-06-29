DEVICE   := Arria10
SRC      := tutorial.cpp
TARGETS  := tutorial-sp \
            tutorial-dp
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE)

RM       := rm -rfv

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)
	@echo "Compilation complete.  Open the README and example the report files to examine the impact of double vs. single precision operations."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) : % : $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

tutorial-sp: CXXFLAGS := $(CXXFLAGS)
tutorial-dp: CXXFLAGS := $(CXXFLAGS) -DUSE_DOUBLE_PRECISION
