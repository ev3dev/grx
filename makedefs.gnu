##################### CONFIGURATION ######################

# Specify version of GRX
# Currently only used to generate name of shared libraries for linux
GRX_VERSION=2.4.0

# Specify if you have libtiff.a and corresponding .h files.
# Change setting to 'y' if you have it, or to 'n' if not.
HAVE_LIBTIFF=n

# Specify if you have libjpeg.a and corresponding .h files.
HAVE_LIBJPEG=n

# Specify if one of the above libs requires the
# zlib compression library
NEED_ZLIB=n

# Specify if you want to include printing code from addons
INCLUDE_PRINTING_CODE=y

# Specify if you want to include bmp image code from addons
INCLUDE_BMP_CODE=y

# Specify if you want to include GNU_Pascal (gpc) support
INCLUDE_GPC_SUPPORT=n

#################### SYSTEM SETTINGS #####################

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
DOS_DJGPP_V1              = $(SYSTEM_TAG_PREFIX).005
WIN32_GCC_i386_STATIC     = $(SYSTEM_TAG_PREFIX).006

ifdef   DEBUG
CCOPT=  -O2 -Wall -g -DDEBUG=$(DEBUG)
LDOPT=  -g
else
CCOPT=  -O6 -Wall
LDOPT=
endif

ifdef PROFILE
CCOPT += -pg
endif

# Additional warnings for development
WARNOPTS = -W -Wshadow -Wpointer-arith -Wbad-function-cast \
	   -Wcast-align -Wconversion -Wmissing-prototypes  \
	   -Wnested-externs -Wstrict-prototypes
#CCOPT += $(WARNOPTS)

# place libgrx20.a in lib/unix by default
GRX_LIB_SUBDIR=unix
