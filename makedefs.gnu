CC=     gcc
AR=     ar
RANLIB= ranlib
STRIP=  strip

# Different systems / setups may generate .o files
# this tag files will show what version is present
SYSTEM_TAG_PREFIX=systag
LINUX_i386_SVGALIB_STATIC = $(SYSTEM_TAG_PREFIX).000
LINUX_i386_SVGALIB_SHARED = $(SYSTEM_TAG_PREFIX).001
LINUX_i386_X11_STATIC     = $(SYSTEM_TAG_PREFIX).002
LINUX_i386_X11_SHARED     = $(SYSTEM_TAG_PREFIX).003
DOS_DJGPP_V2              = $(SYSTEM_TAG_PREFIX).004

ifdef   DEBUG
CCOPT=  -O6 -Wall -g -DDEBUG=$(DEBUG)
LDOPT=  -g
else
CCOPT=  -O6 -Wall -fomit-frame-pointer
LDOPT=
endif
