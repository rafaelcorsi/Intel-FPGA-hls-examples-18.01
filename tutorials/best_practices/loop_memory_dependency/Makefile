DEVICE   := Arria10
SRC      := transpose_and_fold.cpp
TARGETS  := transpose_and_fold_no_ivdep \
            transpose_and_fold_ivdep
CXX      := i++
CXXFLAGS := $(USERCXXFLAGS) -march=$(DEVICE)
RM       := rm -rfv

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)
	@echo "Compilation complete.  Open the README and the report files to examine the impact of using the ivdep pragma."

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) $(foreach t,$(TARGETS),$(t).prj)

$(TARGETS) : % : $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

transpose_and_fold_ivdep: CXXFLAGS := $(CXXFLAGS) -DUSE_IVDEP
