#---------------------------------------------------
# Olivier Dadoun Makefile for
# BDSIM display 
# Amended Steve Malton

ARCH         := $(shell root-config --arch)
ifeq ($(ARCH),macosx)
LIBFILES = libBDS.so libBDS.dylib
ifndef ROOTSYS
ROOTSYS       = /sw/
endif
else
LIBFILES = libBDS.so
endif

CXX           = g++
LD            = g++
CXXFLAGS      = $(shell $(ROOTSYS)/bin/root-config --cflags) -I../..
LDFLAGS       = 

ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
LIBS          = $(ROOTLIBS) -L../../parser -lgmad

HDRS          = BDSCanvas.h BDSPad.h BDSAxis.h BDSParser.h BDSBox.h BDSPopUp.h

SRCS          = main.C BDSCanvas.C BDSPad.C BDSAxis.C BDSParser.C BDSBox.C BDSPopUp.C mydict.C 

OBJS          = BDSCanvas.o BDSPad.o BDSAxis.o BDSParser.o BDSBox.o BDSPopUp.o mydict.o

PROGRAM       = display

all:            $(PROGRAM) $(LIBFILES)

libBDS.so :
	@$(LD) -O -bundle -undefined dynamic_lookup $(OBJS) -o $@

libBDS.dylib : 
	@$(LD) -O2 -dynamiclib -single_module -undefined dynamic_lookup	-install_name $@ -o $@ $(OBJS)

$(PROGRAM):     $(OBJS) main.o
	@echo "Linking $(PROGRAM) ..."
	@/bin/rm -f $(PROGRAM)
	@$(LD) $(LDFLAGS) $(OBJS) main.o $(LIBS) -o $(PROGRAM)
	@chmod 555 $(PROGRAM)
	@echo "done"

clean:
	rm -f $(OBJS) $(PROGRAM) main.o mydict.*

###
BDSCanvas.o: BDSCanvas.h
BDSPad.o: BDSPad.h
BDSAxis.o: BDSAxis.h
BDSParser.o: BDSParser.h
BDSBox.o: BDSBox.h
BDSPopUp.o: BDSPopUp.h

mydict.C: BDSCanvas.h BDSPad.h BDSAxis.h BDSParser.h BDSBox.h BDSPopUp.h
	@echo "Generating dictionary ..."
	@rootcint mydict.C -c -I../.. BDSCanvas.h BDSPad.h BDSAxis.h BDSParser.h BDSBox.h BDSPopUp.h
#---------------------------------------------------
