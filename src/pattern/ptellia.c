/**
 ** PTELLIA.C
 **
 ** Copyright (C) 1997, Michael Goffioul
 ** [e-mail : goffioul@emic.ucl.ac.be]
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
#include "allocate.h"
#include "shapes.h"

void GrPatternedEllipseArc(int xc,int yc,int xa,int ya,int start,int end,int style,GrLinePattern *lp)
{
    int (*points)[2];
    setup_ALLOC();
    points = ALLOC(sizeof(int) * 2 * (GR_MAX_ELLIPSE_POINTS + 2));
    if (points != NULL)
    {
        int numpts = GrGenerateEllipseArc(xc,yc,xa,ya,start,end,points);
        GrFillArg fval;
        int close;

        close = FALSE;
        if (style == GR_ARC_STYLE_CLOSE2) {
                points[numpts][0] = xc;
                points[numpts][1] = yc;
                numpts++;
                points[numpts][0] = points[0][0];
                points[numpts][1] = points[0][1];
                numpts++;
                close = TRUE;
        }
        if (style == GR_ARC_STYLE_CLOSE1) {
                points[numpts][0] = points[0][0];
                points[numpts][1] = points[0][1];
                numpts++;
                close = TRUE;
        }
        fval.p = lp->lnp_pattern;
        _GrDrawCustomPolygon(numpts,points,lp->lnp_option,
                             &_GrPatternFiller,fval,close,TRUE);
        FREE(points);
    }
    reset_ALLOC();
}
