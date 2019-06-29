DEVICE   := Arria10
SRC      := tutorial.cpp
TARGETS  := tutorial-no-merge \
            tutorial-merge
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE) --simulator none
RM       := rm -rfv

.PHONY: test
test: $(TARGETS)
	@echo "Compilation complete.  Open the README and the report files to examine the impact of depth-wise merging of memories."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) : % : $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

tutorial-no-merge: CXXFLAGS := $(CXXFLAGS)
tutorial-merge: CXXFLAGS := $(CXXFLAGS) -DMERGE
