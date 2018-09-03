CXX = mpic++ -march=native -std=c++11 -O3 -DNDEBUG -g -Wall

.PHONY: rebuild all clean
	
TARGETDIR = build$(SUBDIR)
LIBDIR = lib${SUBDIR}

SRCDIR = ./src
LIB_HEADER_DIR = ./include

LIB_HEADER = $(LIB_HEADER_DIR)/synchronized_barrier.hpp
LIB_SOURCE = $(LIB_HEADER_DIR)/synchronized_barrier.cpp

EXAMPLE = $(TARGETDIR)/example.o $(LIBDIR)/libmpisyncbarrier.a

all : $(TARGETDIR) $(LIBDIR) $(LIBDIR)/libmpisyncbarrier.a $(TARGETDIR)/example
	
# Create the executable 'example'
$(TARGETDIR)/example : $(EXAMPLE)
	$(CXX) -o $@ $(EXAMPLE)
	
# Create library 'libmpisyncbarrier.a'
$(LIBDIR)/libmpisyncbarrier.a : $(TARGETDIR)/synchronized_barrier.o
	ar rcs $@ $<

# Compile example file
$(TARGETDIR)/example.o : $(SRCDIR)/example.cpp $(LIB_HEADER)
	$(CXX) -c -o $@ $< -I$(LIB_HEADER_DIR)
	
# Compile library source file
$(TARGETDIR)/synchronized_barrier.o : $(LIB_SOURCE) $(LIB_HEADER)
	$(CXX) -c -o $@ $<

# Directories
clean : 
	rm -rf $(TARGETDIR)
	rm -rf $(LIBDIR)
	
$(LIBDIR) :
	mkdir -p $(LIBDIR)
	
$(TARGETDIR) :
	mkdir -p $(TARGETDIR)/	
	
rebuild : clean all
