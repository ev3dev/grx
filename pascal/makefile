# --------------------------------
# Check, if the paths are correct 
# --------------------------------

# Set the library-path to libgrx
GRXLIB = -L/usr/X11R6/lib

# Set the library-path to libjpeg and libtiff
GRAPHICSLIB = -L/usr/lib

# Compiler and optins on your system  
COMPILER = gpc --automake -Wall

# Needed libraries (at least jpeg and tiff)
LIBRARIES= -ljpeg -ltiff 

# --------------------------------------------
# I Think, you don't need to change from here
# --------------------------------------------

all: blt_test modetest taste txt_test vir_test

blt_test:blt_test.pas
	$(COMPILER) $(GRXLIB) $(GRAPHICSLIB) $(LIBRARIES) blt_test.pas -oblt_test

modetest:modetest.pas
	$(COMPILER) $(GRXLIB) $(GRAPHICSLIB) $(LIBRARIES) modetest.pas -omodetest

taste:	taste.pas
	$(COMPILER) $(GRXLIB) $(GRAPHICSLIB) $(LIBRARIES) taste.pas -otaste

txt_test:txt_test.pas
	$(COMPILER) $(GRXLIB) $(GRAPHICSLIB) $(LIBRARIES) txt_test.pas -otxt_test

vir_test:vir_test.pas
	$(COMPILER) $(GRXLIB) $(GRAPHICSLIB) $(LIBRARIES) vir_test.pas -ovir_test

clean:
	rm -f *.o *.gpi blt_test modetest taste txt_test vir_test


