SRC      := tutorial.cpp
TARGETS  := tutorial \
            tutorial-warning \
            tutorial-error
RM       := rm -rf

HLS_INSTALL_DIR := $(shell which i++ | sed 's|/bin/i++||g')

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

tutorial:            CXX = i++
tutorial:            CXXFLAGS = -march=x86-64

tutorial-warning:    CXX = i++
tutorial-warning:    CXXFLAGS = -march=x86-64 -D DEBUG_AC_INT_WARNING

tutorial-error:      CXX = i++
tutorial-error:      CXXFLAGS = -march=x86-64 -D DEBUG_AC_INT_ERROR