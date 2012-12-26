#########################
CXXFILES := main.cpp code.cpp tree.cpp node.cpp
PROG := huffman
CXXFLAGS := "-std=c++0x" -Wall -pedantic -g
LDFLAGS :=
########################

# -MMD generates dependencies while compiling
CFLAGS += -MMD
CC := g++

OBJFILES := $(CXXFILES:.cpp=.o)
DEPFILES := $(CXXFILES:.cpp=.d)

$(PROG) : $(OBJFILES)
	$(LINK.o) $(LDFLAGS) -o $@ $^

clean :
	rm -f $(PROG) $(OBJFILES) $(DEPFILES)

-include $(DEPFILES)
