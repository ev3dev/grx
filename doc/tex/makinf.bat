makeinfo --no-split grx2.tex
makeinfo --html --no-split grx2.tex
dtou grx*
texi2dvi grx2.tex
dvips grx2
dtou grx2.ps
texi2dvi --pdf grx2.tex
