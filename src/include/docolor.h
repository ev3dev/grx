/**
 ** DOTABLE.H ---- color mode operation check table
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/

extern unsigned int _GrFDdotable8[];

#define DOCOLOR8(c,op) (_GrFDdotable8[(op)] ^ (unsigned int)(unsigned char)(c))

