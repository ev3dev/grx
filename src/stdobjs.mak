# Standard object files

STD_1 = $(OP)draw/bitblt$(OX)       \
	$(OP)draw/bitbltnc$(OX)     \
	$(OP)draw/bitblt1b$(OX)     \
	$(OP)draw/box$(OX)          \
	$(OP)draw/boxnc$(OX)        \
	$(OP)draw/clearclp$(OX)     \
	$(OP)draw/clearctx$(OX)     \
	$(OP)draw/clearscr$(OX)     \
	$(OP)draw/drwinlne$(OX)     \
	$(OP)draw/fillbox$(OX)      \
	$(OP)draw/fillboxn$(OX)     \
	$(OP)draw/frambox$(OX)      \
	$(OP)draw/framboxn$(OX)     \
	$(OP)draw/getscl$(OX)       \
	$(OP)draw/line$(OX)         \
	$(OP)draw/linenc$(OX)       \
	$(OP)draw/majorln1$(OX)     \
	$(OP)draw/majorln2$(OX)     \
	$(OP)draw/majorln3$(OX)     \
	$(OP)draw/majorln4$(OX)     \
	$(OP)draw/pixel$(OX)        \
	$(OP)draw/pixelc$(OX)       \
	$(OP)draw/plot$(OX)         \
	$(OP)draw/putscl$(OX)       \
	$(OP)draw/flodspil$(OX)

STD_2 = $(OP)fdrivers/dotab8$(OX)   \
	$(OP)fdrivers/ftable$(OX)   \
	$(OP)fdrivers/genblit$(OX)  \
	$(OP)fdrivers/gengiscl$(OX) \
	$(OP)fdrivers/genptscl$(OX) \
	$(OP)fdrivers/genstrch$(OX) \
	$(OP)fdrivers/pblitr2r$(OX) \
	$(OP)fdrivers/pblitr2v$(OX) \
	$(OP)fdrivers/pblitv2r$(OX) \
	$(OP)fdrivers/pblitv2v$(OX) \
	$(OP)fdrivers/ram1$(OX)     \
	$(OP)fdrivers/ram16$(OX)    \
	$(OP)fdrivers/ram4$(OX)     \
	$(OP)fdrivers/ram8$(OX)     \
	$(OP)fdrivers/rblit_14$(OX)

STD_3 = $(OP)fonts/fdv_bgi$(OX)     \
	$(OP)fonts/fdv_grx$(OX)     \
	$(OP)fonts/fdv_raw$(OX)     \
	$(OP)fonts/fdv_fna$(OX)     \
	$(OP)fonts/fdv_win$(OX)     \
	$(OP)fonts/fdtable$(OX)     \
	$(OP)fonts/pc6x8$(OX)       \
	$(OP)fonts/pc8x8$(OX)       \
	$(OP)fonts/pc8x14$(OX)      \
	$(OP)fonts/pc8x16$(OX)      \
	$(OP)image/ialloc$(OX)      \
	$(OP)image/ibuild$(OX)      \
	$(OP)image/ifbox$(OX)       \
	$(OP)image/ihline$(OX)      \
	$(OP)image/iinverse$(OX)    \
	$(OP)image/imginlne$(OX)    \
	$(OP)image/iplot$(OX)       \
	$(OP)image/istretch$(OX)    \
	$(OP)gformats/ctx2pnm$(OX)  \
	$(OP)gformats/pnm2ctx$(OX)

STD_4 = $(OP)gcursors/bldcurs$(OX)  \
	$(OP)gcursors/drawcurs$(OX) \
	$(OP)input/mouinfo$(OX)     \
	$(OP)input/mouinlne$(OX)    \
	$(OP)input/mscursor$(OX)    \
	$(OP)input/grevent$(OX)     \
	$(OP)input/recode$(OX)      \
	$(OP)input/auxintl$(OX)

STD_5 = $(OP)pattern/fillpatt$(OX)  \
	$(OP)pattern/makepat$(OX)   \
	$(OP)pattern/patfbits$(OX)  \
	$(OP)pattern/patfbox$(OX)   \
	$(OP)pattern/patfcvxp$(OX)  \
	$(OP)pattern/patfline$(OX)  \
	$(OP)pattern/patfplot$(OX)  \
	$(OP)pattern/patfpoly$(OX)  \
	$(OP)pattern/patternf$(OX)  \
	$(OP)pattern/pattfldf$(OX)  \
	$(OP)pattern/pattline$(OX)  \
	$(OP)pattern/pattpoly$(OX)  \
	$(OP)pattern/pfcirc$(OX)    \
	$(OP)pattern/pfcirca$(OX)   \
	$(OP)pattern/pfelli$(OX)    \
	$(OP)pattern/pfellia$(OX)   \
	$(OP)pattern/ptcirc$(OX)    \
	$(OP)pattern/ptcirca$(OX)   \
	$(OP)pattern/ptelli$(OX)    \
	$(OP)pattern/ptellia$(OX)

STD_6 = $(OP)setup/clip$(OX)        \
	$(OP)setup/clrinfo$(OX)     \
	$(OP)setup/clrinlne$(OX)    \
	$(OP)setup/colorbw$(OX)     \
	$(OP)setup/colorega$(OX)    \
	$(OP)setup/colors$(OX)      \
	$(OP)setup/context$(OX)     \
	$(OP)setup/cxtinfo$(OX)     \
	$(OP)setup/cxtinlne$(OX)    \
	$(OP)setup/drvinfo$(OX)     \
	$(OP)setup/drvinlne$(OX)    \
	$(OP)setup/fframe$(OX)      \
	$(OP)setup/fgeom$(OX)       \
	$(OP)setup/hooks$(OX)       \
	$(OP)setup/modewalk$(OX)    \
	$(OP)setup/setdrvr$(OX)     \
	$(OP)setup/setmode$(OX)     \
	$(OP)setup/version$(OX)     \
	$(OP)setup/viewport$(OX)    \
	$(OP)shape/circle1$(OX)

STD_7 = $(OP)shape/circle2$(OX)     \
	$(OP)shape/circle3$(OX)     \
	$(OP)shape/circle4$(OX)     \
	$(OP)shape/drawpoly$(OX)    \
	$(OP)shape/fillcir1$(OX)    \
	$(OP)shape/fillcir2$(OX)    \
	$(OP)shape/fillcnvx$(OX)    \
	$(OP)shape/fillell1$(OX)    \
	$(OP)shape/fillell2$(OX)    \
	$(OP)shape/fillpoly$(OX)    \
	$(OP)shape/flood$(OX)       \
	$(OP)shape/floodfil$(OX)    \
	$(OP)shape/genellip$(OX)    \
	$(OP)shape/polygon$(OX)     \
	$(OP)shape/polyline$(OX)    \
	$(OP)shape/scancnvx$(OX)    \
	$(OP)shape/scanellp$(OX)    \
	$(OP)shape/scanpoly$(OX)    \
	$(OP)shape/solidfil$(OX)

STD_8 = $(OP)text/buildaux$(OX)     \
	$(OP)text/buildfnt$(OX)     \
	$(OP)text/convfont$(OX)     \
	$(OP)text/drawstrg$(OX)     \
	$(OP)text/drawtext$(OX)     \
	$(OP)text/drwstrg$(OX)      \
	$(OP)text/dumpfna$(OX)      \
	$(OP)text/dumpfont$(OX)     \
	$(OP)text/dumptext$(OX)     \
	$(OP)text/epatstrg$(OX)     \
	$(OP)text/fntinlne$(OX)     \
	$(OP)text/fontinfo$(OX)     \
	$(OP)text/fontpath$(OX)     \
	$(OP)text/loadfont$(OX)     \
	$(OP)text/pattstrg$(OX)     \
	$(OP)text/propwdt$(OX)      \
	$(OP)text/unloadfn$(OX)

STD_9 = $(OP)user/ubox$(OX)         \
	$(OP)user/ucbox$(OX)        \
	$(OP)user/uccirc$(OX)       \
	$(OP)user/uccirca$(OX)      \
	$(OP)user/ucelli$(OX)       \
	$(OP)user/ucellia$(OX)      \
	$(OP)user/ucirc$(OX)        \
	$(OP)user/ucirca$(OX)       \
	$(OP)user/ucircf$(OX)       \
	$(OP)user/ucircfa$(OX)      \
	$(OP)user/ucline$(OX)       \
	$(OP)user/ucpolyg$(OX)      \
	$(OP)user/ucpolyl$(OX)      \
	$(OP)user/udrwchar$(OX)     \
	$(OP)user/udrwstrg$(OX)     \
	$(OP)user/uelli$(OX)        \
	$(OP)user/uellia$(OX)       \
	$(OP)user/uellif$(OX)       \
	$(OP)user/uellifa$(OX)      \
	$(OP)user/ufcpolyg$(OX)     \
	$(OP)user/ufillbox$(OX)     \
	$(OP)user/uflood$(OX)       \
	$(OP)user/ufpolyg$(OX)      \
	$(OP)user/uframbox$(OX)     \
	$(OP)user/ugetwin$(OX)      \
	$(OP)user/uhline$(OX)       \
	$(OP)user/uline$(OX)        \
	$(OP)user/upbox$(OX)        \
	$(OP)user/upcirc$(OX)

STD_10= $(OP)user/upcirca$(OX)      \
	$(OP)user/upelli$(OX)       \
	$(OP)user/upellia$(OX)      \
	$(OP)user/upfbox$(OX)       \
	$(OP)user/upfcirc$(OX)      \
	$(OP)user/upfcirca$(OX)     \
	$(OP)user/upfcpoly$(OX)     \
	$(OP)user/upfelli$(OX)      \
	$(OP)user/upfellia$(OX)     \
	$(OP)user/upfflood$(OX)     \
	$(OP)user/upfline$(OX)      \
	$(OP)user/upfplot$(OX)      \
	$(OP)user/upfpolyg$(OX)     \
	$(OP)user/upixel$(OX)       \
	$(OP)user/upixelc$(OX)      \
	$(OP)user/upline$(OX)       \
	$(OP)user/uplot$(OX)        \
	$(OP)user/upolygon$(OX)     \
	$(OP)user/upolylin$(OX)     \
	$(OP)user/uppolyg$(OX)      \
	$(OP)user/uppolyl$(OX)      \
	$(OP)user/usercord$(OX)     \
	$(OP)user/usetwin$(OX)      \
	$(OP)user/utextxy$(OX)      \
	$(OP)user/uvline$(OX)

STD_11= $(OP)utils/resize$(OX)      \
	$(OP)utils/ordswap$(OX)     \
	$(OP)utils/shiftscl$(OX)    \
	$(OP)utils/strmatch$(OX)    \
	$(OP)utils/tmpbuff$(OX)     \
	$(OP)vdrivers/vd_mem$(OX)   \
	$(OP)vdrivers/vtable$(OX)   \
	$(OP)wideline/ccirc$(OX)    \
	$(OP)wideline/ccirca$(OX)   \
	$(OP)wideline/celli$(OX)    \
	$(OP)wideline/cellia$(OX)   \
	$(OP)wideline/custbox$(OX)  \
	$(OP)wideline/custline$(OX) \
	$(OP)wideline/custplne$(OX) \
	$(OP)wideline/custpoly$(OX) \
	$(OP)wideline/drwcpoly$(OX)

DBG_1 = $(OP)utils/dbgprint$(OX)

JPG_1 = $(OP)gformats/ctx2jpg$(OX) \
	$(OP)gformats/jpg2ctx$(OX)

NOJPG_1 = $(OP)gformats/dummyjpg$(OX)

PNG_1 = $(OP)gformats/ctx2png$(OX) \
	$(OP)gformats/png2ctx$(OX)

NOPNG_1 = $(OP)gformats/dummypng$(OX)

