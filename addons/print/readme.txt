Printing from GRX (currently DJGPP version only)

This is source of beta version of printing procedures for GRX.
These procedures are based on sources of printer BGI drivers
for Borland C++ and Pascal compilers. This BGI driver was 
developed by Ullrich von Bassevitz (see copying.uz)

Current version is tested with DJGPP only. I didn't even try 
to compile it with Borland C++ as I think it is useless and 
under Linux. Perhaps it could also be used under Linux (it 
is only necessary to write printer filter to let PCL through 
to real printer without changes. As I remember there are 
such filters available). I havent done this yet (maybe sometimes 
in future). This code will not work in real mode under Borland C++
as there is not enough memory for buffers and I didn't want
to port memory managment part of printer BGI driver (I don't 
think it is necessary)

Only some modes are tested:
	Epson LQ printer : 180x180 dpi
        LaserJet 4L : 300x300 dpi (with and without compression)
I also tried DeskJet 500C mode (300x300 dpi with separate black)
on DeskJet 690C and it worked.

The code for LaserJet and DeskJet printers is not optimized and
does not compiles Ok with -O3 if some of static functions are
made inlined (DJGPP likes this). Therefore these functions are
moved to end to avoid inlining them.

I wrote this code to avoid linking it unless it is really
used. It can be included as addon to GRX and also in main 
source distribution. 

--------------------  Files  -------------------------------------------
grxprint.c	- main sources of printing code
grxprint.h      - interface definitions for user
prndata.c       - printer definitions 
grxprn00.h      - definitions used internally by grxprint only
test.cc         - test example
print.gpr       - RHIDE project file for test           
copying.uz      - original copyright notice from Ullrich von Bassevitz
printer.doc     - original docs on printer BGI driver
------------------------------------------------------------------------

NOTE:   Ullrich von Bassevitz is no more maintaining printer BGI driver.
        Addresses mentioned in printer.doc are NO MORE USABLE


Andris Pavenis        
e-mail: pavenis@acad.latnet.lv
