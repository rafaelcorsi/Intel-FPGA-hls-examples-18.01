DEVICE   := Arria10
TARGETS  := x86-64_testbench \
            fpga_testbench

CXX      := i++
RM     := rm -rfv

#Get Path to our include files. Our compiler knows where they are but gcc/g++ doesn't
HLS_INCLUDE := $(shell dirname `which i++`)/../include

.PHONY: test
test: $(TARGETS)
	@$(foreach t,$(TARGETS),echo ./$(t); ./$(t); echo "";)


.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	-$(RM) $(TARGETS) *.o $(foreach t,$(TARGETS),$(t).prj)

testbench.o : testbench.cpp accelerate.h
	g++ -c -I$(HLS_INCLUDE) $< -o $@

x86-64_accelerate.o: accelerate.cpp accelerate.h
	$(CXX) $(CXXFLAGS) -c -march=x86-64 $< -o $@

x86-64_testbench : testbench.o x86-64_accelerate.o
	$(CXX) $(CXXFLAGS) $^ -o $@

fpga_accelerate.o: accelerate.cpp accelerate.h
	$(CXX) $(CXXFLAGS) -c -march=$(DEVICE) $< -o $@

fpga_testbench : testbench.o fpga_accelerate.o
	$(CXX) $(CXXFLAGS) $^ -o $@

