/** 
 ** UCIRCLE.C 
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
 **
 **
 **        Copyright (C) 1997, Michael Goffioul
 **
 **          goffioul@emic.ucl.ac.be
 **
 **
 ** Note : compiling the library with -DUSR_KEEP_SHAPE makes a circle
 **           looks like a circle on the screen
 **/

#include "libgrx.h"
#include "usercord.h"

static void udrawellip(int xc,int yc,int xa,int ya,long c,int filled)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->gc_xmax,CURC->gc_usrwidth);
        SCALE(ya,ya,CURC->gc_ymax,CURC->gc_usrheight);
        (*(filled ? GrFilledEllipse : GrEllipse))(xc,yc,xa,ya,c);
}

void GrUsrCircle(int xc,int yc,int r,long c)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrCircle(xc,yc,r,c);
#else
        udrawellip(xc,yc,r,r,c,FALSE);
#endif /* USR_KEEP_SHAPE */
}

void GrUsrFilledCircle(int xc,int yc,int r,long c)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrFilledCircle(xc,yc,r,c);
#else
        udrawellip(xc,yc,r,r,c,TRUE);
#endif /* USR_KEEP_SHAPE */
}

void GrUsrEllipse(int xc,int yc,int xa,int ya,long c)
{
        udrawellip(xc,yc,xa,ya,c,FALSE);
}

void GrUsrFilledEllipse(int xc,int yc,int xa,int ya,long c)
{
        udrawellip(xc,yc,xa,ya,c,TRUE);
}
