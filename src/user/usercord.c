/** 
 ** USERCORD.C 
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
#include "usercord.h"

void GrSetUserWindow(int x1,int y1,int x2,int y2)
{
        CURC->gc_usrxbase  = x1;
        CURC->gc_usrybase  = y1;
        CURC->gc_usrwidth  = x2 - x1;
        CURC->gc_usrheight = y2 - y1;
}

void GrGetUserWindow(int *x1,int *y1,int *x2,int *y2)
{
        *x1 = CURC->gc_usrxbase;
        *y1 = CURC->gc_usrybase;
        *x2 = CURC->gc_usrxbase + CURC->gc_usrwidth;
        *y2 = CURC->gc_usrybase + CURC->gc_usrheight;
}

void GrGetScreenCoord(int *x,int *y)
{
        U2SX(*x,CURC);
        U2SY(*y,CURC);
}

void GrGetUserCoord(int *x,int *y)
{
        S2UX(*x,CURC);
        S2UY(*y,CURC);
}
