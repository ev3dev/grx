/**
 ** PATFPOLY.C
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **
 **  Copyright (C) 1992, Csaba Biegl
 **    820 Stirrup Dr, Nashville, TN, 37221
 **    csaba@vuse.vanderbilt.edu
 **
 **  This file is distributed under the terms listed in the document
 **  "copying.cb", available from the author at the address above.
 **  A copy of "copying.cb" should accompany this file; if not, a copy
 **  should be available from where this file was obtained.  This file
 **  may not be distributed without a verbatim copy of "copying.cb".
 **  You should also have received a copy of the GNU General Public
 **  License along with this program (it is in the file "copying");
 **  if not, write to the Free Software Foundation, Inc., 675 Mass Ave,
 **  Cambridge, MA 02139, USA.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrPatternFilledPolygon(int n,int pt[][2],GrPattern *p)
{
        GrFiller *grf = (GrFiller *)malloc(sizeof(GrFiller));
        GrFillArg fa;

        grf->pixel = _GrDrawPatternedPixel;
        grf->line  = _GrDrawPatternedLine;
        grf->scan  = _GrFillPatternedScanLine;
        fa.p = p;
        if(n <= 3) _GrScanConvexPoly(n,pt,grf,fa);
        else _GrScanPolygon(n,pt,grf,fa);
}

