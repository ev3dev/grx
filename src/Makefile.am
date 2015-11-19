# preamble

NULL =
AM_CPPFLAGS =
AM_CFLAGS =
BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST =
DISTCLEANFILES =
lib_LTLIBRARIES =
bin_PROGRAMS =

AM_CPPFLAGS += \
    $(GRX_CFLAGS) \
    $(NULL)

AM_CFLAGS += \
    -DGRX_DEFAULT_FONT_PATH=\"$(prefix)/share/$(PACKAGE)/fonts\" \
    -I$(top_srcdir)/include \
    -Iinclude \
    $(GRX_CFLAGS) \
    $(NULL)

source_h = \
    $(top_srcdir)/include/grx-3.0.h \
    $(top_srcdir)/include/grxkeys.h \
    $(NULL)

source_c = \
    draw/bitblt.c       \
    draw/bitbltnc.c     \
    draw/bitblt1b.c     \
    draw/box.c          \
    draw/boxnc.c        \
    draw/clearclp.c     \
    draw/clearctx.c     \
    draw/clearscr.c     \
    draw/drwinlne.c     \
    draw/fillbox.c      \
    draw/fillboxn.c     \
    draw/frambox.c      \
    draw/framboxn.c     \
    draw/getscl.c       \
    draw/line.c         \
    draw/linenc.c       \
    draw/majorln1.c     \
    draw/majorln2.c     \
    draw/majorln3.c     \
    draw/majorln4.c     \
    draw/pixel.c        \
    draw/pixelc.c       \
    draw/plot.c         \
    draw/putscl.c       \
    draw/flodspil.c     \
    fdrivers/dotab8.c   \
    fdrivers/ftable.c   \
    fdrivers/genblit.c  \
    fdrivers/gengiscl.c \
    fdrivers/genptscl.c \
    fdrivers/genstrch.c \
    fdrivers/pblitr2r.c \
    fdrivers/pblitr2v.c \
    fdrivers/pblitv2r.c \
    fdrivers/pblitv2v.c \
    fdrivers/ram1.c     \
    fdrivers/ram4.c     \
    fdrivers/ram8.c     \
    fdrivers/ram16.c    \
    fdrivers/ram24.c    \
    fdrivers/ram32l.c   \
    fdrivers/ram32h.c   \
    fdrivers/rblit_14.c \
    fonts/fdv_grx.c     \
    fonts/fdv_raw.c     \
    fonts/fdv_fna.c     \
    fonts/fdv_win.c     \
    fonts/fdtable.c     \
    fonts/pc6x8.c       \
    fonts/pc8x8.c       \
    fonts/pc8x14.c      \
    fonts/pc8x16.c      \
    image/ialloc.c      \
    image/ibuild.c      \
    image/ifbox.c       \
    image/ihline.c      \
    image/iinverse.c    \
    image/imginlne.c    \
    image/iplot.c       \
    image/istretch.c    \
    gformats/ctx2pnm.c  \
    gformats/pnm2ctx.c  \
    mouse/bldcurs.c     \
    mouse/drawcurs.c    \
    mouse/lnxinpu2.c    \
    mouse/lnxkeys.c     \
    mouse/mouinfo.c     \
    mouse/mouinlne.c    \
    mouse/mscursor.c    \
    mouse/mstime.c      \
    pattern/fillpatt.c  \
    pattern/makepat.c   \
    pattern/patfbits.c  \
    pattern/patfbox.c   \
    pattern/patfcvxp.c  \
    pattern/patfline.c  \
    pattern/patfplot.c  \
    pattern/patfpoly.c  \
    pattern/patternf.c  \
    pattern/pattfldf.c  \
    pattern/pattline.c  \
    pattern/pattpoly.c  \
    pattern/pfcirc.c    \
    pattern/pfcirca.c   \
    pattern/pfelli.c    \
    pattern/pfellia.c   \
    pattern/ptcirc.c    \
    pattern/ptcirca.c   \
    pattern/ptelli.c    \
    pattern/ptellia.c   \
    setup/clip.c        \
    setup/clrinfo.c     \
    setup/clrinlne.c    \
    setup/colorbw.c     \
    setup/colorega.c    \
    setup/colors.c      \
    setup/context.c     \
    setup/cxtinfo.c     \
    setup/cxtinlne.c    \
    setup/drvinfo.c     \
    setup/drvinlne.c    \
    setup/fframe.c      \
    setup/fgeom.c       \
    setup/hooks.c       \
    setup/modewalk.c    \
    setup/setdrvr.c     \
    setup/setmode.c     \
    setup/viewport.c    \
    shape/circle1.c     \
    shape/circle2.c     \
    shape/circle3.c     \
    shape/circle4.c     \
    shape/drawpoly.c    \
    shape/fillcir1.c    \
    shape/fillcir2.c    \
    shape/fillcnvx.c    \
    shape/fillell1.c    \
    shape/fillell2.c    \
    shape/fillpoly.c    \
    shape/flood.c       \
    shape/floodfil.c    \
    shape/genellip.c    \
    shape/polygon.c     \
    shape/polyline.c    \
    shape/scancnvx.c    \
    shape/scanellp.c    \
    shape/scanpoly.c    \
    shape/solidfil.c    \
    text/buildaux.c     \
    text/buildfnt.c     \
    text/convfont.c     \
    text/drawstrg.c     \
    text/drawtext.c     \
    text/drwstrg.c      \
    text/dumpfna.c      \
    text/dumpfont.c     \
    text/dumptext.c     \
    text/epatstrg.c     \
    text/fntinlne.c     \
    text/fontinfo.c     \
    text/fontpath.c     \
    text/loadfont.c     \
    text/pattstrg.c     \
    text/propwdt.c      \
    text/unloadfn.c     \
    user/ubox.c         \
    user/ucbox.c        \
    user/uccirc.c       \
    user/uccirca.c      \
    user/ucelli.c       \
    user/ucellia.c      \
    user/ucirc.c        \
    user/ucirca.c       \
    user/ucircf.c       \
    user/ucircfa.c      \
    user/ucline.c       \
    user/ucpolyg.c      \
    user/ucpolyl.c      \
    user/udrwchar.c     \
    user/udrwstrg.c     \
    user/uelli.c        \
    user/uellia.c       \
    user/uellif.c       \
    user/uellifa.c      \
    user/ufcpolyg.c     \
    user/ufillbox.c     \
    user/uflood.c       \
    user/ufpolyg.c      \
    user/uframbox.c     \
    user/ugetwin.c      \
    user/uhline.c       \
    user/uline.c        \
    user/upbox.c        \
    user/upcirc.c       \
    user/upcirca.c      \
    user/upelli.c       \
    user/upellia.c      \
    user/upfbox.c       \
    user/upfcirc.c      \
    user/upfcirca.c     \
    user/upfcpoly.c     \
    user/upfelli.c      \
    user/upfellia.c     \
    user/upfflood.c     \
    user/upfline.c      \
    user/upfplot.c      \
    user/upfpolyg.c     \
    user/upixel.c       \
    user/upixelc.c      \
    user/upline.c       \
    user/uplot.c        \
    user/upolygon.c     \
    user/upolylin.c     \
    user/uppolyg.c      \
    user/uppolyl.c      \
    user/usercord.c     \
    user/usetwin.c      \
    user/utextxy.c      \
    user/uvline.c       \
    utils/resize.c      \
    utils/ordswap.c     \
    utils/shiftscl.c    \
    utils/strmatch.c    \
    utils/tmpbuff.c     \
    vdrivers/vd_lnxfb.c \
    vdrivers/vd_mem.c   \
    vdrivers/vtable.c   \
    wideline/ccirc.c    \
    wideline/ccirca.c   \
    wideline/celli.c    \
    wideline/cellia.c   \
    wideline/custbox.c  \
    wideline/custline.c \
    wideline/custplne.c \
    wideline/custpoly.c \
    wideline/drwcpoly.c \
    gformats/ctx2jpg.c  \
    gformats/jpg2ctx.c  \
    gformats/ctx2png.c  \
    gformats/png2ctx.c  \
    $(NULL)

lib_LTLIBRARIES += libgrx-3.0.la
libgrx_3_0_la_LIBADD = $(GRX_LIBS)
libgrx_3_0_la_SOURCES = $(source_c) $(source_h)

grxincludedir = $(includedir)
grxinclude_DATA = \
    $(source_h) \
    $(NULL)

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS = Grx_3_0_gir
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir) --warn-all
INTROSPECTION_COMPILER_ARGS =

if HAVE_INTROSPECTION
introspection_sources =  \
    ../include/grx-3.0.h \
    setup/fgeom.c        \
    $(NULL)

Grx-3.0.gir: libgrx-3.0.la
Grx_3_0_gir_INCLUDES = GLib-2.0
Grx_3_0_gir_CFLAGS = $(AM_CPPFLAGS)
Grx_3_0_gir_PACKAGES =
Grx_3_0_gir_LIBS = libgrx-3.0.la
Grx_3_0_gir_FILES = $(introspection_sources)
Grx_3_0_gir_NAMESPACE = Grx
INTROSPECTION_GIRS += Grx-3.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-3.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif
