
	BGI2GRX
	=======

BGI2GRX is based on GRX and BCC2GRX and only runs by using these libraries.
Pay attention to the authors copyrights.

BCC2GRX is an interface library for GCC written by Helmut Schirmer which
allows to access the GRX20 library by using Borland's BGI syntax.

GRX20 is a useful collection of graphics functions written by Csaba A. Biegl.

With BGI2GRX I give GPC users access to those functions with Gnu PASCAL.

BGI2GRX was tested with:
	GPC 2.0 (DJGPP),
	GRX 2.0 (dated 04/96) and
	BCC2GRX 2.0 (dated 10/96)
in plain DOS and in the Win 95 DOS box.
GRX20 and BCC2GRX are made to compile under LINUX so I assume that BGI2GRX
should compile and run flawlessly on a Linux box but haven't tested it yet.


!!! CAUTION !!!
---------------
For displaying text in graphics mode BCC2GRX requires you to set an
environment variable "GRXFONT" which points to the path where GRX fonts are
installed in.

You have to set the environment variable "GRX20DRV" to use different graphics
drivers. For details read the GRX documentation. I only ran GRX programs on
VESA compatible graphics cards and they worked without "GRX20DRV".

The graphics functions which use fill style and/or line style settings only
work with their default settings. This is because the required functionality
is not yet available in GRX 2.0. You can set different settings but these are
simply ignored.


Authors and sources of suply of GRX20 and BCC2GRX
-------------------------------------------------
GRX20	: csaba@vuse.vanderbildt.edu (Csaba A. Biegl)
	  ftp://ftp.simtel.net/pub/gnu/djgpp/

BCC2GRX : hsc@techfak.uni-kiel.de (Hartmut Schirmer)
	  ftp://ftp.simtel.net/pub/gnu/djgpp/
	  http://www.techfak.uni-kiel.de/~hsc/BCC2GRX/


The demos
---------
All demos (except Colors.pas) were tested with GPC as well as with Borland
Pascal. Noticable are the speed differences (especially with 8 bit color
depth) and the good compatibility (except FillStyle and LineStyle).
I noticed that some "special" BGI drivers (e.g. VESA.BGI) swap AndPut and
OrPut functionality when using PutImage.


History
-------
	12/96	- first release

Future plans
------------
	- same unit for DOS (DJGPP) and Linux (SVGALIB, X)
	- improve compatibility


Thanks to:
----------
	Hartmut Schirmer and Csaba Biegl for their help
	Peter Gerwinski for his patience and his short-term changes to GPC
	Ralf Meyer for his help on the readme translation


Have fun with GPC, GRX, BCC2GRX and BGI2GRX !!!


Sven Hilscher
mailto: sven@rufus.central.de

Any feedback is welcome (criticism, suggestions, assistance).
