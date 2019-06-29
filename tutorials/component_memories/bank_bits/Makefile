DEVICE   := Arria10
SRC      := tutorial.cpp
TARGETS  := tutorial-no-bank-bits \
            tutorial-bad-bank-bits \
            tutorial-good-bank-bits
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE) --simulator none
RM       := rm -rfv

.PHONY: test
test: $(TARGETS)
	@echo "Compilation complete.  Open the README and the report files to examine the impact of properly selecting banking bits."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) : % : $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

tutorial-no-bank-bits: CXXFLAGS := $(CXXFLAGS)
tutorial-bad-bank-bits: CXXFLAGS := $(CXXFLAGS) -DBANKBITS_BAD
tutorial-good-bank-bits: CXXFLAGS := $(CXXFLAGS) -DBANKBITS_GOOD
