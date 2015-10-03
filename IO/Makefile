
SHLIB = libORIO.$(SOSUFFIX)
    
ORFLAGS = $(patsubst %,-I$(INSTALLDIR)/%,Util Decoders Processors IO)
ORLIBS = -L$(INSTALLDIR)/lib -lORUtil -lORDecoders

include ../buildTools/BasicMakefile
